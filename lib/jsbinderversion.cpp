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
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>

#include <linux/binder.h>

v8::Handle<v8::Value> getBinderVersion() {
  v8::HandleScope scope;
  int vers = 0, result, fd = -1;
  
  fd = open("/dev/binder", O_RDWR);
    
  if (fd >= 0) {
    result = ioctl(fd, BINDER_VERSION, &vers);

    if (result == -1) {
      std::string serr = "Binder ioctl to obtain version failed: " + std::string(strerror(errno));
      
      v8::ThrowException(
        v8::Exception::Error(v8::String::New(serr.c_str())));
          
      close(fd);
      fd = -1;
    }
  } else {
    std::string serr = "Opening '/dev/binder' failed: " + std::string(strerror(errno));
    
    v8::ThrowException(
      v8::Exception::Error(v8::String::New(serr.c_str())));
  }
  
  close(fd);

  return scope.Close(v8::Number::New(vers));
}

void Init(v8::Handle<v8::Object> exports) {
  exports->Set(v8::String::NewSymbol("Version"), getBinderVersion());
}

NODE_MODULE(jsbinderversion, Init);
