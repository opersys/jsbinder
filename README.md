# JS Binder Library

This module is usually built as part of Binder Explorer so you should probably
consult that project in order to use it.

The Brave Individuals that may want to build and use this module in their own
projects should do the following.

- Get Node.js for Android sources from:

https://github.com/opersys/node

- In the node-android directory:

source ./android-configure <Android NDK directory> <ia32 or arm>

- In the jslibbinder directory:

node-gyp --arch=arm --nodedir=`node-android directory`

This should give you the .so needed for Android. The proper use of this module
within another project is left as an exercise to the reader while we figure
out a more friendly way to make Android-based Node.js project.
