/*
 * Copyright (C) 2015,2017 Opersys inc.
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

#ifndef _JSSERVICEMANAGER_HXX
#define _JSSERVICEMANAGER_HXX 

#include <nan.h>

#include <binder/IPCThreadState.h>
#include <binder/Binder.h>
#include <binder/BpBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>

using namespace android;

class JsServiceManager : public node::ObjectWrap {

public:
    static NAN_MODULE_INIT(Init);
    
private:

    static Nan::Persistent<v8::Function> & constructor() {
        static Nan::Persistent<v8::Function> jsServiceManagerConstructor;
        return jsServiceManagerConstructor;
    }

    static NAN_METHOD(List);
    static NAN_METHOD(New);
    static NAN_METHOD(GetService);
    
    sp<IServiceManager> sm;
    
    explicit JsServiceManager();
    ~JsServiceManager();
};

#endif // _JSSERVICEMANAGER_HXX
