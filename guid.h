#include <uuid/uuid.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

// Class to represent a GUID/UUID. Each instance acts as a wrapper around a
// 16 byte value that can be passed around by value. It also supports
// conversion to string (via the stream operator <<) and conversion from a
// string via constructor.
class Guid
{
  public:

    // create a guid from vector of bytes
    Guid(const vector<unsigned char> &bytes);

    // create a guid from array of bytes
    Guid(const unsigned char *bytes);

    // create a guid from string
    Guid(const string &fromString);

    // create empty guid
    Guid();

    // copy constructor
    Guid(const Guid &other);

    // overload assignment operator
    Guid &operator=(const Guid &other);

    // overload equality and inequality operator
    bool operator==(const Guid &other) const;
    bool operator!=(const Guid &other) const;

  private:

    // actual data
    vector<unsigned char> _bytes;

    // make the << operator a friend so it can access _bytes
    friend ostream &operator<<(ostream &s, const Guid &guid);
};

// create a new unique identifier (implementation is system specific)
Guid generateGuid();

