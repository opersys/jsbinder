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
 
#ifndef _JSPARCEL_HXX
#define _JSPARCEL_HXX

#include <nan.h>

using namespace android;

class JsParcel : public Nan::ObjectWrap {

public:
    Parcel parcel;
    static NAN_MODULE_INIT(Init);
    
private:
    friend class JsService;
    
    static inline Nan::Persistent<v8::Function> & constructor() {
        static Nan::Persistent<v8::Function> jsParcelConstructor;
        return jsParcelConstructor;
    }

    static NAN_METHOD(Init);
    static NAN_METHOD(New);
    static NAN_METHOD(ReadInt32);
    static NAN_METHOD(ReadInt64);
    static NAN_METHOD(WriteInt32);
    static NAN_METHOD(WriteInt64);
    static NAN_METHOD(ReadString);
    static NAN_METHOD(WriteString);
    static NAN_METHOD(ReadString8);
    static NAN_METHOD(WriteString8);
    static NAN_METHOD(ReadExceptionCode);
    static NAN_METHOD(WriteInterfaceToken);
    
    static NAN_METHOD(SetDataPosition);
    static NAN_METHOD(GetDataPosition);
    static NAN_METHOD(GetDataSize);

    explicit JsParcel();
    ~JsParcel();
};

#endif // _JSPARCEL_HXX
