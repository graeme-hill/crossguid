#include "guid.h"

#ifdef GUID_LIBUUID
#include <uuid/uuid.h>
#endif

#ifdef GUID_WINDOWS
#include <objbase.h>
#endif

using namespace std;

// overload << so that it's easy to convert to a string
ostream &operator<<(ostream &s, const Guid &guid)
{
  return s << hex << setfill('0')
    << setw(2) << (int)guid._bytes[0]
    << setw(2) << (int)guid._bytes[1]
    << setw(2) << (int)guid._bytes[2]
    << setw(2) << (int)guid._bytes[3]
    << "-"
    << setw(2) << (int)guid._bytes[4]
    << setw(2) << (int)guid._bytes[5]
    << "-"
    << setw(2) << (int)guid._bytes[6]
    << setw(2) << (int)guid._bytes[7]
    << "-"
    << setw(2) << (int)guid._bytes[8]
    << setw(2) << (int)guid._bytes[9]
    << "-"
    << setw(2) << (int)guid._bytes[10]
    << setw(2) << (int)guid._bytes[11]
    << setw(2) << (int)guid._bytes[12]
    << setw(2) << (int)guid._bytes[13]
    << setw(2) << (int)guid._bytes[14]
    << setw(2) << (int)guid._bytes[15];
}

// create a guid from vector of bytes
Guid::Guid(const vector<unsigned char> &bytes)
{
  _bytes = bytes;
}

// create a guid from array of bytes
Guid::Guid(const unsigned char *bytes)
{
  _bytes.assign(bytes, bytes + 16);
}

// create a guid from string
Guid::Guid(const string &fromString)
{
  _bytes.clear();
  char byteHexString[3] = {0};

  bool lookingForFirstChar = true;

  for (const char &ch : fromString)
  {
    if (ch == '-')
      continue;

    if (lookingForFirstChar)
    {
      byteHexString[0] = ch;
      lookingForFirstChar = false;
    }
    else
    {
      byteHexString[1] = ch;
      unsigned long byte;
      byte = stoul(byteHexString, 0, 16);
      _bytes.push_back(byte);
      lookingForFirstChar = true;
    }
  }
  
}

// create empty guid
Guid::Guid()
{
  _bytes = vector<unsigned char>(16, 0);
}

// copy constructor
Guid::Guid(const Guid &other)
{
  _bytes = other._bytes;
}

// overload assignment operator
Guid &Guid::operator=(const Guid &other)
{
  _bytes = other._bytes;
  return *this;
}

// overload equality operator
bool Guid::operator==(const Guid &other) const
{
  return _bytes == other._bytes;
}

// overload inequality operator
bool Guid::operator!=(const Guid &other) const
{
  return !((*this) == other);
}

#ifdef GUID_LIBUUID
Guid generateGuid()
{
  uuid_t id;
  uuid_generate(id);
  return id;
}
#endif

#ifdef GUID_WINDOWS
Guid generateGuid()
{
	GUID newId;
	CoCreateGuid(&newId);

	const unsigned char bytes[16] = 
	{
		(newId.Data1 >> 24) & 0xff,
		(newId.Data1 >> 16) & 0xff,
		(newId.Data1 >> 8) & 0xff,
		(newId.Data1) & 0xff,

		(newId.Data2 >> 8) & 0xff,
		(newId.Data2) & 0xff,

		(newId.Data3 >> 8) & 0xff,
		(newId.Data3) & 0xff,

		newId.Data4[0],
		newId.Data4[1],
		newId.Data4[2],
		newId.Data4[3],
		newId.Data4[4],
		newId.Data4[5],
		newId.Data4[6],
		newId.Data4[7]
	};

	return bytes;
}
#endif
