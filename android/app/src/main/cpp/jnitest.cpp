#include <string>
#include <sstream>
#include <atomic>
#include <iostream>

#include <jni.h>
#include "Guid.hpp"
#include "Test.hpp"

JavaVM *&javaVM() {
    static JavaVM *jvm;
    return jvm;
}

extern "C"
{

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *jvm, void * /* reserved */) {
    javaVM() = jvm;
    return JNI_VERSION_1_6;
}

JNIEXPORT jstring JNICALL
Java_ca_graemehill_crossguid_testapp_MainActivity_test(
    JNIEnv *env, jobject /*thiz*/)
{
    std::stringstream resultStream;
    xg::initJni(env);
    test(resultStream);
    return env->NewStringUTF(resultStream.str().c_str());
}

JNIEXPORT jstring JNICALL
Java_ca_graemehill_crossguid_testapp_MainActivity_newGuid(
    JNIEnv *env, jobject /*thiz*/) {
    return env->NewStringUTF(xg::newGuid(env).str().c_str());
}

JNIEXPORT jstring JNICALL
Java_ca_graemehill_crossguid_testapp_MainActivity_createGuidFromNativeThread(
    JNIEnv *env, jobject /*thiz*/) {

    // there is no promise<> in armeabi of ndk
    // so ugly atomic_bool wait solution
    std::atomic_bool ready { false };
    std::string guid;

    std::thread([&ready, &guid](){
        JNIEnv *threadEnv;
        javaVM()->AttachCurrentThread(&threadEnv, NULL);
        guid = xg::newGuid(threadEnv);
        javaVM()->DetachCurrentThread();

        ready = true;
    }).detach();

    while (!ready);
    return env->NewStringUTF(guid.c_str());
}

}
