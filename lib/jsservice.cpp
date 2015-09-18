#include <string>
#include <iostream>
#include <sstream>

#include <node.h>

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

v8::Handle<v8::Value> JsService::Dump(const v8::Arguments& args) {
  v8::HandleScope scope;
  JsService *jsSv;
  Vector<String16> dumpArgs;
  status_t err;
  std::stringstream errMsg;
  const char* fname = ".jsbinder-dumparea";

  jsSv = node::ObjectWrap::Unwrap<JsService>(args.This());
  assert(jsSv->sv != 0);

  // There might be some arguments.
  if (args.Length() > 0) {
    for (int i = 0; i < args.Length(); i++) {
      if (!args[i]->IsString()) {
        errMsg << "Argument " << i + 1 << " should be a string";
        return v8::ThrowException(
          v8::Exception::TypeError(v8::String::New(errMsg.str().c_str())));
      }
      else
        dumpArgs.add(String16((const char16_t *)*v8::String::Value(args[i])));
    }
  }

  // Initialize the area where the 'dump' command will write its output.
  if (!jsSv->dumpArea) {

    // FIXME: This is clearly not safe to do. The name should be random.
    if ((jsSv->dumpFd = open(fname, O_RDWR | O_CREAT)) < 0) {
      errMsg << "Cannot open the file for dumping service: "
             << strerror(errno);

      return v8::ThrowException(
        v8::Exception::TypeError(v8::String::New(errMsg.str().c_str())));
    }

    unlink(fname);
    jsSv->dumpArea = mmap(NULL, jsSv->dumpAreaSize, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE, jsSv->dumpFd, 0);

    if (!jsSv->dumpArea) {
      errMsg << "mmap on memory area for dump failed: "
             << strerror(errno);

      close(jsSv->dumpFd);
      jsSv->dumpFd = 0;

      return v8::ThrowException(
        v8::Exception::TypeError(v8::String::New(errMsg.str().c_str())));
    }
  }

  // Sync to the start of the temporary file.
  lseek(jsSv->dumpFd, 0, SEEK_SET);
  err = jsSv->sv->dump(jsSv->dumpFd, dumpArgs);

  if (err == NO_ERROR) {
    // Get how much has been read.
    off_t pos = lseek(jsSv->dumpFd, 0, SEEK_CUR);

    return scope.Close(v8::String::New((char *)jsSv->dumpArea, pos));
  } 
  else
    return scope.Close(v8::String::New("Dump failed"));
}

void JsService::Init(v8::Handle<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
  v8::Local<v8::ObjectTemplate> proto;

  proto = tpl->PrototypeTemplate();

  tpl->SetClassName(v8::String::NewSymbol("JsService"));
  tpl->InstanceTemplate()->SetInternalFieldCount(4);

  proto->Set(v8::String::NewSymbol("ping"),
             v8::FunctionTemplate::New(Ping)->GetFunction());
  proto->Set(v8::String::NewSymbol("transact"),
             v8::FunctionTemplate::New(Transact)->GetFunction());
  proto->Set(v8::String::NewSymbol("getInterface"),
             v8::FunctionTemplate::New(GetInterface)->GetFunction());
  proto->Set(v8::String::NewSymbol("dump"),
             v8::FunctionTemplate::New(Dump)->GetFunction());

  constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
  exports->Set(v8::String::NewSymbol("JsService"), constructor);
}
