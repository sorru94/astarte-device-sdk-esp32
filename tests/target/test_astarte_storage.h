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

#ifndef _TEST_ASTARTE_STORAGE_H_
#define _TEST_ASTARTE_STORAGE_H_

#ifdef __cplusplus
extern "C" {
#endif

void test_astarte_storage_store_delete_cycle(void);
void test_astarte_storage_store_null_deletes(void);
void test_astarte_storage_contains(void);
void test_astarte_storage_clear(void);
void test_astarte_storage_iteration(void);
void test_astarte_storage_iteration_empty_memory(void);

#ifdef __cplusplus
}
#endif

#endif /* _TEST_ASTARTE_STORAGE_H_ */