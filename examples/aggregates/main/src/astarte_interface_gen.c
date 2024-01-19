/*
 * (C) Copyright 2023, SECO Mind Srl
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later OR Apache-2.0
 */

/**
 * @file astarte_interface_gen.c
 * @brief Contains automatically generated interfaces.
 */

#include "astarte_interface_gen.h"

static const astarte_mapping_t astarte_mappings_org_astarteplatform_esp32_examples_DeviceAggregate[4] = {
    {
        .endpoint = "/%{sensor_id}/double_endpoint",
        .type = TYPE_DOUBLE,
        .reliability = RELIABILITY_UNRELIABLE,
        .explicit_timestamp = true,
        .allow_unset =  false,
    },
    {
        .endpoint = "/%{sensor_id}/integer_endpoint",
        .type = TYPE_INTEGER,
        .reliability = RELIABILITY_UNRELIABLE,
        .explicit_timestamp = true,
        .allow_unset =  false,
    },
    {
        .endpoint = "/%{sensor_id}/boolean_endpoint",
        .type = TYPE_BOOLEAN,
        .reliability = RELIABILITY_UNRELIABLE,
        .explicit_timestamp = true,
        .allow_unset =  false,
    },
    {
        .endpoint = "/%{sensor_id}/doublearray_endpoint",
        .type = TYPE_DOUBLEARRAY,
        .reliability = RELIABILITY_UNRELIABLE,
        .explicit_timestamp = true,
        .allow_unset =  false,
    },
};

const astarte_interface_t astarte_interface_org_astarteplatform_esp32_examples_DeviceAggregate = {
    .name = "org.astarteplatform.esp32.examples.DeviceAggregate",
    .major_version = 0,
    .minor_version = 1,
    .type = TYPE_DATASTREAM,
    .ownership = OWNERSHIP_DEVICE,
    .aggregation = AGGREGATION_OBJECT,
    .mappings = astarte_mappings_org_astarteplatform_esp32_examples_DeviceAggregate,
};

static const astarte_mapping_t astarte_mappings_org_astarteplatform_esp32_examples_ServerAggregate[2] = {
    {
        .endpoint = "/%{sensor_id}/booleanarray_endpoint",
        .type = TYPE_BOOLEANARRAY,
        .reliability = RELIABILITY_UNRELIABLE,
        .explicit_timestamp = true,
        .allow_unset =  false,
    },
    {
        .endpoint = "/%{sensor_id}/longinteger_endpoint",
        .type = TYPE_LONGINTEGER,
        .reliability = RELIABILITY_UNRELIABLE,
        .explicit_timestamp = true,
        .allow_unset =  false,
    },
};

const astarte_interface_t astarte_interface_org_astarteplatform_esp32_examples_ServerAggregate = {
    .name = "org.astarteplatform.esp32.examples.ServerAggregate",
    .major_version = 0,
    .minor_version = 1,
    .type = TYPE_DATASTREAM,
    .ownership = OWNERSHIP_SERVER,
    .aggregation = AGGREGATION_OBJECT,
    .mappings = astarte_mappings_org_astarteplatform_esp32_examples_ServerAggregate,
};
