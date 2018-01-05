/*
 * Copyright (C) 2015 Opersys inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <nan.h>

#include "jsservicemanager.hxx"
#include "jsservice.hxx"
#include "jsparcel.hxx"

using namespace android;

NAN_MODULE_INIT(InitAll) {
  JsServiceManager::Init(target);
  JsService::Init(target);
  JsParcel::Init(target);
}

#ifdef BINDER_IPC_32BIT
NODE_MODULE(jsbinder_binder32, InitAll);
#else
NODE_MODULE(jsbinder_binder64, InitAll);
#endif
