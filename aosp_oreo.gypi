#
# Copyright (C) 2017-2018 Opersys inc.
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
    "variables": {
        "extra_cflags": [
            "--sysroot <(aosp_build_top)/prebuilts/clang/host/linux-x86/clang-4053586",
            "-isystem <(aosp_build_top)/prebuilts/clang/host/linux-x86/clang-4053586/lib/clang/5.0/include",

            "-nostdlibinc",

            # LibC++
            "-I <(aosp_build_top)/prebuilts/clang/host/linux-x86/clang-4053586/include/c++/v1",

            # For Binder.
            "-I <(aosp_build_top)/frameworks/native/libs/binder/include/",

            # AOSP core libraries
            "-I <(aosp_build_top)/system/core/libutils/include/",
            "-I <(aosp_build_top)/system/core/base/include/",
            "-I <(aosp_build_top)/system/core/libcutils/include",
            "-I <(aosp_build_top)/system/core/liblog/include",
            "-I <(aosp_build_top)/system/core/libsystem/include",
        ]
    },

    "conditions": [
        ['target_arch==\"x86_64\"', {
            "targets": [
                {
                    "target_name": "jsbinder-binder64",
                    "sources": ["<@(jsbinder_sources)"],
                    "cflags+": [
                        "-isystem <(aosp_build_top)/prebuilts/clang/host/linux-x86/clang-4053586/lib64/clang/5.0/include",
                        "<@(extra_cflags)",
                    ],
#                    "ldflags": ["<(aosp_crtbegin)"],
                    "libraries": [
                        "<(aosp_product_out)/system/lib64/libbinder.so",
                        "<(aosp_product_out)/system/lib64/libc.so"
                    ]
                },
                {
                    "target_name": "jsbinderversion",
                    "sources": ["<@(jsbinderversion_sources)"],
                    "cflags+": ["<@(extra_cflags)"],
                    "libraries": [
                        "<(aosp_product_out)/system/lib64/libc.so"
                    ]
                }
            ]
        }],
         ['target_arch==\"arm64\"', {
            "targets": [
                {
                    "target_name": "jsbinder-binder64",
                    "sources": ["<@(jsbinder_sources)"],
                    "cflags+": [
                        "-isystem <(aosp_build_top)/prebuilts/clang/host/linux-x86/clang-4053586/lib64/clang/5.0/include",
                        "<@(extra_cflags)",
                    ],
#                    "ldflags": ["<(aosp_crtbegin)"],
                    "libraries": [
                        "<(aosp_product_out)/system/lib64/libbinder.so",
                        "<(aosp_product_out)/system/lib64/libc.so"
                    ]
                },
                {
                    "target_name": "jsbinderversion",
                    "sources": ["<@(jsbinderversion_sources)"],
                    "cflags+": ["<@(extra_cflags)"],
                    "libraries": [
                        "<(aosp_product_out)/system/lib64/libc.so"
                    ]
                }
            ]
        }],
        ['target_arch==\"arm\"', {
            "targets": [
                {
                    "target_name": "jsbinder-binder32",
                    "defines": [
                        "BINDER_IPC_32BIT",
                    ],
                    "sources": ["<@(jsbinder_sources)"],
                    "cflags+": [
                        "<@(extra_cflags)"
                    ],
#                    "ldflags": ["<(aosp_crtbegin)"],
                    "libraries": [
                        "<(aosp_product_out)/system/lib/libbinder.so",
                        "<(aosp_product_out)/system/lib/libc.so"
                    ]
                },
                {
                    "target_name": "jsbinderversion",
                    "sources": ["<@(jsbinderversion_sources)"],
                    "cflags+": ["<@(extra_cflags)"],
                    "libraries": [
                        "<(aosp_product_out)/system/lib/libc.so"
                    ]
                }
            ]
        }]
    ],
}
