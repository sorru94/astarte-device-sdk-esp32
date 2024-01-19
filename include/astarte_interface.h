/*
 * (C) Copyright 2018-2023, SECO Mind Srl
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later OR Apache-2.0
 */

/**
 * @file astarte_interface.h
 * @brief Generic specifications for Astarte interface.
 */

#ifndef _ASTARTE_INTERFACE_H_
#define _ASTARTE_INTERFACE_H_

#include "astarte_mapping.h"

typedef enum
{
    OWNERSHIP_DEVICE = 1,
    OWNERSHIP_SERVER,
} astarte_interface_ownership_t;

typedef enum
{
    TYPE_DATASTREAM = 1,
    TYPE_PROPERTIES,
} astarte_interface_type_t;

typedef enum
{
    AGGREGATION_INDIVIDUAL = 1,
    AGGREGATION_OBJECT,
} astarte_interface_aggregation_t;

typedef struct {
    const char *name;
    int major_version;
    int minor_version;
    astarte_interface_type_t type;
    astarte_interface_ownership_t ownership;
    astarte_interface_aggregation_t aggregation;
    const astarte_mapping_t *mappings;
} astarte_interface_t;

#endif /* _ASTARTE_INTERFACE_H_ */
