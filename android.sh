#!/usr/bin/env bash

export LC_NUMERIC="en_US.UTF-8"

pushd android
./gradlew clean assembleDebug
adb uninstall ca.graemehill.crossguid.testapp || { exit 1; }
adb install app/build/outputs/apk/debug/app-debug.apk || { exit 1; }
adb shell am start -n ca.graemehill.crossguid.testapp/ca.graemehill.crossguid.testapp.MainActivity
popd
