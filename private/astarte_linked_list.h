/*
 * (C) Copyright 2018-2023, SECO Mind Srl
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later OR Apache-2.0
 */

/**
 * @file astarte_linked_list.h
 * @brief Utility module containing a basic linked list implementation.
 *
 * @details This library does not perform deep copies when storing values.
 * The user should ensure that any value is freed correctly when appropriate.
 */

#ifndef _ASTARTE_LINKED_LIST_H_
#define _ASTARTE_LINKED_LIST_H_

#include <stdbool.h>
#include <stdlib.h>

#include "astarte.h"

typedef struct
{
    struct astarte_linked_list_node *head;
    struct astarte_linked_list_node *tail;
} astarte_linked_list_handle_t;

typedef struct
{
    astarte_linked_list_handle_t *handle;
    struct astarte_linked_list_node *node;
} astarte_linked_list_iterator_t;

typedef struct
{
    void *value;
    size_t value_len;
} astarte_linked_list_item_t;

/**
 * @brief Initializes a new empty linked list
 *
 * @return A handle to the newly initialized list
 */
astarte_linked_list_handle_t astarte_linked_list_init(void);

/**
 * @brief Checks if the linked list is empty
 *
 * @param handle Linked list handle
 * @return true if the list is empty, false otherwise
 */
bool astarte_linked_list_is_empty(astarte_linked_list_handle_t *handle);

/**
 * @brief Append an item to the end of a linked list
 *
 * @param handle Linked list handle
 * @param item Element to append to the linked list
 * @return One of the follwing error codes:
 * - ASTARTE_ERR_OUT_OF_MEMORY if memory allocation failed,
 * - ASTARTE_OK if operation has been successful
 */
astarte_err_t astarte_linked_list_append(
    astarte_linked_list_handle_t *handle, astarte_linked_list_item_t item);

/**
 * @brief Remove and return the last item from a linked list
 *
 * @param handle Linked list handle
 * @param item Tail item retrieved from the linked list
 * @return One of the follwing error codes:
 * - ASTARTE_ERR_NOT_FOUND if list was already empty,
 * - ASTARTE_OK if operation has been successful
 */
astarte_err_t astarte_linked_list_remove_tail(
    astarte_linked_list_handle_t *handle, astarte_linked_list_item_t *item);

/**
 * @brief Find an item in a linked list
 *
 * @param handle Linked list handle
 * @param item Item that will be searched
 * @return True if the item is found, false otherwise
 */
bool astarte_linked_list_find(
    astarte_linked_list_handle_t *handle, astarte_linked_list_item_t item);

/**
 * @brief Destroy the list without de-allocating its content
 *
 * @note This function must be called on an non-empty list when its use has ended.
 * While it releases all the internal structures used by this list it does not free the content
 * of each item.
 *
 * @param handle Linked list handle
 */
void astarte_linked_list_destroy(astarte_linked_list_handle_t *handle);

/**
 * @brief Destroy the list releasing with 'free()' its content
 *
 * @note This function can be called on an non-empty list when its use has ended.
 * It releases all the internal structures used by this list as well as each item placed into the
 * list.
 *
 * @param handle Linked list handle
 */
void astarte_linked_list_destroy_and_release(astarte_linked_list_handle_t *handle);

/**
 * @brief Initialize an iterator over a linked list
 *
 * @note After intialization the iterator will be pointing to the first item of the list.
 *
 * @param handle Linked list handle
 * @param iterator Iterator to initialize
 * @return One of the follwing error codes:
 * - ASTARTE_ERR_NOT_FOUND if list is empty,
 * - ASTARTE_OK if operation has been successful
 */
astarte_err_t astarte_linked_list_iterator_init(
    astarte_linked_list_handle_t *handle, astarte_linked_list_iterator_t *iterator);

/**
 * @brief Advance the iterator to the next item of the list
 *
 * @param iterator Iterator to advance
 * @return One of the follwing error codes:
 * - ASTARTE_ERR_NOT_FOUND if the end of the list has been reached,
 * - ASTARTE_OK if operation has been successful
 */
astarte_err_t astarte_linked_list_iterator_advance(astarte_linked_list_iterator_t *iterator);

/**
 * @brief Get the item pointed by the iterator
 *
 * @param iterator Iterator to use for the operation
 * @param item Element retrieved from the linked list
 */
void astarte_linked_list_iterator_get_item(
    astarte_linked_list_iterator_t *iterator, astarte_linked_list_item_t *item);

#endif /* _ASTARTE_LINKED_LIST_H_ */
