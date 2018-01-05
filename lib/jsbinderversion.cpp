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

#include <nan.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef AOSP_VERSION
#include <linux/binder.h>
#else
#include <linux/android/binder.h>
#endif

NAN_METHOD(getBinderVersion) {
    int vers = 0, result, fd = -1;
  
    fd = open("/dev/binder", O_RDWR);
    
    if (fd >= 0) {
        result = ioctl(fd, BINDER_VERSION, &vers);

        if (result == -1) {
            std::string serr = "Binder ioctl to obtain version failed: " + std::string(strerror(errno));

            Nan::ThrowError(serr.c_str());
         
            close(fd);
            fd = -1;
        }
    } else {
        std::string serr = "Opening '/dev/binder' failed: " + std::string(strerror(errno));

        Nan::ThrowError(serr.c_str());
    }
  
    close(fd);

    info.GetReturnValue().Set(Nan::New<v8::Number>(vers));
}

NAN_MODULE_INIT(Init) {
    Nan::Set(target,
             Nan::New("Version").ToLocalChecked(),
             Nan::GetFunction(Nan::New<v8::FunctionTemplate>(getBinderVersion)).ToLocalChecked());
}

NODE_MODULE(jsbinderversion, Init);
