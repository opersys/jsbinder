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

#include <node.h>

#include <binder/IPCThreadState.h>
#include <binder/Binder.h>
#include <binder/BpBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <utils/String16.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <private/binder/binder_module.h>

using namespace android;

class JsParcel : public node::ObjectWrap {
  
public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Persistent<v8::Function> constructor;
  
  Parcel parcel;
  
private:
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> ReadInt32(const v8::Arguments& args);
  static v8::Handle<v8::Value> ReadInt64(const v8::Arguments& args);
  static v8::Handle<v8::Value> WriteInt32(const v8::Arguments& args);
  static v8::Handle<v8::Value> WriteInt64(const v8::Arguments& args);
  static v8::Handle<v8::Value> ReadString(const v8::Arguments& args);
  static v8::Handle<v8::Value> WriteString(const v8::Arguments& args);
  static v8::Handle<v8::Value> ReadString8(const v8::Arguments& args);
  static v8::Handle<v8::Value> WriteString8(const v8::Arguments& args);
  static v8::Handle<v8::Value> WriteInterfaceToken(const v8::Arguments& args);
  static v8::Handle<v8::Value> ReadExceptionCode(const v8::Arguments& args);
  
  static v8::Handle<v8::Value> SetDataPosition(const v8::Arguments& args);
  static v8::Handle<v8::Value> GetDataPosition(const v8::Arguments& args);
  static v8::Handle<v8::Value> GetDataSize(const v8::Arguments& args);
  
  explicit JsParcel();
  ~JsParcel();
};

class JsServiceManager : public node::ObjectWrap {
  
public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Persistent<v8::Function> constructor;
  
private:
  static v8::Handle<v8::Value> List(const v8::Arguments& args);
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> GetService(const v8::Arguments& args);
  
  sp<IServiceManager> sm;
  
  explicit JsServiceManager();
  ~JsServiceManager();
};

class JsService : public node::ObjectWrap {
public:
  static void Init(v8::Handle<v8::Object> exports);
  static v8::Persistent<v8::Function> constructor;
  
  void setService(sp<IBinder> sv) {
    assert(sv != 0);
    this->sv = sv;
  }
  
private:
  static v8::Handle<v8::Value> New(const v8::Arguments& args);
  static v8::Handle<v8::Value> Ping(const v8::Arguments& args);
  static v8::Handle<v8::Value> Transact(const v8::Arguments& args);
  static v8::Handle<v8::Value> GetInterface(const v8::Arguments& args);
  
  sp<IBinder> sv;
  
  explicit JsService();
  ~JsService();
};

v8::Persistent<v8::Function> JsServiceManager::constructor;
v8::Persistent<v8::Function> JsService::constructor;
v8::Persistent<v8::Function> JsParcel::constructor;

JsServiceManager::JsServiceManager() {
  this->sm = defaultServiceManager();
}

JsServiceManager::~JsServiceManager() {
}

JsService::JsService() {
}

JsService::~JsService() {
}

JsParcel::JsParcel() : parcel() {
}

JsParcel::~JsParcel() {
}

void JsServiceManager::Init(v8::Handle<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
  
  tpl->SetClassName(v8::String::NewSymbol("JsServiceManager"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("list"),
  v8::FunctionTemplate::New(List)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getService"),
  v8::FunctionTemplate::New(GetService)->GetFunction());
  
  constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
  exports->Set(v8::String::NewSymbol("JsServiceManager"), constructor);
}

void JsService::Init(v8::Handle<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
  v8::Local<v8::ObjectTemplate> proto;
  
  proto = tpl->PrototypeTemplate();
  
  tpl->SetClassName(v8::String::NewSymbol("JsService"));
  tpl->InstanceTemplate()->SetInternalFieldCount(2);
  
  proto->Set(v8::String::NewSymbol("ping"), 
  v8::FunctionTemplate::New(Ping)->GetFunction());
  proto->Set(v8::String::NewSymbol("transact"), 
  v8::FunctionTemplate::New(Transact)->GetFunction());
  proto->Set(v8::String::NewSymbol("getInterface"), 
  v8::FunctionTemplate::New(GetInterface)->GetFunction());
  
  constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
  exports->Set(v8::String::NewSymbol("JsService"), constructor);
}

void JsParcel::Init(v8::Handle<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
  v8::Local<v8::ObjectTemplate> proto;
  
  proto = tpl->PrototypeTemplate();
  
  tpl->SetClassName(v8::String::NewSymbol("JsParcel"));
  tpl->InstanceTemplate()->SetInternalFieldCount(8);
  
  proto->Set(v8::String::NewSymbol("writeInt32"), 
    v8::FunctionTemplate::New(WriteInt32)->GetFunction());
  proto->Set(v8::String::NewSymbol("readInt32"), 
    v8::FunctionTemplate::New(ReadInt32)->GetFunction());
  
  proto->Set(v8::String::NewSymbol("writeInt64"), 
    v8::FunctionTemplate::New(WriteInt64)->GetFunction());
  proto->Set(v8::String::NewSymbol("readInt64"), 
    v8::FunctionTemplate::New(ReadInt64)->GetFunction());
  
  proto->Set(v8::String::NewSymbol("writeString"), 
    v8::FunctionTemplate::New(WriteString)->GetFunction());
  proto->Set(v8::String::NewSymbol("readString"), 
    v8::FunctionTemplate::New(ReadString)->GetFunction());
  
  proto->Set(v8::String::NewSymbol("writeString8"), 
    v8::FunctionTemplate::New(WriteString8)->GetFunction());
  proto->Set(v8::String::NewSymbol("readString8"), 
    v8::FunctionTemplate::New(ReadString8)->GetFunction());
  
  proto->Set(v8::String::NewSymbol("setDataPosition"), 
    v8::FunctionTemplate::New(SetDataPosition)->GetFunction());
  proto->Set(v8::String::NewSymbol("dataPosition"), 
    v8::FunctionTemplate::New(GetDataPosition)->GetFunction());
  proto->Set(v8::String::NewSymbol("dataSize"), 
    v8::FunctionTemplate::New(GetDataSize)->GetFunction());
  
  proto->Set(v8::String::NewSymbol("writeInterfaceToken"), 
    v8::FunctionTemplate::New(WriteInterfaceToken)->GetFunction());
  proto->Set(v8::String::NewSymbol("readExceptionCode"), 
    v8::FunctionTemplate::New(ReadExceptionCode)->GetFunction());
  
  constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
  exports->Set(v8::String::NewSymbol("JsParcel"), constructor);
}

v8::Handle<v8::Value> JsServiceManager::New(const v8::Arguments& args) {
  if (args.IsConstructCall()) {
    JsServiceManager* jsSm = new JsServiceManager();
    jsSm->Wrap(args.This());
    return args.This();
  }
  else return v8::ThrowException(
    v8::Exception::TypeError(
      v8::String::New("Constructor not to be called directly")));
}
    
v8::Handle<v8::Value> JsService::New(const v8::Arguments& args) {
  if (args.IsConstructCall()) {
    JsService* jsSv = new JsService();
    jsSv->Wrap(args.This());
    return args.This();
  }
  else return v8::ThrowException(
    v8::Exception::TypeError(
      v8::String::New("Constructor not to be called directly")));
}

v8::Handle<v8::Value> JsParcel::New(const v8::Arguments& args) {
  if (args.IsConstructCall()) {
    JsParcel *jsP = new JsParcel();
    jsP->Wrap(args.This());
    return args.This();
  }
  else return v8::ThrowException(
    v8::Exception::TypeError(
      v8::String::New("Constructor not to be called directly")));
}
    
v8::Handle<v8::Value> JsServiceManager::List(const v8::Arguments& args) {
  v8::Handle<v8::Array> jsServices;
  v8::HandleScope scope;
  Vector<String16> services;
  sp<IServiceManager> sm;
  JsServiceManager *jsSm;
  
  jsSm = ObjectWrap::Unwrap<JsServiceManager>(args.This());
  services = jsSm->sm->listServices();
  jsServices = v8::Array::New(services.size());
  
  for (size_t i = 0; i < services.size(); i++) {
    String16 name = services[i];
    jsServices->Set(i, v8::String::New((uint16_t *)name.string()));
  }
  
  return scope.Close(jsServices);
}
    
v8::Handle<v8::Value> JsServiceManager::GetService(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsServiceManager *jsSm;
  JsService *jsSv;
  sp<IBinder> sv;
  v8::Local<v8::Object> svObj;
      
  if (!args[0]->IsString())
    return v8::ThrowException(
      v8::Exception::TypeError(
        v8::String::New("Expected: service name")));
      
  // Unwrap the C++ object from inside the V8 object.
  jsSm = ObjectWrap::Unwrap<JsServiceManager>(args.This());
  sv = jsSm->sm->getService(String16(*v8::String::Utf8Value(args[0])));
      
  if (sv != 0) {
    // Calls the JsService JavaScript constructor
    svObj = JsService::constructor->NewInstance();
    jsSv = ObjectWrap::Unwrap<JsService>(svObj);
        
    // Set the service to the object we just built.
    jsSv->setService(sv);
        
    return scope.Close(svObj);
  }
      
  return v8::ThrowException(
    v8::Exception::TypeError(
      v8::String::New("Could not find service")));
}
    
// Send a parcel to a service, and returns the parcel that is returned as an answer.
v8::Handle<v8::Value> JsService::Transact(const v8::Arguments& args) {
  v8::HandleScope scope;
  v8::Local<v8::Object> objR;
  JsParcel *jsP, *jsR;
  JsService* jsSv;
  uint32_t code;
      
  // FIXME: More type checking on the type of object.
  if (!args[0]->IsNumber())
    return v8::ThrowException(
      v8::Exception::TypeError(
        v8::String::New("Argument 1 should be a number")));
  if (!args[1]->IsObject())
      return v8::ThrowException(
        v8::Exception::TypeError(
          v8::String::New("Argument 2 should be a Parcel object")));
      
  code = args[0]->ToUint32()->Value();
      
  jsP = node::ObjectWrap::Unwrap<JsParcel>(args[1]->ToObject());
  jsSv = node::ObjectWrap::Unwrap<JsService>(args.This());
      
  // Construct a new JavaScript object that will receive the reply.
  objR = JsParcel::constructor->NewInstance();
  jsR = node::ObjectWrap::Unwrap<JsParcel>(objR);
      
  jsSv->sv->transact(code, jsP->parcel, &jsR->parcel);
      
  return scope.Close(objR);
}
    
v8::Handle<v8::Value> JsService::Ping(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsService *jsSv;
      
  jsSv = ObjectWrap::Unwrap<JsService>(args.This());
  assert(jsSv->sv != 0);
      
  jsSv->sv->pingBinder();
      
  return scope.Close(v8::Null());
}
    
v8::Handle<v8::Value> JsService::GetInterface(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsService *jsSv;
  Parcel p, r;
  status_t err;
  String16 iface;
      
  jsSv = node::ObjectWrap::Unwrap<JsService>(args.This());
  assert(jsSv->sv != 0);
      
  err = jsSv->sv->transact(IBinder::INTERFACE_TRANSACTION, p, &r);
  iface = r.readString16();
      
  if (err == NO_ERROR)
    return scope.Close(v8::String::New((uint16_t *)iface.string()));
  else
    return scope.Close(v8::String::New(""));
}
    
v8::Handle<v8::Value> JsParcel::WriteString(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  String16 val;
      
  if (!args[0]->IsString())
    return v8::ThrowException(
      v8::Exception::TypeError(
        v8::String::New("Argument 1 should be a string")));
      
  val = String16((const char16_t *)*v8::String::Value(args[0]));
      
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  jsP->parcel.writeString16(val);
      
  return scope.Close(v8::Null());
}
    
v8::Handle<v8::Value> JsParcel::ReadString(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  String16 val;
  
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  val = jsP->parcel.readString16();
  
  return scope.Close(v8::String::New((uint16_t *)val.string()));
}

v8::Handle<v8::Value> JsParcel::WriteString8(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  String8 val;
  
  if (!args[0]->IsString())
    return v8::ThrowException(
      v8::Exception::TypeError(
        v8::String::New("Argument 1 should be a string")));
  
  val = String8(*(v8::String::Utf8Value(args[0])));
  
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  jsP->parcel.writeString8(val);
  
  return scope.Close(v8::Null());
}

v8::Handle<v8::Value> JsParcel::ReadString8(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  String8 val;
  
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  val = jsP->parcel.readString8();
  
  return scope.Close(v8::String::New((uint16_t *)val.string()));
}

v8::Handle<v8::Value> JsParcel::WriteInterfaceToken(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  String16 val;
  
  if (!args[0]->IsString())
    return v8::ThrowException(
      v8::Exception::TypeError(
        v8::String::New("Argument 1 should be a string")));
  
  val = String16(*(v8::String::Utf8Value(args[0])));
  
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  jsP->parcel.writeInterfaceToken(val);
  
  return scope.Close(v8::Null());
}

v8::Handle<v8::Value> JsParcel::ReadExceptionCode(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  int32_t ex;
  
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  ex = jsP->parcel.readExceptionCode();
  
  return scope.Close(v8::Number::New(ex));
}

v8::Handle<v8::Value> JsParcel::WriteInt32(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  uint32_t val;
  
  if (!args[0]->IsNumber())
    return v8::ThrowException(
      v8::Exception::TypeError(v8::String::New("Argument 1 should be a number")));
  
  val = args[0]->ToUint32()->Value();
  
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  jsP->parcel.writeInt32(val);
  
  return scope.Close(v8::Null());
}

v8::Handle<v8::Value> JsParcel::ReadInt32(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  int32_t r;
  
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  r = jsP->parcel.readInt32();
  
  return scope.Close(v8::Number::New(r));
}

v8::Handle<v8::Value> JsParcel::WriteInt64(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  int64_t val;
  
  if (!args[0]->IsNumber())
    return v8::ThrowException(
      v8::Exception::TypeError(
        v8::String::New("Argument 1 should be a number")));
  
  val = args[0]->ToNumber()->Value();
  
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  jsP->parcel.writeInt64(val);
  
  return scope.Close(v8::Null());
}

v8::Handle<v8::Value> JsParcel::ReadInt64(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  int64_t r;
  
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  r = jsP->parcel.readInt64();
  
  return scope.Close(v8::Number::New(r));
}

v8::Handle<v8::Value> JsParcel::SetDataPosition(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  uint32_t val;
  
  if (!args[0]->IsNumber())
    return v8::ThrowException(
      v8::Exception::TypeError(
        v8::String::New("Argument 1 should be a number")));
  
  val = args[0]->ToUint32()->Value();
  
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  jsP->parcel.setDataPosition(val);
  
  return scope.Close(v8::Null());
}

v8::Handle<v8::Value> JsParcel::GetDataPosition(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  size_t pos;
  
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  pos = jsP->parcel.dataPosition();
  
  return scope.Close(v8::Number::New(pos));
}

v8::Handle<v8::Value> JsParcel::GetDataSize(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsParcel *jsP;
  size_t sz;
  
  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  sz = jsP->parcel.dataSize();
  
  return scope.Close(v8::Number::New(sz));
}

void Init(v8::Handle<v8::Object> exports) {
  JsServiceManager::Init(exports);
  JsService::Init(exports);
  JsParcel::Init(exports);
}

#ifdef BINDER_IPC_32BIT
NODE_MODULE(jsbinder_binder32, Init);
#else
NODE_MODULE(jsbinder_binder64, Init);
#endif
