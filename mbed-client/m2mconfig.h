/*
 * Copyright (c) 2015 ARM Limited. All rights reserved.
 * SPDX-License-Identifier: Apache-2.0
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef M2MCONFIG_H
#define M2MCONFIG_H

#include <stdlib.h>
#include <cstring>
#include <cstdio>
#include "mbed-client/m2mstring.h"
using namespace m2m;

#ifndef YOTTA_CFG_RECONNECTION_COUNT
#define YOTTA_CFG_RECONNECTION_COUNT 2
#endif

#ifndef YOTTA_CFG_RECONNECTION_INTERVAL
#define YOTTA_CFG_RECONNECTION_INTERVAL 5
#endif

#ifndef YOTTA_CFG_TCP_KEEPALIVE_TIME
#define YOTTA_CFG_TCP_KEEPALIVE_TIME 300
#endif

#endif // M2MCONFIG_H
