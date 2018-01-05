#
# Copyright (C) 2017 Opersys inc.
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
      "-I <(aosp_build_top)/external/libcxx/include",

      # For Binder.
      "-I <(aosp_build_top)/frameworks/native/include/",

      # AOSP core libraries
      "-I <(aosp_build_top)/system/core/include/",
    ]
  },

    "targets": [
      {
        "target_name": "jsbinder-binder64",
        "defines": [
          "AOSP_VERSION=7"
        ],
        "sources": ["<@(jsbinder_sources)"],
        "cflags": ["<@(extra_cflags)"],
        "ldflags": ["<(aosp_crtbegin)"],
        "libraries": [
          "<(aosp_product_out)/system/lib64/libbinder.so"
        ]
      }
    ],

    "conditions": [
      ['target_arch==\"arm64\"', {
        "targets": [
          {
            "target_name": "jsbinder-binder64",
             "defines": [
               "AOSP_VERSION=7"
             ],
            "sources": ["<@(jsbinder_sources)"],
            "cflags": ["<@(extra_cflags)"],
            "ldflags": ["<(aosp_crtbegin)"],
            "libraries": [
              "<(aosp_product_out)/system/lib64/libbinder.so"
            ]
          }
        ]
      },
       'target_arch==\"arm\"', {
         "targets": [
           {
             "target_name": "jsbinder-binder32",
             "defines": [
               "BINDER_IPC_32BIT",
               "AOSP_VERSION=7"
             ],
             "sources": ["<@(jsbinder_sources)"],
             "cflags": ["<@(extra_cflags)"],
             "ldflags": ["<(aosp_crtbegin)"],
             "libraries": [
               "<(aosp_product_out)/system/lib/libbinder.so"
             ]
           }
         ]
       },
       'target_arch==\"ia32\"', {
         "targets": [
           {
             "target_name": "jsbinder-binder32",
             "defines": [
               "BINDER_IPC_32BIT",
               "AOSP_VERSION=7"
             ],
             "sources": ["<@(jsbinder_sources)"],
             "cflags": ["<@(extra_cflags)"],
             "ldflags": ["<(aosp_crtbegin)"],
             "libraries": [
               "<(aosp_product_out)/system/lib/libbinder.so"
             ]
           }
         ]
       }]
    ],

    "targets": [
      {
        "target_name": "jsbinderversion",
        "sources": ["<@(jsbinderversion_sources)"],
        "cflags": ["<@(extra_cflags)"]
      }
    ]
}
