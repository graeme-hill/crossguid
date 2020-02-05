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

#pragma once

#ifdef GUID_ANDROID
#include <thread>
#include <jni.h>
#endif

#include <array>
#include <iomanip>
#include <ostream>
#include <sstream>
#include <string_view>

#define BEGIN_XG_NAMESPACE namespace xg {
#define END_XG_NAMESPACE }

// forward decl
BEGIN_XG_NAMESPACE
class Guid;
END_XG_NAMESPACE

inline std::ostream &operator<<(std::ostream &s, const xg::Guid &guid);

BEGIN_XG_NAMESPACE

// Class to represent a GUID/UUID. Each instance acts as a wrapper around a
// 16 byte value that can be passed around by value. It also supports
// conversion to string (via the stream operator <<) and conversion from a
// string via constructor.
class Guid
{
public:
	// create a guid from vector of bytes
	explicit constexpr Guid(const std::array<unsigned char, 16> &bytes)
		: _bytes{ bytes } {}
	// create a guid from vector of bytes
	explicit constexpr Guid(std::array<unsigned char, 16> &&bytes)
		: _bytes{ std::move(bytes) } {}

	// create a guid from string
	explicit constexpr Guid(std::string_view fromString);

	// create empty guid
	constexpr Guid()
		: _bytes{ {0} } {}

	constexpr Guid(const Guid &other) = default;
	constexpr Guid &operator=(const Guid &other) = default;
	constexpr Guid(Guid &&other) = default;
	constexpr Guid &operator=(Guid &&other) = default;

	// overload equality operator
	constexpr bool operator==(const Guid &other) const
	{
		// Hopefully the optimizer can be smart about this :))))
		for (std::size_t i = 0; i < _bytes.size(); i += 1) {
			if (_bytes[i] != other._bytes[i])
				return false;
		}
		return true;
	}
	// overload inequality operator
	constexpr bool operator!=(const Guid &other) const { return !(*this == other); }

	// convert to string using std::snprintf() and std::string
	std::string str() const
	{
		std::stringstream stream;
		stream << *this;
		return stream.str();
	}
	// conversion operator for std::string
	operator std::string() const { return str(); }
	// Access underlying bytes
	constexpr const std::array<unsigned char, 16>& bytes() const { return _bytes; }
	// member swap function
	void swap(Guid &other) { _bytes.swap(other._bytes); }
	constexpr bool isValid() const
	{
		Guid empty;
		return *this != empty;
	}

private:
	// set all bytes to zero
	constexpr void zeroify()
	{
		for (std::size_t i = 0; i < _bytes.size(); i += 1) {
			_bytes[i] = 0;
		}
	}

	// actual data
	std::array<unsigned char, 16> _bytes{ {0} };

	friend bool operator<(const Guid &lhs, const Guid &rhs)
	{
		return lhs.bytes() <	rhs.bytes();
	}
};

Guid newGuid();

#ifdef GUID_ANDROID
struct AndroidGuidInfo
{
	static AndroidGuidInfo fromJniEnv(JNIEnv *env);

	JNIEnv *env;
	jclass uuidClass;
	jmethodID newGuidMethod;
	jmethodID mostSignificantBitsMethod;
	jmethodID leastSignificantBitsMethod;
	std::thread::id initThreadId;
};

extern AndroidGuidInfo androidInfo;

void initJni(JNIEnv *env);

// overloading for multi-threaded calls
Guid newGuid(JNIEnv *env);
#endif

namespace details
{
	template <typename...> struct hash;

	template<typename T> 
	struct hash<T> : public std::hash<T>
	{
		using std::hash<T>::hash;
	};


	template <typename T, typename... Rest>
	struct hash<T, Rest...>
	{
		inline std::size_t operator()(const T& v, const Rest&... rest) {
			std::size_t seed = hash<Rest...>{}(rest...);
			seed ^= hash<T>{}(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
			return seed;
		}
	};
}

// converts a single hex char to a number (0 - 15)
constexpr unsigned char hexDigitToChar(char ch)
{
	// 0-9
	if (ch > 47 && ch < 58)
		return ch - 48;

	// a-f
	if (ch > 96 && ch < 103)
		return ch - 87;

	// A-F
	if (ch > 64 && ch < 71)
		return ch - 55;

	return 0;
}

constexpr bool isValidHexChar(char ch)
{
	// 0-9
	if (ch > 47 && ch < 58)
		return true;

	// a-f
	if (ch > 96 && ch < 103)
		return true;

	// A-F
	if (ch > 64 && ch < 71)
		return true;

	return false;
}

// converts the two hexadecimal characters to an unsigned char (a byte)
constexpr unsigned char hexPairToChar(char a, char b)
{
	return hexDigitToChar(a) * 16 + hexDigitToChar(b);
}

constexpr Guid::Guid(std::string_view fromString) {
	char charOne = '\0';
	char charTwo = '\0';
	bool lookingForFirstChar = true;
	unsigned nextByte = 0;

	for (const char &ch : fromString)
	{
		if (ch == '-')
			continue;

		if (nextByte >= 16 || !isValidHexChar(ch))
		{
			// Invalid string so bail
			zeroify();
			return;
		}

		if (lookingForFirstChar)
		{
			charOne = ch;
			lookingForFirstChar = false;
		}
		else
		{
			charTwo = ch;
			auto byte = hexPairToChar(charOne, charTwo);
			_bytes[nextByte++] = byte;
			lookingForFirstChar = true;
		}
	}

	// if there were fewer than 16 bytes in the string then guid is bad
	if (nextByte < 16)
	{
		zeroify();
		return;
	}
}

END_XG_NAMESPACE

namespace std
{
	// Specialization for std::hash<Guid> -- this implementation
	// uses std::hash<std::string> on the stringification of the guid
	// to calculate the hash
	template <>
	struct hash<xg::Guid>
	{
		std::size_t operator()(xg::Guid const &guid) const
		{
			const uint64_t* p = reinterpret_cast<const uint64_t*>(guid.bytes().data());
			return xg::details::hash<uint64_t, uint64_t>{}(p[0], p[1]);
		}
	};
}

// overload << so that it's easy to convert to a string
inline std::ostream &operator<<(std::ostream &s, const xg::Guid &guid)
{
	auto& bytes = guid.bytes();
	std::ios_base::fmtflags f(s.flags()); // politely don't leave the ostream in hex mode
	s << std::hex << std::setfill('0')
		<< std::setw(2) << (int)bytes[0]
		<< std::setw(2) << (int)bytes[1]
		<< std::setw(2) << (int)bytes[2]
		<< std::setw(2) << (int)bytes[3]
		<< "-"
		<< std::setw(2) << (int)bytes[4]
		<< std::setw(2) << (int)bytes[5]
		<< "-"
		<< std::setw(2) << (int)bytes[6]
		<< std::setw(2) << (int)bytes[7]
		<< "-"
		<< std::setw(2) << (int)bytes[8]
		<< std::setw(2) << (int)bytes[9]
		<< "-"
		<< std::setw(2) << (int)bytes[10]
		<< std::setw(2) << (int)bytes[11]
		<< std::setw(2) << (int)bytes[12]
		<< std::setw(2) << (int)bytes[13]
		<< std::setw(2) << (int)bytes[14]
		<< std::setw(2) << (int)bytes[15];
	s.flags(f);
	return s;
}
