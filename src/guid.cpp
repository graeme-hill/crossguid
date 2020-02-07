/*
The MIT License (MIT)

Copyright (c) 2014 Graeme Hill (http://graemehill.ca)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "crossguid/guid.hpp"

#ifdef GUID_LIBUUID
#include <uuid/uuid.h>
#endif

#ifdef GUID_CFUUID
#include <CoreFoundation/CFUUID.h>
#endif

#ifdef GUID_WINDOWS
#include <objbase.h>
#endif

#ifdef GUID_ANDROID
#include <jni.h>
#include <cassert>
#endif

BEGIN_XG_NAMESPACE

#ifdef GUID_ANDROID
AndroidGuidInfo androidInfo;

AndroidGuidInfo AndroidGuidInfo::fromJniEnv(JNIEnv *env)
{
	AndroidGuidInfo info;
	info.env = env;
	auto localUuidClass = env->FindClass("java/util/UUID");
	info.uuidClass = (jclass)env->NewGlobalRef(localUuidClass);
	env->DeleteLocalRef(localUuidClass);
	info.newGuidMethod = env->GetStaticMethodID(
		info.uuidClass, "randomUUID", "()Ljava/util/UUID;");
	info.mostSignificantBitsMethod = env->GetMethodID(
		info.uuidClass, "getMostSignificantBits", "()J");
	info.leastSignificantBitsMethod = env->GetMethodID(
		info.uuidClass, "getLeastSignificantBits", "()J");
	info.initThreadId = std::this_thread::get_id();
	return info;
}

void initJni(JNIEnv *env)
{
	androidInfo = AndroidGuidInfo::fromJniEnv(env);
}
#endif

// This is the linux friendly implementation, but it could work on other
// systems that have libuuid available
#ifdef GUID_LIBUUID
Guid newGuid()
{
	std::array<unsigned char, 16> data;
	static_assert(std::is_same<unsigned char[16], uuid_t>::value, "Wrong type!");
	uuid_generate(data.data());
	return Guid{std::move(data)};
}
#endif

// this is the mac and ios version
#ifdef GUID_CFUUID
Guid newGuid()
{
	auto newId = CFUUIDCreate(NULL);
	auto bytes = CFUUIDGetUUIDBytes(newId);
	CFRelease(newId);

	std::array<unsigned char, 16> byteArray =
	{{
		bytes.byte0,
		bytes.byte1,
		bytes.byte2,
		bytes.byte3,
		bytes.byte4,
		bytes.byte5,
		bytes.byte6,
		bytes.byte7,
		bytes.byte8,
		bytes.byte9,
		bytes.byte10,
		bytes.byte11,
		bytes.byte12,
		bytes.byte13,
		bytes.byte14,
		bytes.byte15
	}};
	return Guid{std::move(byteArray)};
}
#endif

// obviously this is the windows version
#ifdef GUID_WINDOWS
Guid newGuid()
{
	GUID newId;
	CoCreateGuid(&newId);

	std::array<unsigned char, 16> bytes =
	{
		(unsigned char)((newId.Data1 >> 24) & 0xFF),
		(unsigned char)((newId.Data1 >> 16) & 0xFF),
		(unsigned char)((newId.Data1 >> 8) & 0xFF),
		(unsigned char)((newId.Data1) & 0xff),

		(unsigned char)((newId.Data2 >> 8) & 0xFF),
		(unsigned char)((newId.Data2) & 0xff),

		(unsigned char)((newId.Data3 >> 8) & 0xFF),
		(unsigned char)((newId.Data3) & 0xFF),

		(unsigned char)newId.Data4[0],
		(unsigned char)newId.Data4[1],
		(unsigned char)newId.Data4[2],
		(unsigned char)newId.Data4[3],
		(unsigned char)newId.Data4[4],
		(unsigned char)newId.Data4[5],
		(unsigned char)newId.Data4[6],
		(unsigned char)newId.Data4[7]
	};

	return Guid{std::move(bytes)};
}
#endif

// android version that uses a call to a java api
#ifdef GUID_ANDROID
Guid newGuid(JNIEnv *env)
{
	assert(env != androidInfo.env || std::this_thread::get_id() == androidInfo.initThreadId);

	jobject javaUuid = env->CallStaticObjectMethod(
		androidInfo.uuidClass, androidInfo.newGuidMethod);
	jlong mostSignificant = env->CallLongMethod(javaUuid,
		androidInfo.mostSignificantBitsMethod);
	jlong leastSignificant = env->CallLongMethod(javaUuid,
		androidInfo.leastSignificantBitsMethod);

	std::array<unsigned char, 16> bytes =
	{
		(unsigned char)((mostSignificant >> 56) & 0xFF),
		(unsigned char)((mostSignificant >> 48) & 0xFF),
		(unsigned char)((mostSignificant >> 40) & 0xFF),
		(unsigned char)((mostSignificant >> 32) & 0xFF),
		(unsigned char)((mostSignificant >> 24) & 0xFF),
		(unsigned char)((mostSignificant >> 16) & 0xFF),
		(unsigned char)((mostSignificant >> 8) & 0xFF),
		(unsigned char)((mostSignificant) & 0xFF),
		(unsigned char)((leastSignificant >> 56) & 0xFF),
		(unsigned char)((leastSignificant >> 48) & 0xFF),
		(unsigned char)((leastSignificant >> 40) & 0xFF),
		(unsigned char)((leastSignificant >> 32) & 0xFF),
		(unsigned char)((leastSignificant >> 24) & 0xFF),
		(unsigned char)((leastSignificant >> 16) & 0xFF),
		(unsigned char)((leastSignificant >> 8) & 0xFF),
		(unsigned char)((leastSignificant) & 0xFF)
	};

	env->DeleteLocalRef(javaUuid);

	return Guid{std::move(bytes)};
}

Guid newGuid()
{
	return newGuid(androidInfo.env);
}
#endif


END_XG_NAMESPACE
