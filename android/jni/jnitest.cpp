#include <string>
#include <sstream>
#include <jni.h>
#include <iostream>
#include "../../test.h"
#include "../../guid.h"

extern "C" 
{

  jstring Java_ca_graemehill_crossguid_testapp_MainActivity_test(JNIEnv *env, jobject thiz)
  {
    std::stringstream resultStream;
    test(GuidGenerator(env), resultStream);
    return env->NewStringUTF(resultStream.str().c_str());
  }

}
