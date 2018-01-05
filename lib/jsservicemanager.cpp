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

#include <iostream>

#include "jsservicemanager.hxx"
#include "jsservice.hxx"

JsServiceManager::JsServiceManager() {
    this->sm = defaultServiceManager();
}

JsServiceManager::~JsServiceManager() {}

NAN_MODULE_INIT(JsServiceManager::Init) {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);

    tpl->SetClassName(Nan::New("JsServiceManager").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(2);
    
    Nan::SetPrototypeMethod(tpl, "list", List);
    Nan::SetPrototypeMethod(tpl, "getService", GetService);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target,
             Nan::New("JsServiceManager").ToLocalChecked(),
             Nan::GetFunction(tpl).ToLocalChecked());    
}

NAN_METHOD(JsServiceManager::New) {
    if (info.IsConstructCall()) {
        JsServiceManager* jsSm = new JsServiceManager();
        jsSm->Wrap(info.This());
        info.GetReturnValue().Set(info.This());
    }
    else {
        const int argc = 1;
        v8::Local<v8::Value> argv[argc] = {info[0]};
        v8::Local<v8::Function> cons = Nan::New(constructor());
        info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
}

NAN_METHOD(JsServiceManager::List) {
    v8::Handle<v8::Array> jsServices;
    Vector<String16> services;
    sp<IServiceManager> sm;
    JsServiceManager *jsSm;

    jsSm = ObjectWrap::Unwrap<JsServiceManager>(info.This());
    services = jsSm->sm->listServices();
    jsServices = Nan::New<v8::Array>(services.size());

    for (size_t i = 0; i < services.size(); i++) {
        String16 name = services[i];
        Nan::Set(jsServices, i, Nan::New((uint16_t *)name.string()).ToLocalChecked());
    }

    info.GetReturnValue().Set(jsServices);
}

NAN_METHOD(JsServiceManager::GetService) {
    JsServiceManager *jsSm;
    JsService *jsSv;
    sp<IBinder> sv;
    v8::MaybeLocal<v8::Object> svObj;
    v8::Local<v8::Value> argv[0] = {};
    v8::Local<v8::Function> cons;

    if (!info[0]->IsString())
        Nan::ThrowError("Expected: service name");

    //std::cout << v8::String::Utf8Value(info[0])) << std::endl;
    
    // Unwrap the C++ object from inside the V8 object.
    jsSm = ObjectWrap::Unwrap<JsServiceManager>(info.This());
    sv = jsSm->sm->getService(String16(*v8::String::Utf8Value(info[0])));
  
    if (sv != 0) {
        // Calls the JsService JavaScript constructor
        cons = Nan::New<v8::Function>(JsService::constructor());
        svObj = Nan::NewInstance(cons, 0, argv);
        jsSv = Nan::ObjectWrap::Unwrap<JsService>(svObj.ToLocalChecked());

        // Set the service to the object we just built.
        jsSv->setService(sv);
        info.GetReturnValue().Set(svObj.ToLocalChecked());
    }
    else Nan::ThrowError("Could not find service");
}
