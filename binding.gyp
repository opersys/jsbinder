#
# Copyright (C) 2015-2017 Opersys inc.
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

# Build with:

{
  "variables": {
    "aosp_build_top": "<!(echo $ANDROID_BUILD_TOP)",
    "aosp_product_out": "<!(echo $ANDROID_PRODUCT_OUT)",
    "aosp_major": "<!(cat $ANDROID_PRODUCT_OUT/system/build.prop | perl -ne'/ro.build.version.release=([0-9]*)\./ && print $1+\"\n\"')",
    "aosp_crtbegin": "<(aosp_product_out)/obj/lib/crtbegin_so.o",

    # JSBinder
    "jsbinder_sources": [
      "lib/jsparcel.cpp",
      "lib/jsservicemanager.cpp",
      "lib/jsservice.cpp",
      "lib/jsbinder.cpp"
    ],

    # 32/64 bit detector.
    "jsbinderversion_sources": [
      "lib/jsbinderversion.cpp"
    ]
  },

  "conditions": [

    # AOSP versions

    ["aosp_major==8", {"includes": {"aosp_oreo.gypi"}},
     "aosp_major==7", {"includes": {"aosp_nougat.gypi"}},
     "aosp_major==6", {"includes": {"aosp_nougat.gypi"}},
     #Untested
     "aosp_major==5", {"includes": {"aosp_nougat.gypi"}},
     "aosp_major==4", {"includes": {"aosp_nougat.gypi"}}],
  ],

  "target_defaults": {
    "include_dirs": [
      "external/aosp/include",
      "include",
      "<!(node -e \"require('nan')\")"
    ],

    "conditions": [
      ['target_arch==\"arm64\"', {
        "cflags": [
          "-target aarch64-linux-android",
          "-isystem <(aosp_build_top)/bionic/libc/kernel/uapi/asm-arm64"
        ],
        "ldflags": [
#          "-target aarch64-linux-android",
          "-Wl,-dynamic-linker,/system/bin/linker64",
        ],
      }],
      ['target_arch==\"arm\"', {
        "cflags": [
          "-target arm-linux-android",
          "-isystem <(aosp_build_top)/bionic/libc/kernel/uapi/asm-arm"
        ],
        "ldflags": [
#          "-target arm-linux-android",
          "-Wl,-dynamic-linker,/system/bin/linker32",
        ]
      }],
      ['target_arch==\"ia32\"', {
        "cflags": [
          "-isystem <(aosp_build_top)/bionic/libc/kernel/uapi/asm-x86"
        ],
        "ldflags": [
#          "-target i686-linux-android",
          "-Wl,-dynamic-linker,/system/bin/linker32",
        ]
      }],
    ],

    "cflags": [
      #"-nostdlib",
      #"-nodefaultlibs",

      "-I <(aosp_build_top)/bionic/libc/include",
      # "-I <(aosp_build_top)/bionic/libm/include",

      "-I <(aosp_build_top)/bionic/libc/kernel/uapi",
      "-I <(aosp_build_top)/bionic/libc/kernel/android/uapi",
    ],
    "cflags_cc": [
      "--std=c++0x"
    ],
    "defines": [
      "ANDROID", "__ANDROID__"
    ],
    "ldflags": [
      "-BDynamic",
      "-nostdlib",
      "-Wl,--gc-sections"
    ],
  }
}
