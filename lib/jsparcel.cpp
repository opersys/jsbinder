#include <node.h>

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

void JsParcel::Init(v8::Handle<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);
  v8::Local<v8::ObjectTemplate> proto;

  proto = tpl->PrototypeTemplate();

  tpl->SetClassName(v8::String::NewSymbol("JsParcel"));
  tpl->InstanceTemplate()->SetInternalFieldCount(13);

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
