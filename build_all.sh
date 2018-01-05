#!/bin/bash

if [[ ! -e $ANDROID_PRODUCT_OUT/system/build.prop ]]; then
    echo "Missing system/build.prop. Your AOSP was not built!"
    exit 1
fi

PKG_NAME=$(cat 'package.json' | jq -r '.name')
PKG_VER=$(cat 'package.json' | jq -r '.version');

AOSP_VERSION=$(cat $ANDROID_PRODUCT_OUT/system/build.prop | perl -ne'/ro.build.version.release=([0-9]*)\./ && print "$1\n"')
AOSP_ARCH=$(cat $ANDROID_PRODUCT_OUT/system/build.prop | perl -ne'/ro.product.cpu.abi=(.*)$/ && print $1')

echo "AOSP_ARCH: $AOSP_ARCH"
echo "AOSP_VERSION: $AOSP_VERSION"

case "$AOSP_ARCH" in
    arm64-v8a)
        NODE_ARCH=arm64
        ;;
    arm*)
        NODE_ARCH=arm
        ;;
    x86)
        NODE_ARCH=ia32
        ;;
esac

if [[ $AOSP_VERSION -eq 8 ]]; then
    CLANG_DIR=$(ls -d $ANDROID_BUILD_TOP/prebuilts/clang/host/linux-x86/clang-[0-9]* | sort -rn | head -1)
    CC=$CLANG_DIR/bin/clang
    CXX=$CLANG_DIR/bin/clang++
elif [[ $AOSP_VERSION -eq 7 || $AOSP_VERSION -eq 6 || $AOSP_VERSION -eq 5 || $AOSP_VERSION -eq 4 ]]; then
    CLANG_DIR=$(ls -d $ANDROID_BUILD_TOP/prebuilts/clang/linux-x86/host/3\.[0-9]* | sort -rn)
    CC=$CLANG_DIR/bin/clang
    CXX=$CLANG_DIR/bin/clang++
    LINK=$(ls $ANDROID_TOOLCHAIN/*-g++ | head -1)
else
    echo "Unhandled version: $AOSP_VERSION"
    exit 1
fi

NODE_SRC=/imports/Misc/Src/NODE_MODULES/nodejs-mobile.$NODE_ARCH

if [[ ! -d $NODE_SRC ]]; then
    echo "Oops dude you might not have created $NODE_SRC!"
    exit 1
fi

export CC CXX LINK

$NDK_ARM node-gyp clean configure build \
         --verbose \
         --arch=$NODE_ARCH \
         --nodedir=$NODE_SRC

for i in build.*; do
    aosp=$(echo $i | cut -d '.' -f 2)
    arch=$(echo $i | cut -d '.' -f 3)
    n="${PKG_NAME}-${PKG_VER}_${aosp}_${arch}.tar.gz"
    
    echo "Packaging: $i to $n"
    
    oldname=$i
    mv $i build
    
    (cd .. && tar --hard-dereference \
                  --exclude=obj.target \
                  --exclude=.git \
                  --exclude=.idea \
                  --exclude=build.oreo* \
                  --exclude=build.nougat* \
                  -zcf $n jsbinder)
    mv build $oldname
done
