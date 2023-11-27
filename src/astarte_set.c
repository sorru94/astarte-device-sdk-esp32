/*
 * (C) Copyright 2018-2023, SECO Mind Srl
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later OR Apache-2.0
 */

#include <astarte_set.h>

#include <string.h>

#include <esp_log.h>

/************************************************
 *        Defines, constants and typedef        *
 ***********************************************/

#define TAG "ASTARTE_SET"

/************************************************
 *         Global functions definitions         *
 ***********************************************/

astarte_set_handle_t astarte_set_init(void)
{
    astarte_set_handle_t handle;
    handle.list_handle = astarte_linked_list_init();
    return handle;
}

bool astarte_set_is_empty(astarte_set_handle_t *handle)
{
    return astarte_linked_list_is_empty(&handle->list_handle);
}

astarte_err_t astarte_set_add(astarte_set_handle_t *handle, astarte_set_element_t element)
{
    // Ensure that the element is not already in the set
    astarte_linked_list_item_t item = { .value = element.value, .value_len = element.value_len };
    if (astarte_linked_list_find(&handle->list_handle, item)) {
        ESP_LOGD(TAG, "Element already present in the set.");
        return ASTARTE_OK;
    }

    // Append element to the list
    return astarte_linked_list_append(&handle->list_handle, item);
}

astarte_err_t astarte_set_pop(astarte_set_handle_t *handle, astarte_set_element_t *element)
{
    astarte_linked_list_item_t item;
    astarte_err_t err = astarte_linked_list_remove_tail(&handle->list_handle, &item);
    if (err == ASTARTE_OK) {
        element->value = item.value;
        element->value_len = item.value_len;
    }
    return err;
}

void astarte_set_destroy(astarte_set_handle_t *handle)
{
    astarte_linked_list_destroy(&handle->list_handle);
}

void astarte_set_destroy_and_release(astarte_set_handle_t *handle)
{
    astarte_linked_list_destroy_and_release(&handle->list_handle);
}
