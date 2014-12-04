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

using namespace v8;
using namespace android;

class JsParcel : public node::ObjectWrap {

  public: 
    static void Init(Handle<Object> exports);
    static Persistent<Function> constructor;

    Parcel parcel;

  private:
    static Handle<Value> New(const Arguments& args);
    static Handle<Value> ReadInt32(const Arguments& args);
    static Handle<Value> WriteInt32(const Arguments& args);
    static Handle<Value> ReadString(const Arguments& args);
    static Handle<Value> WriteString(const Arguments& args);
    static Handle<Value> WriteInterfaceToken(const Arguments& args);
    static Handle<Value> ReadExceptionCode(const Arguments& args);

    static Handle<Value> SetDataPosition(const Arguments& args);
    static Handle<Value> GetDataPosition(const Arguments& args);
    static Handle<Value> GetDataSize(const Arguments& args);

    explicit JsParcel();
    ~JsParcel();
};

class JsServiceManager : public node::ObjectWrap {

  public:
    static void Init(Handle<Object> exports);
    static Persistent<Function> constructor;

  private:
    static Handle<Value> List(const Arguments& args);
    static Handle<Value> New(const Arguments& args);
    static Handle<Value> GetService(const Arguments& args);

    sp<IServiceManager> sm;

    explicit JsServiceManager();
    ~JsServiceManager();
};

class JsService : public node::ObjectWrap {
  public:
    static void Init(Handle<Object> exports);
    static Persistent<Function> constructor;

    void setService(sp<IBinder> sv) {
      assert(sv != 0);
      this->sv = sv;
    }

  private:
    static Handle<Value> New(const Arguments& args);
    static Handle<Value> Ping(const Arguments& args);
    static Handle<Value> Transact(const Arguments& args);    
    static Handle<Value> GetInterface(const Arguments& args);

    sp<IBinder> sv;

    explicit JsService();
    ~JsService();
};

Persistent<Function> JsServiceManager::constructor;
Persistent<Function> JsService::constructor;
Persistent<Function> JsParcel::constructor;

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

void JsServiceManager::Init(Handle<Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);

  tpl->SetClassName(String::NewSymbol("JsServiceManager"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  tpl->PrototypeTemplate()->Set(String::NewSymbol("list"), 
    FunctionTemplate::New(List)->GetFunction());
  tpl->PrototypeTemplate()->Set(String::NewSymbol("getService"), 
    FunctionTemplate::New(GetService)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("JsServiceManager"), constructor);  
}

void JsService::Init(Handle<Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  Local<ObjectTemplate> proto;

  proto = tpl->PrototypeTemplate();

  tpl->SetClassName(String::NewSymbol("JsService"));
  tpl->InstanceTemplate()->SetInternalFieldCount(2);

  proto->Set(String::NewSymbol("ping"), FunctionTemplate::New(Ping)->GetFunction());
  proto->Set(String::NewSymbol("transact"), FunctionTemplate::New(Transact)->GetFunction());
  proto->Set(String::NewSymbol("getInterface"), FunctionTemplate::New(GetInterface)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("JsService"), constructor);
}

void JsParcel::Init(Handle<Object> exports) {
  Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
  Local<ObjectTemplate> proto;

  proto = tpl->PrototypeTemplate();

  tpl->SetClassName(String::NewSymbol("JsParcel"));
  tpl->InstanceTemplate()->SetInternalFieldCount(8);

  proto->Set(String::NewSymbol("writeInt32"), FunctionTemplate::New(WriteInt32)->GetFunction());
  proto->Set(String::NewSymbol("readInt32"), FunctionTemplate::New(ReadInt32)->GetFunction());

  proto->Set(String::NewSymbol("writeString"), FunctionTemplate::New(WriteString)->GetFunction());
  proto->Set(String::NewSymbol("readString"), FunctionTemplate::New(ReadString)->GetFunction());

  proto->Set(String::NewSymbol("setDataPosition"), FunctionTemplate::New(SetDataPosition)->GetFunction());
  proto->Set(String::NewSymbol("dataPosition"), FunctionTemplate::New(GetDataPosition)->GetFunction());
  proto->Set(String::NewSymbol("dataSize"), FunctionTemplate::New(GetDataSize)->GetFunction());

  proto->Set(String::NewSymbol("writeInterfaceToken"), FunctionTemplate::New(WriteInterfaceToken)->GetFunction());
  proto->Set(String::NewSymbol("readExceptionCode"), FunctionTemplate::New(ReadExceptionCode)->GetFunction());

  constructor = Persistent<Function>::New(tpl->GetFunction());
  exports->Set(String::NewSymbol("JsParcel"), constructor);
}

Handle<Value> JsServiceManager::New(const Arguments& args) {
  HandleScope scope;

  if (args.IsConstructCall()) {
    JsServiceManager* jsSm = new JsServiceManager();
    jsSm->Wrap(args.This());
    return args.This();
  }
  else return ThrowException(Exception::TypeError(String::New("Constructor not to be called directly")));
}

Handle<Value> JsService::New(const Arguments& args) {
  if (args.IsConstructCall()) {
    JsService* jsSv = new JsService();
    jsSv->Wrap(args.This());
    return args.This();
  } 
  else return ThrowException(Exception::TypeError(String::New("Constructor not to be called directly")));
}

Handle<Value> JsParcel::New(const Arguments& args) {
  if (args.IsConstructCall()) {
    JsParcel *jsP = new JsParcel();
    jsP->Wrap(args.This());
    return args.This();
  }
  else return ThrowException(Exception::TypeError(String::New("Constructor not to be called directly")));
}

Handle<Value> JsServiceManager::List(const Arguments& args) 
{
  Handle<Array> jsServices;
  HandleScope scope;
  Vector<String16> services;
  sp<IServiceManager> sm;
  JsServiceManager *jsSm;

  jsSm = ObjectWrap::Unwrap<JsServiceManager>(args.This());
  services = jsSm->sm->listServices();  
  jsServices = Array::New(services.size());

  for (size_t i = 0; i < services.size(); i++) {
    String16 name = services[i];
    jsServices->Set(i, String::New(name));
  }

  return scope.Close(jsServices);
}

Handle<Value> JsServiceManager::GetService(const Arguments& args) {
  HandleScope scope;
  JsServiceManager *jsSm;
  JsService *jsSv;
  sp<IBinder> sv;
  Local<Object> svObj;

  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("Expected: service name")));

  // Unwrap the C++ object from inside the V8 object.
  jsSm = ObjectWrap::Unwrap<JsServiceManager>(args.This());  
  sv = jsSm->sm->getService(String16(*String::Utf8Value(args[0])));

  if (sv != 0) {
    // Calls the JsService JavaScript constructor
    svObj = JsService::constructor->NewInstance();
    jsSv = ObjectWrap::Unwrap<JsService>(svObj);

    // Set the service to the object we just built.
    jsSv->setService(sv); 

    return scope.Close(svObj);
  }

  return ThrowException(Exception::TypeError(String::New("Could not find service")));
}

// Send a parcel to a service, and returns the parcel that is returned as an answer.
Handle<Value> JsService::Transact(const Arguments& args) {
  HandleScope scope;
  Local<Object> objR;
  JsParcel *jsP, *jsR;
  JsService* jsSv;
  uint32_t code;

  // FIXME: More type checking on the type of object.

  if (!args[0]->IsNumber())
    return ThrowException(Exception::TypeError(String::New("Argument 1 should be a number")));
  if (!args[1]->IsObject())
    return ThrowException(Exception::TypeError(String::New("Argument 2 should be a Parcel object")));

  code = args[0]->ToUint32()->Value();
  
  jsP = node::ObjectWrap::Unwrap<JsParcel>(args[1]->ToObject());
  jsSv = node::ObjectWrap::Unwrap<JsService>(args.This());

  // Construct a new JavaScript object that will receive the reply.
  objR = JsParcel::constructor->NewInstance();
  jsR = node::ObjectWrap::Unwrap<JsParcel>(objR);

  jsSv->sv->transact(code, jsP->parcel, &jsR->parcel);

  return scope.Close(objR);
}

Handle<Value> JsService::Ping(const Arguments& args) {
  HandleScope scope;
  JsService *jsSv;

  jsSv = ObjectWrap::Unwrap<JsService>(args.This());
  assert(jsSv->sv != 0);

  jsSv->sv->pingBinder();

  return scope.Close(Null());
}

Handle<Value> JsService::GetInterface(const Arguments& args) {
  HandleScope scope;
  JsService *jsSv;
  Parcel p, r;
  status_t err;
  String16 iface;

  jsSv = node::ObjectWrap::Unwrap<JsService>(args.This());
  assert(jsSv->sv != 0);

  err = jsSv->sv->transact(IBinder::INTERFACE_TRANSACTION, p, &r);
  iface = r.readString16();

  if (err == NO_ERROR)
    return scope.Close(String::New(iface.string()));
  else
    return scope.Close(String::New(""));
}

Handle<Value> JsParcel::WriteString(const Arguments& args) {
  HandleScope scope;
  JsParcel *jsP;
  String8 val;

  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("Argument 1 should be a string")));

  val = String8(*(String::Utf8Value(args[0])));

  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  jsP->parcel.writeString8(val);

  return scope.Close(Null());
}

Handle<Value> JsParcel::ReadString(const Arguments& args) {
  HandleScope scope;
  JsParcel *jsP;
  String8 val;

  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  val = jsP->parcel.readString8();

  return scope.Close(String::New(val.string()));
}

Handle<Value> JsParcel::WriteInterfaceToken(const Arguments& args) {
  HandleScope scope;
  JsParcel *jsP;
  String16 val;

  if (!args[0]->IsString())
    return ThrowException(Exception::TypeError(String::New("Argument 1 should be a string")));

  val = String16(*(String::Utf8Value(args[0])));

  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  jsP->parcel.writeInterfaceToken(val);

  return scope.Close(Null());
}

Handle<Value> JsParcel::ReadExceptionCode(const Arguments& args) {
  HandleScope scope;
  JsParcel *jsP;
  int32_t ex;

  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  ex = jsP->parcel.readExceptionCode();

  return scope.Close(Number::New(ex));
}

Handle<Value> JsParcel::WriteInt32(const Arguments& args) {
  HandleScope scope;
  JsParcel *jsP;
  uint32_t val;

  if (!args[0]->IsNumber())
    return ThrowException(Exception::TypeError(String::New("Argument 1 should be a number")));

  val = args[0]->ToUint32()->Value();

  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  jsP->parcel.writeInt32(val);
  
  return scope.Close(Null());
}

Handle<Value> JsParcel::ReadInt32(const Arguments& args) {
  HandleScope scope;
  JsParcel *jsP;
  int32_t r;

  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  r = jsP->parcel.readInt32();

  return scope.Close(Number::New(r));  
}

Handle<Value> JsParcel::SetDataPosition(const Arguments& args) {
  HandleScope scope;
  JsParcel *jsP;
  uint32_t val;

  if (!args[0]->IsNumber())
    return ThrowException(Exception::TypeError(String::New("Argument 1 should be a number")));

  val = args[0]->ToUint32()->Value();

  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  jsP->parcel.setDataPosition(val);

  return scope.Close(Null());
}

Handle<Value> JsParcel::GetDataPosition(const Arguments& args) {
  HandleScope scope;
  JsParcel *jsP;
  size_t pos;

  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  pos = jsP->parcel.dataPosition();

  return scope.Close(Number::New(pos));
}

Handle<Value> JsParcel::GetDataSize(const Arguments& args) {
  HandleScope scope;
  JsParcel *jsP;
  size_t sz; 

  jsP = ObjectWrap::Unwrap<JsParcel>(args.This());
  sz = jsP->parcel.dataSize();

  return scope.Close(Number::New(sz));
}

void Init(Handle<Object> exports) {
  JsServiceManager::Init(exports);
  JsService::Init(exports);
  JsParcel::Init(exports);
}

NODE_MODULE(jsbinder, Init);
