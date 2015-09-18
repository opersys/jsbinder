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
 
#ifndef _JSPARCEL_HXX
#define _JSPARCEL_HXX

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
  static v8::Handle<v8::Value> ReadExceptionCode(const v8::Arguments& args);
  static v8::Handle<v8::Value> WriteInterfaceToken(const v8::Arguments& args);

  static v8::Handle<v8::Value> SetDataPosition(const v8::Arguments& args);
  static v8::Handle<v8::Value> GetDataPosition(const v8::Arguments& args);
  static v8::Handle<v8::Value> GetDataSize(const v8::Arguments& args);

  explicit JsParcel();
  ~JsParcel();
};

#endif // _JSPARCEL_HXX
