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
 
#ifndef _JSSERVICE_HXX
#define _JSSERVICE_HXX

#include <nan.h>
#include <binder/Parcel.h>

using namespace android;

class JsService : public Nan::ObjectWrap {
public:
    static NAN_MODULE_INIT(Init);
    
    void setService(sp<IBinder> sv) {
        assert(sv != 0);
        this->sv = sv;
    }
    
private:
    friend class JsServiceManager;
    
    static Nan::Persistent<v8::Function> & constructor() {
        static Nan::Persistent<v8::Function> jsServiceConstructor;
        return jsServiceConstructor;
    }

    static NAN_METHOD(New);
    static NAN_METHOD(Ping);
    static NAN_METHOD(Transact);
    static NAN_METHOD(GetInterface);
    static NAN_METHOD(Dump);
    
    // The memory that is reserved for dumpMem calls on the service.
    const int dumpAreaSize = 4096000;
    void *dumpArea;
    int dumpFd;

    sp<IBinder> sv;

    explicit JsService();
    ~JsService();
};

#endif // _JSSERVICE_HXX
