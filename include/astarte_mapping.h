/*
 * (C) Copyright 2018-2023, SECO Mind Srl
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later OR Apache-2.0
 */

/**
 * @file astarte_mapping.h
 * @brief Generic specifications for Astarte interface mappings.
 */

#ifndef _ASTARTE_MAPPING_H_
#define _ASTARTE_MAPPING_H_

#include <stdbool.h>
#include "astarte.h"

typedef enum {
    TYPE_INTEGER = 1,
    TYPE_LONGINTEGER,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_BINARYBLOB,
    TYPE_BOOLEAN,
    TYPE_DATETIME,
    TYPE_INTEGERARRAY,
    TYPE_LONGINTEGERARRAY,
    TYPE_DOUBLEARRAY,
    TYPE_STRINGARRAY,
    TYPE_BINARYBLOBARRAY,
    TYPE_BOOLEANARRAY,
    TYPE_DATETIMEARRAY,
} astarte_mapping_type_t;

typedef enum {
    RELIABILITY_UNRELIABLE = 0,
    RELIABILITY_GUARANTEED = 1,
    RELIABILITY_UNIQUE = 2,
} astarte_mapping_reliability_t;

typedef struct {
    const char *endpoint;
    astarte_mapping_type_t type;
    astarte_mapping_reliability_t reliability;
    bool explicit_timestamp;
    bool allow_unset;
} astarte_mapping_t;

astarte_err_t validate_path(astarte_mapping_t mapping, const char *path, bool *result);

#endif /* _ASTARTE_MAPPING_H_ */
