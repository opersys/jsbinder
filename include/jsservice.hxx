#include <node.h>

#include <binder/Parcel.h>

using namespace android;

#ifndef _JSSERVICE_HXX
#define _JSSERVICE_HXX

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
  static v8::Handle<v8::Value> Dump(const v8::Arguments& args);

  // The memory that is reserved for dumpMem calls on the service.
  const int dumpAreaSize = 4096000;
  void *dumpArea;
  int dumpFd;

  sp<IBinder> sv;

  explicit JsService();
  ~JsService();
};

#endif // _JSSERVICE_HXX
