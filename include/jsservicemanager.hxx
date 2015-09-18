#include <node.h>

#include <binder/IPCThreadState.h>
#include <binder/Binder.h>
#include <binder/BpBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>

#ifndef _JSSERVICEMANAGER_HXX
#define _JSSERVICEMANAGER_HXX 

using namespace android;

#include <node.h>

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

#endif // _JSSERVICEMANAGER_HXX
