/*
 * (C) Copyright 2018-2023, SECO Mind Srl
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later OR Apache-2.0
 */

/**
 * @file astarte_set.h
 * @brief Utility module containing a basic set implementation.
 *
 * @details This set is implemented as an ordered list without duplicates.
 */

#ifndef _ASTARTE_SET_H_
#define _ASTARTE_SET_H_

#include <stdbool.h>
#include <stdlib.h>

#include "astarte.h"
#include "astarte_linked_list.h"

typedef struct
{
    astarte_linked_list_handle_t list_handle;
} astarte_set_handle_t;

typedef struct
{
    void *value;
    size_t value_len;
} astarte_set_element_t;

/**
 * @brief Initializes a new empty set
 *
 * @return A handle to the newly initialized set
 */
astarte_set_handle_t astarte_set_init(void);

/**
 * @brief Checks if the set is empty
 *
 * @param handle Set handle
 * @return true if the set is empty, false otherwise
 */
bool astarte_set_is_empty(astarte_set_handle_t *handle);

/**
 * @brief Adds an item to the set
 *
 * @param handle Set handle
 * @param element Element to add to the set
 * @return One of the follwing error codes:
 * - ASTARTE_ERR_OUT_OF_MEMORY if memory allocation failed,
 * - ASTARTE_OK if intialization has been successful
 */
astarte_err_t astarte_set_add(astarte_set_handle_t *handle, astarte_set_element_t element);

/**
 * @brief Pops an item from the set
 *
 * @note Since sets are unordered collections of items this element could be any element from the
 * set
 *
 * @param handle Set handle
 * @param element Element retrieved from the set
 * @return One of the follwing error codes:
 * - ASTARTE_ERR_NOT_FOUND if set was empty,
 * - ASTARTE_OK if operation has been successful
 */
astarte_err_t astarte_set_pop(astarte_set_handle_t *handle, astarte_set_element_t *element);

/**
 * @brief Destroy the set without de-allocating its content
 *
 * @note This function must be called on an non-empty set when its use has ended.
 * While it releases all the internal structures used by this set it does not free the content
 * of each element.
 *
 * @param handle Handle to the set to destroy
 */
void astarte_set_destroy(astarte_set_handle_t *handle);

/**
 * @brief Destroy the set releasing with 'free()' its content
 *
 * @note This function must be called on an non-empty set when its use has ended.
 * It releases all the internal structures used by this set as well as each element placed into the
 * set.
 *
 * @param handle Handle to the set to destroy
 */
void astarte_set_destroy_and_release(astarte_set_handle_t *handle);

#endif /* _ASTARTE_SET_H_ */
