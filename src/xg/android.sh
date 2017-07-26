#!/usr/bin/env bash

pushd android
ndk-build clean || { exit 1; }
ndk-build || { exit 1; }
ant debug || { exit 1; }
adb uninstall ca.graemehill.crossguid.testapp || { exit 1; }
adb install bin/TestApp-debug.apk || { exit 1; }
adb shell am start -n ca.graemehill.crossguid.testapp/ca.graemehill.crossguid.testapp.MainActivity
popd
