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
    "target_defaults": {
        "include_dirs": [
            "external/aosp/include",
        ],
        "cflags": [
            "-include AndroidConfig.h",
        ],
        "cflags_cc": [
            "--std=c++0x"                
        ],
        "defines": [
            "ANDROID_SMP=0",
            "LIBLOG_LOG_TAG=1005",
            "__USE_BSD"
        ],    
        "sources": [
            "external/aosp/binder/IPCThreadState.cpp",
            "external/aosp/binder/Debug.cpp",
            "external/aosp/binder/ProcessState.cpp",
            "external/aosp/binder/PermissionCache.cpp",
            "external/aosp/binder/IInterface.cpp",
            "external/aosp/binder/Parcel.cpp",
            "external/aosp/binder/AppOpsManager.cpp",
            "external/aosp/binder/Static.cpp",
            "external/aosp/binder/IMemory.cpp",
            "external/aosp/binder/IAppOpsService.cpp",
            "external/aosp/binder/TextOutput.cpp",
            "external/aosp/binder/MemoryBase.cpp",
            "external/aosp/binder/IAppOpsCallback.cpp",
            "external/aosp/binder/IServiceManager.cpp",
            "external/aosp/binder/IBatteryStats.cpp",
            "external/aosp/binder/BpBinder.cpp",
            "external/aosp/binder/IPermissionController.cpp",
            "external/aosp/binder/BufferedTextOutput.cpp",
            "external/aosp/binder/Binder.cpp",

            "external/aosp/utils/Trace.cpp",
            "external/aosp/utils/NativeHandle.cpp",
            "external/aosp/utils/Threads.cpp",
            "external/aosp/utils/VectorImpl.cpp",
            "external/aosp/utils/Static.cpp",
            "external/aosp/utils/SystemClock.cpp",
            "external/aosp/utils/LinearTransform.cpp",
            "external/aosp/utils/SharedBuffer.cpp",
            "external/aosp/utils/FileMap.cpp",
            "external/aosp/utils/LinearAllocator.cpp",
            "external/aosp/utils/Tokenizer.cpp",
            "external/aosp/utils/Timers.cpp",
            "external/aosp/utils/RefBase.cpp",
            "external/aosp/utils/BasicHashtable.cpp",
            "external/aosp/utils/String16.cpp",
            "external/aosp/utils/String8.cpp",
            "external/aosp/utils/JenkinsHash.cpp",
            "external/aosp/utils/Log.cpp",
            "external/aosp/utils/Looper.cpp",
            "external/aosp/utils/Printer.cpp",
            "external/aosp/utils/BlobCache.cpp",
            "external/aosp/utils/StopWatch.cpp",
            "external/aosp/utils/Unicode.cpp",
            "external/aosp/utils/misc.cpp",

            "external/aosp/libcutils/properties.c",
            "external/aosp/libcutils/strlcpy.c",
            "external/aosp/libcutils/atomic.c",
            "external/aosp/libcutils/native_handle.c",
            "external/aosp/libcutils/process_name.c",
            "external/aosp/libcutils/socket_inaddr_any_server.c",
            "external/aosp/libcutils/open_memstream.c",
            "external/aosp/libcutils/qtaguid.c",
            "external/aosp/libcutils/config_utils.c",
            "external/aosp/libcutils/hashmap.c",
            "external/aosp/libcutils/socket_loopback_server.c",
            "external/aosp/libcutils/dlmalloc_stubs.c",
            "external/aosp/libcutils/uevent.c",
            "external/aosp/libcutils/multiuser.c",
            "external/aosp/libcutils/ashmem-host.c",
            "external/aosp/libcutils/strdup16to8.c",
            "external/aosp/libcutils/fs.c",
            "external/aosp/libcutils/strdup8to16.c",
            "external/aosp/libcutils/arch-mips/android_memset.c",
            "external/aosp/libcutils/partition_utils.c",
            "external/aosp/libcutils/socket_local_server.c",
            "external/aosp/libcutils/socket_network_client.c",
            "external/aosp/libcutils/socket_loopback_client.c",
            "external/aosp/libcutils/threads.c",
            "external/aosp/libcutils/iosched_policy.c",
            "external/aosp/libcutils/trace-host.c",
            "external/aosp/libcutils/load_file.c",
            "external/aosp/libcutils/socket_local_client.c",
            "external/aosp/libcutils/klog.c",
            "external/aosp/libcutils/sched_policy.c",
            "external/aosp/libcutils/record_stream.c",
            "external/aosp/libcutils/debugger.c",

            "external/aosp/liblog/event_tag_map.c",
            "external/aosp/liblog/fake_log_device.c",
            "external/aosp/liblog/logd_write.c",
            "external/aosp/liblog/logprint.c",
            
            "external/aosp/base/stringprintf.cpp",
            "external/aosp/base/logging.cpp",
            "external/aosp/base/file.cpp",
            "external/aosp/base/strings.cpp",                
            
            "lib/jsbinder.cpp"
        ]
    },

    "targets": [
        {
            "target_name": "jsbinder-binder64",
            "defines": [
            ]
        },
        {
            "target_name": "jsbinder-binder32",
            "defines": [
                "BINDER_IPC_32BIT",
            ]
        },
        {
            "target_name": "jsbinderversion",
	        "sources/": [
                [ "exclude", "external/aosp" ],
                [ "exclude", "lib/jsbinder.cpp" ]
            ],
            "sources": [ "lib/jsbinderversion.cpp" ]
        }
    ]
}
