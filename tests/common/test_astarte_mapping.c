/**
 * This file is part of Astarte.
 *
 * Copyright 2018-2023 SECO Mind Srl
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later OR Apache-2.0
 *
 **/
#include "unity.h"

#include "test_astarte_mapping.h"
#include "astarte_mapping.h"

#include <esp_log.h>

#define TAG "MAPPING TEST"

void test_astarte_mapping_validate_path(void) {
    const astarte_mapping_t mapping = {
        .endpoint = "/interface_name/%{param1}/endpo1nt/endpoint2",
        .type = TYPE_INTEGER,
        .reliability = RELIABILITY_UNIQUE,
        .explicit_timestamp = false,
        .allow_unset = true,
    };
    bool validation_res;

    // Correct path
    char ok_path[] = "/interface_name/Parameter_6_nAme/endpo1nt/endpoint2";
    TEST_ASSERT_EQUAL(ASTARTE_OK, validate_path(mapping, ok_path, &validation_res));
    TEST_ASSERT_TRUE(validation_res);

    // Empty path
    char empty_path[] = "";
    TEST_ASSERT_EQUAL(ASTARTE_OK, validate_path(mapping, empty_path, &validation_res));
    TEST_ASSERT_FALSE(validation_res);

    // No slash path
    char no_slash_path[] = "interface_name/Parameter_6_nAme/endpo1nt/endpoint2";
    TEST_ASSERT_EQUAL(ASTARTE_OK, validate_path(mapping, no_slash_path, &validation_res));
    TEST_ASSERT_FALSE(validation_res);

    // Short path
    char short_path[] = "/interface_name/Parameter_6_nAme/endpo1nt";
    TEST_ASSERT_EQUAL(ASTARTE_OK, validate_path(mapping, short_path, &validation_res));
    TEST_ASSERT_FALSE(validation_res);

    // long path
    char long_path[] = "/interface_name/Parameter_6_nAme/endpo1nt/endpoint2/endpoint3";
    TEST_ASSERT_EQUAL(ASTARTE_OK, validate_path(mapping, long_path, &validation_res));
    TEST_ASSERT_FALSE(validation_res);

    // missing segment path
    char missing_segment_path[] = "/interface_name/Parameter_6_nAme/endpoint2";
    TEST_ASSERT_EQUAL(ASTARTE_OK, validate_path(mapping, missing_segment_path, &validation_res));
    TEST_ASSERT_FALSE(validation_res);

    // incorrect first segment path
    char first_seg_path[] = "/interfac_name/Parameter_6_nAme/endpo1nt/endpoint2";
    TEST_ASSERT_EQUAL(ASTARTE_OK, validate_path(mapping, first_seg_path, &validation_res));
    TEST_ASSERT_FALSE(validation_res);

    // incorrect param segment path
    char param_seg_path[] = "/interface_name/Parame.ter_6_nAme/endpo1nt/endpoint2";
    TEST_ASSERT_EQUAL(ASTARTE_OK, validate_path(mapping, param_seg_path, &validation_res));
    TEST_ASSERT_FALSE(validation_res);

    // incorrect middle segment path
    char middl_seg_path[] = "/interface_name/Parameter_6_nAme/endpo11nt/endpoint2";
    TEST_ASSERT_EQUAL(ASTARTE_OK, validate_path(mapping, middl_seg_path, &validation_res));
    TEST_ASSERT_FALSE(validation_res);

    // incorrect last segment path
    char last_seg_path[] = "/interface_name/Parameter_6_nAme/endpo1nt/enndpoint2";
    TEST_ASSERT_EQUAL(ASTARTE_OK, validate_path(mapping, last_seg_path, &validation_res));
    TEST_ASSERT_FALSE(validation_res);
}
