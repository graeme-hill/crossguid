#include <string>
#include <sstream>
#include <jni.h>
#include <iostream>
#include "../../test/Test.hpp"
#include "../../Guid.hpp"

extern "C"
{
	jstring Java_ca_graemehill_crossguid_testapp_MainActivity_test(
		JNIEnv *env, jobject thiz)
	{
		std::stringstream resultStream;
		xg::initJni(env);
		test(resultStream);
		return env->NewStringUTF(resultStream.str().c_str());
	}
}
