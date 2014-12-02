/*
 * Copyright 2014 MongoDB, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MONGOC_SERVER_SELECTION_H
#define MONGOC_SERVER_SELECTION_H

#include "mongoc-read-prefs-private.h"
#include "mongoc-server-description.h"
#include "mongoc-topology-description.h"

#define MONGOC_SS_DEFAULT_TIMEOUT_MS 30000
#define MONGOC_SS_DEFAULT_LOCAL_THRESHOLD_MS 15

typedef enum
   {
      MONGOC_SS_READ,
      MONGOC_SS_WRITE
   } mongoc_ss_optype_t;

mongoc_server_description_t *_mongoc_ss_select (mongoc_ss_optype_t             optype,
                                                mongoc_topology_description_t *topology,
                                                const mongoc_read_prefs_t     *read_pref,
                                                bson_error_t                  *error);

#endif
