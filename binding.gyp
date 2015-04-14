#
# Copyright (C) 2015 Opersys inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

{
    "targets": [
        {
            "target_name": "jsbinder",
            "include_dirs": [
                "external/aosp/include",
            ],
            "cflags": [
                "-include AndroidConfig.h"
            ],
            "defines": [
                "ANDROID_SMP=0"
            ],
            "sources": [
                "external/aosp/binder/Binder.cpp",
                "external/aosp/binder/BpBinder.cpp",
                "external/aosp/binder/IInterface.cpp",
                "external/aosp/binder/IMemory.cpp",
                "external/aosp/binder/IPCThreadState.cpp",
                "external/aosp/binder/IPermissionController.cpp",
                "external/aosp/binder/IServiceManager.cpp",
                "external/aosp/binder/MemoryBase.cpp",
                "external/aosp/binder/MemoryDealer.cpp",
                "external/aosp/binder/MemoryHeapBase.cpp",
                "external/aosp/binder/MemoryHeapPmem.cpp",
                "external/aosp/binder/Parcel.cpp",
                "external/aosp/binder/Permission.cpp",
                "external/aosp/binder/ProcessState.cpp",
                "external/aosp/binder/Static.cpp",

                "external/aosp/utils/BufferedTextOutput.cpp",
                "external/aosp/utils/CallStack.cpp",
                "external/aosp/utils/Debug.cpp",
                "external/aosp/utils/FileMap.cpp",
                "external/aosp/utils/Flattenable.cpp",
                "external/aosp/utils/Looper.cpp",
                "external/aosp/utils/misc.cpp",
                "external/aosp/utils/ObbFile.cpp",
                "external/aosp/utils/PackageRedirectionMap.cpp",
                "external/aosp/utils/Pool.cpp",
                "external/aosp/utils/RefBase.cpp",
                "external/aosp/utils/ResourceTypes.cpp",
                "external/aosp/utils/SharedBuffer.cpp",
                "external/aosp/utils/Static.cpp",
                "external/aosp/utils/StopWatch.cpp",
                "external/aosp/utils/String16.cpp",
                "external/aosp/utils/String8.cpp",
                "external/aosp/utils/StringArray.cpp",
                "external/aosp/utils/SystemClock.cpp",
                "external/aosp/utils/tests",
                "external/aosp/utils/TextOutput.cpp",
                "external/aosp/utils/Threads.cpp",
                "external/aosp/utils/Timers.cpp",
                "external/aosp/utils/VectorImpl.cpp",

                "external/aosp/libcutils/buffer.c",
                "external/aosp/libcutils/uio.c",
                "external/aosp/libcutils/socket_local_client.c",
                "external/aosp/libcutils/socket_loopback_server.c",
                "external/aosp/libcutils/native_handle.c",
                "external/aosp/libcutils/socket_inaddr_any_server.c",
                "external/aosp/libcutils/properties.c",
                "external/aosp/libcutils/load_file.c",
                "external/aosp/libcutils/dir_hash.c",
                "external/aosp/libcutils/ashmem-host.c",
                "external/aosp/libcutils/record_stream.c",
                "external/aosp/libcutils/tztime.c",
                "external/aosp/libcutils/open_memstream.c",
                "external/aosp/libcutils/cpu_info.c",
                "external/aosp/libcutils/strdup16to8.c",
                "external/aosp/libcutils/socket_local_server.c",
                "external/aosp/libcutils/threads.c",
                "external/aosp/libcutils/dlmalloc_stubs.c",
                "external/aosp/libcutils/sched_policy.c",
                "external/aosp/libcutils/config_utils.c",
                "external/aosp/libcutils/socket_network_client.c",
                "external/aosp/libcutils/abort_socket.c",
                "external/aosp/libcutils/strdup8to16.c",
                "external/aosp/libcutils/array.c",
                "external/aosp/libcutils/process_name.c",
                "external/aosp/libcutils/zygote.c",
                "external/aosp/libcutils/mq.c",
                "external/aosp/libcutils/socket_loopback_client.c",
                "external/aosp/libcutils/atomic.c",
                "external/aosp/libcutils/selector.c",
                "external/aosp/libcutils/hashmap.c",

                "external/aosp/liblog/event_tag_map.c",
                "external/aosp/liblog/fake_log_device.c",
                "external/aosp/liblog/logd_write.c",
                "external/aosp/liblog/logprint.c",

                "lib/jsbinder.cpp"
            ]
        }
    ]
}
