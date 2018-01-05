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

#include <nan.h>

#include <binder/IPCThreadState.h>
#include <binder/Binder.h>
#include <binder/BpBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>

#include "jsparcel.hxx"

using namespace android;

JsParcel::JsParcel() : parcel() {
}

JsParcel::~JsParcel() {
}

NAN_MODULE_INIT(JsParcel::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    v8::Local<v8::ObjectTemplate> proto;

    proto = tpl->PrototypeTemplate();

    tpl->SetClassName(Nan::New("JsParcel").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(14);

    Nan::SetPrototypeMethod(tpl, "writeInt32", WriteInt32);
    Nan::SetPrototypeMethod(tpl, "readInt32", ReadInt32);
    Nan::SetPrototypeMethod(tpl, "writeInt64", WriteInt64);
    Nan::SetPrototypeMethod(tpl, "readInt64", ReadInt64);
    Nan::SetPrototypeMethod(tpl, "writeString", WriteString);
    Nan::SetPrototypeMethod(tpl, "readString", ReadString);
    Nan::SetPrototypeMethod(tpl, "writeString8", WriteString8);
    Nan::SetPrototypeMethod(tpl, "readString8", ReadString8);
    Nan::SetPrototypeMethod(tpl, "setDataPosition", SetDataPosition);
    Nan::SetPrototypeMethod(tpl, "dataPosition", GetDataPosition);
    Nan::SetPrototypeMethod(tpl, "dataSize", GetDataSize);
    Nan::SetPrototypeMethod(tpl, "writeInterfaceToken", WriteInterfaceToken);
    Nan::SetPrototypeMethod(tpl, "readExceptionCode", ReadExceptionCode);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target,
             Nan::New("JsParcel").ToLocalChecked(),
             Nan::GetFunction(tpl).ToLocalChecked());
}

NAN_METHOD(JsParcel::New) {
    if (info.IsConstructCall()) {
        JsParcel *jsP = new JsParcel();
        jsP->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }
    else {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = {info[0]};
        v8::Local<v8::Function> cons = Nan::New(constructor());
        info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
}

NAN_METHOD(JsParcel::WriteString) {
    JsParcel *jsP;
    String16 val;

    if (!info[0]->IsString()) 
        return Nan::ThrowTypeError("Argument 1 should be a string");

    else {
        val = String16((const char16_t *)*v8::String::Value(info[0]));

        jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
        jsP->parcel.writeString16(val);
    }
}

NAN_METHOD(JsParcel::ReadString) {
    JsParcel *jsP;
    String16 val;
    v8::Local<v8::String> ret;

    jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
    val = jsP->parcel.readString16();
    ret = Nan::New<v8::String>(reinterpret_cast<const uint16_t *>(val.string())).ToLocalChecked();

    info.GetReturnValue().Set(ret);
}

NAN_METHOD(JsParcel::WriteString8) {
    JsParcel *jsP;
    String8 val;

    if (!info[0]->IsString())
        Nan::ThrowTypeError("Argument 1 should be a string");
    
    else {
        val = String8(*(v8::String::Utf8Value(info[0])));

        jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
        jsP->parcel.writeString8(val);
    }
}

NAN_METHOD(JsParcel::ReadString8) {
    JsParcel *jsP;
    String8 val;

    jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
    val = jsP->parcel.readString8();

    info.GetReturnValue().Set(Nan::New<v8::String>(val.string()).ToLocalChecked());
}

NAN_METHOD(JsParcel::WriteInterfaceToken) {
    JsParcel *jsP;
    String16 val;

    if (!info[0]->IsString())
        Nan::ThrowTypeError("Argument 1 should be a string");

    else {
        val = String16(*(v8::String::Utf8Value(info[0])));

        jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
        jsP->parcel.writeInterfaceToken(val);
    }
}

NAN_METHOD(JsParcel::ReadExceptionCode) {
    JsParcel *jsP;
    int32_t ex;

    jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
    ex = jsP->parcel.readExceptionCode();

    info.GetReturnValue().Set(Nan::New(ex));
}

NAN_METHOD(JsParcel::WriteInt32) {
    JsParcel *jsP;
    uint32_t val;

    if (!info[0]->IsNumber())
        Nan::ThrowTypeError("Argument 1 should be a number");

    else {
        val = info[0]->ToUint32()->Value();

        jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
        jsP->parcel.writeInt32(val);
    }
}

NAN_METHOD(JsParcel::ReadInt32) {
    JsParcel *jsP;
    int32_t r;

    jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
    r = jsP->parcel.readInt32();

    info.GetReturnValue().Set(Nan::New(r));
}

NAN_METHOD(JsParcel::WriteInt64) {
    JsParcel *jsP;
    int64_t val;

    if (!info[0]->IsNumber())
        Nan::ThrowTypeError("Argument 1 should be a number");

    else {
        val = info[0]->ToNumber()->Value();

        jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
        jsP->parcel.writeInt64(val);
    }
}

NAN_METHOD(JsParcel::ReadInt64) {
    JsParcel *jsP;
    int64_t r;

    jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
    r = jsP->parcel.readInt64();

    info.GetReturnValue().Set(Nan::New<v8::Number>(r));
}

NAN_METHOD(JsParcel::SetDataPosition) {
    JsParcel *jsP;
    uint32_t val;

    if (!info[0]->IsNumber())
        Nan::ThrowTypeError("Argument 1 should be a number");

    else {
        val = info[0]->ToUint32()->Value();
  
        jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
        jsP->parcel.setDataPosition(val);
    }
}

NAN_METHOD(JsParcel::GetDataPosition) {
    JsParcel *jsP;
    size_t pos;

    jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
    pos = jsP->parcel.dataPosition();

    info.GetReturnValue().Set(Nan::New<v8::Number>(pos));
}

NAN_METHOD(JsParcel::GetDataSize) {
    JsParcel *jsP;
    size_t sz;

    jsP = ObjectWrap::Unwrap<JsParcel>(info.Holder());
    sz = jsP->parcel.dataSize();

    info.GetReturnValue().Set(Nan::New(static_cast<uint32_t>(sz)));
}
