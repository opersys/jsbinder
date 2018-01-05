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

#include <string>
#include <iostream>
#include <sstream>

#include <nan.h>

#include <sys/types.h>
#include <sys/mman.h>

#include <binder/IPCThreadState.h>
#include <binder/Binder.h>
#include <binder/BpBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>

#include "jsservice.hxx"
#include "jsparcel.hxx"

using namespace android;

JsService::JsService() {
    this->dumpArea = 0;
    this->dumpFd = -1;
}

JsService::~JsService() {
    if (this->dumpArea)
        munmap(this->dumpArea, this->dumpAreaSize);

    if (this->dumpFd > 0)
        close(this->dumpFd);

    this->dumpArea = 0;
    this->dumpFd = -1;
}

NAN_METHOD(JsService::New) {
    if (info.IsConstructCall()) {
        JsService* jsSv = new JsService();
        jsSv->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }
    else {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = {info[0]};
        v8::Local<v8::Function> cons = Nan::New(constructor());
        info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
}

// Send a parcel to a service, and returns the parcel that is returned as an answer.
NAN_METHOD(JsService::Transact) {
    v8::MaybeLocal<v8::Object> objR;
    v8::Local<v8::Function> cons;
    v8::Local<v8::Value> argv[0] = {};
    JsParcel *jsP, *jsR;
    JsService* jsSv;
    uint32_t code;

    // FIXME: More type checking on the type of object.
    if (!info[0]->IsNumber()) {
        Nan::ThrowTypeError("Argument 1 should be a number");
        return;
    }        
    if (!info[1]->IsObject()) {
        Nan::ThrowTypeError("Argument 2 should be a Parcel object");
        return;
    }

    code = Nan::To<uint32_t>(info[0]).FromJust();
    
    jsP = Nan::ObjectWrap::Unwrap<JsParcel>(info[1]->ToObject());
    jsSv = Nan::ObjectWrap::Unwrap<JsService>(info.Holder());
    
    // Construct a new JavaScript object that will receive the reply.
    cons = Nan::New<v8::Function>(JsParcel::constructor());
    objR = Nan::NewInstance(cons, 0, argv);
    jsR = Nan::ObjectWrap::Unwrap<JsParcel>(objR.ToLocalChecked());

    jsSv->sv->transact(code, jsP->parcel, &jsR->parcel);

    info.GetReturnValue().Set(objR.ToLocalChecked());
}

NAN_METHOD(JsService::Ping) {
    JsService *jsSv;

    jsSv = Nan::ObjectWrap::Unwrap<JsService>(info.Holder());
    assert(jsSv->sv != 0);

    jsSv->sv->pingBinder();
}

NAN_METHOD(JsService::GetInterface) {
    JsService *jsSv;
    Parcel p, r;
    status_t err;
    String16 iface;
    v8::MaybeLocal<v8::String> ret;

    jsSv = ObjectWrap::Unwrap<JsService>(info.Holder());
    assert(jsSv->sv != 0);

    err = jsSv->sv->transact(IBinder::INTERFACE_TRANSACTION, p, &r);
    iface = r.readString16();       
    
    if (err == NO_ERROR)
        ret = Nan::New<v8::String>(reinterpret_cast<const uint16_t *>(iface.string()));
    else
        ret = Nan::New<v8::String>("");

    info.GetReturnValue().Set(ret.ToLocalChecked());
}

NAN_METHOD(JsService::Dump) {
    JsService *jsSv;
    Vector<String16> dumpArgs;
    status_t err;
    std::stringstream errMsg;
    const char* fname = ".jsbinder-dumparea";
    v8::MaybeLocal<v8::String> ret;

    jsSv = Nan::ObjectWrap::Unwrap<JsService>(info.Holder());
    assert(jsSv->sv != 0);

    // There might be some arguments.
    if (info.Length() > 0) {
        for (int i = 0; i < info.Length(); i++) {
            if (!info[i]->IsString()) {
                errMsg << "Argument " << i + 1 << " should be a string";
                Nan::ThrowError(errMsg.str().c_str());
            }
            else dumpArgs.add(String16((const char16_t *)*v8::String::Value(info[i])));
        }
    }

    // Initialize the area where the 'dump' command will write its output.
    if (!jsSv->dumpArea) {
        // FIXME: This is clearly not safe to do. The name should be random.
        if ((jsSv->dumpFd = open(fname, O_RDWR | O_CREAT)) < 0) {
            errMsg << "Cannot open the file for dumping service: "
                   << strerror(errno);

            Nan::ThrowError(errMsg.str().c_str());
        }        
        
        unlink(fname);
        jsSv->dumpArea = mmap(NULL, jsSv->dumpAreaSize, PROT_READ | PROT_WRITE,
                              MAP_PRIVATE, jsSv->dumpFd, 0);
        
        if (!jsSv->dumpArea) {
            errMsg << "mmap on memory area for dump failed: "
                   << strerror(errno);
            
            close(jsSv->dumpFd);
            jsSv->dumpFd = 0;
            
            Nan::ThrowError(errMsg.str().c_str());
            return;
        }
    }
        
    // Sync to the start of the temporary file.
    lseek(jsSv->dumpFd, 0, SEEK_SET);
    err = jsSv->sv->dump(jsSv->dumpFd, dumpArgs);
    
    if (err == NO_ERROR) {
        // Get how much has been read.
        off_t pos = lseek(jsSv->dumpFd, 0, SEEK_CUR);
        ret = Nan::New<v8::String>(reinterpret_cast<char *>(jsSv->dumpArea), pos);
    } 
    else
        ret = Nan::New<v8::String>("Dump failed");

    info.GetReturnValue().Set(ret.ToLocalChecked());
}

NAN_MODULE_INIT(JsService::Init) {
    v8::MaybeLocal<v8::FunctionTemplate> mbTmpl = Nan::New<v8::FunctionTemplate>(New);
    v8::Local<v8::ObjectTemplate> proto;
    v8::Local<v8::FunctionTemplate> tmpl;

    tmpl = mbTmpl.ToLocalChecked();

    proto = tmpl->PrototypeTemplate();

    tmpl->SetClassName(Nan::New("JsService").ToLocalChecked());
    tmpl->InstanceTemplate()->SetInternalFieldCount(4);

    Nan::SetPrototypeMethod(tmpl, "ping", Ping);
    Nan::SetPrototypeMethod(tmpl, "transact", Transact);
    Nan::SetPrototypeMethod(tmpl, "getInterface", GetInterface);
    Nan::SetPrototypeMethod(tmpl, "dump", Dump);

    constructor().Reset(Nan::GetFunction(tmpl).ToLocalChecked());
    Nan::Set(target,
             Nan::New("JsService").ToLocalChecked(),
             Nan::GetFunction(tmpl).ToLocalChecked());
}
