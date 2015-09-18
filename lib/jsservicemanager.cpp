#include "jsservicemanager.hxx"
#include "jsservice.hxx"

JsServiceManager::JsServiceManager() {
  this->sm = defaultServiceManager();
}

JsServiceManager::~JsServiceManager() {
}

void JsServiceManager::Init(v8::Handle<v8::Object> exports) {
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(New);

  tpl->SetClassName(v8::String::NewSymbol("JsServiceManager"));
  tpl->InstanceTemplate()->SetInternalFieldCount(2);
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("list"),
                                v8::FunctionTemplate::New(List)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("getService"),
                                v8::FunctionTemplate::New(GetService)->GetFunction());

  constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
  exports->Set(v8::String::NewSymbol("JsServiceManager"), constructor);
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
