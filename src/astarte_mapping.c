/*
 * (C) Copyright 2018-2023, SECO Mind Srl
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later OR Apache-2.0
 */

#include "astarte_mapping.h"

#include <sys/types.h>
#include <regex.h>
#include <string.h>

#include <esp_log.h>

#include <astarte_linked_list.h>

#define TAG "ASTARTE_MAPPING"

astarte_err_t validate_path(astarte_mapping_t mapping, const char *path, bool *result) {
    // This function should:
    // - Iterate through all the segments of the endpoint and path, using '/' as a dividing element
    // - If segment is a parameter, then match the segment in the path with the parameter regex
    // - If segment is not a parameter then perform a string comparison

    astarte_err_t validation_err = ASTARTE_OK;

    // Check someone is not passing this function NULL pointers
    if (!path || !result) {
        ESP_LOGE(TAG, "Invalid arguments to validate_path");
        return ASTARTE_ERR;
    }

    // Check for path not to be empty and to start with /
    size_t path_len = strlen(path);
    if ((path_len == 0) || (strncmp(path, "/", 1) != 0)) {
        ESP_LOGD(TAG, "Path can't be empty and has to start with '/'");
        *result = false;
        return ASTARTE_OK;
    }

    // Create a local copy of path and endpoint, it will make it possible to modify them with strtok
    char *path_cpy = calloc(path_len + 1, sizeof(char));
    size_t endpoint_len = strlen(mapping.endpoint);
    char *endpoint_cpy = calloc(endpoint_len + 1, sizeof(char));
    if (!path_cpy || !endpoint_cpy) {
        ESP_LOGE(TAG, "Out of memory %s: %d", __FILE__, __LINE__);
        return ASTARTE_ERR_OUT_OF_MEMORY;
    }
    strncpy(path_cpy, path, path_len + 1);
    strncpy(endpoint_cpy, mapping.endpoint, endpoint_len + 1);

    // Initialize the two lists that will contain the segments
    astarte_linked_list_handle_t path_segments = astarte_linked_list_init();
    astarte_linked_list_handle_t endpoint_segments = astarte_linked_list_init();
    size_t path_segments_num = 0U;
    size_t endpoint_segments_num = 0U;
    // Split all the path segments
    char *segment = strtok(path_cpy, "/");
    do {
        astarte_err_t list_err = astarte_linked_list_append(&path_segments, segment);
        if (list_err != ASTARTE_OK) {
            ESP_LOGE(TAG, "Failed adding segment %s to list", segment);
            validation_err = ASTARTE_ERR;
            goto end;
        }
        path_segments_num++;
        segment = strtok(NULL, "/");
    } while (segment);
    // Split all the endpoint segments
    segment = strtok(endpoint_cpy, "/");
    do {
        astarte_err_t list_err = astarte_linked_list_append(&endpoint_segments, segment);
        if (list_err != ASTARTE_OK) {
            ESP_LOGE(TAG, "Failed adding segment %s to list", segment);
            validation_err = ASTARTE_ERR;
            goto end;
        }
        endpoint_segments_num++;
        segment = strtok(NULL, "/");
    } while (segment);

    // Check that the number of segments is the same for both endpoint and list
    if (path_segments_num != endpoint_segments_num) {
        ESP_LOGD(TAG, "Endpoint and path have a different number of segments");
        *result = false;
        goto end;
    }

    // Iterate over the two lists
    // Two assumptions are used:
    // - Both list contain at least one element as the endpoint can't be formed by 0 segments
    // - Checking '_advance()' for a single list is sufficient as the lists are of the same size
    astarte_linked_list_iterator_t path_iter;
    astarte_linked_list_iterator_t endpoint_iter;
    astarte_linked_list_iterator_init(&path_segments, &path_iter);
    astarte_linked_list_iterator_init(&endpoint_segments, &endpoint_iter);
    astarte_err_t list_err = ASTARTE_OK;
    do {
        char *path_segment = NULL;
        char *endpoint_segment = NULL;
        astarte_linked_list_iterator_get_item(&path_iter, (void **) &path_segment);
        astarte_linked_list_iterator_get_item(&endpoint_iter, (void **) &endpoint_segment);

        // Check if endpoint segment contains a parameter, meaning it starts wiht '%{'
        if (strncmp(endpoint_segment, "%{", 2) != 0) {
            size_t path_segment_len = strlen(path_segment);
            size_t endpoint_segment_len = strlen(endpoint_segment);
            if ((path_segment_len != endpoint_segment_len) ||
                (strncmp(endpoint_segment, path_segment, endpoint_segment_len) != 0)) {
                ESP_LOGD(TAG, "Path and endpoint segments do not match");
                *result = false;
                goto end;
            }
        } else {
            // Check if path regex matches the one for parameters
            regex_t preg;
            const char pattern[] = "^[a-zA-Z_]+[a-zA-Z0-9_]*$";
            if ((regcomp(&preg, pattern, REG_EXTENDED | REG_NOSUB) != 0)) {
                ESP_LOGE(TAG, "Regex compilation failed, for pattern %s.", pattern);
                validation_err = ASTARTE_ERR;
                goto end;
            }
            int regexec_res = regexec(&preg, path_segment, 0, NULL, 0);
            regfree(&preg);
            if (regexec_res != 0) {
                ESP_LOGD(TAG, "Incorrectly formatted parameter path segment");
                *result = false;
                goto end;
            }
        }
        astarte_linked_list_iterator_advance(&path_iter);
        list_err = astarte_linked_list_iterator_advance(&endpoint_iter);
    } while (list_err != ASTARTE_ERR_NOT_FOUND);

    // No difference has been found, validation has succeeded
    *result = true;

end:
    astarte_linked_list_destroy(&path_segments);
    astarte_linked_list_destroy(&endpoint_segments);

    free(path_cpy);
    free(endpoint_cpy);

    return validation_err;
}
