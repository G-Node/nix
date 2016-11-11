#include <iostream>
#include <vector>
#include <stdexcept>
#include <initializer_list>
#include <nix/nixversion.hpp>

#include <nix/Platform.hpp>

#ifndef NIX_VERSION_HPP
#define NIX_VERSION_HPP

namespace nix {
/**
 * @brief Returns the version of the library.
 * 
 * @return the version as a vector<int> in the order major, minor, patch
 */
NIXAPI std::vector<int> apiVersion();

    
class FormatVersion {
public:

  FormatVersion(std::initializer_list<int> l)
    : FormatVersion(std::vector<int>(l)) { }

  explicit FormatVersion(const std::vector<int> &v) {
    if (v.size() != 3) {
      throw std::runtime_error("version vector with != 3 elements");
    }

    vx = v[0];
    vy = v[1];
    vz = v[2];

  };

  int x() const { return vx; }
  int y() const { return vy; }
  int z() const { return vz; }

  // helper functions
  bool canWrite(const FormatVersion &thefile) const {
    // to be able to write, we need to have an
    // exact match
    return *this == thefile;
  }

  bool canRead(const FormatVersion &thefile) const {
    // we are the library, means we must have an
    // exact match for x and, be the same or newer
    // in y
    return vx == thefile.x() && vy >= thefile.y();
  }

  // operators

  int operator[](const size_t index) const {
    switch (index) {
    case 0: return vx;
    case 1: return vy;
    case 2: return vz;
    default: throw std::out_of_range("index out of range for version");
    }
  }

  bool operator==(const FormatVersion &o) const {
    return o.vx == vx && o.vy == vy && o.vz == vz;
  }


  bool operator<(const FormatVersion &b) const {
    //a < b
    for (size_t i = 0; i < 3; i++) {
      int aa = this->operator[](i);
      int bb = b[i];

      if (aa < bb) {
	return true;
      } else if (bb < aa) {
	return false;
      }

    }

    // here: a[i] == b[i] for i in [1,2,3]
    return false;
  }

  //

  bool operator!=(const FormatVersion &o) const {return !this->operator==(o);}
  bool operator> (const FormatVersion &b) const {return  b.operator< (*this);}
  bool operator<=(const FormatVersion &b) const {return !this->operator> (b);}
  bool operator>=(const FormatVersion &b) const {return !this->operator< (b);}

private:
  int vx, vy, vz;
};

inline std::ostream& operator<<(std::ostream& out, const FormatVersion& v)
{
  return out << v.x() << "." << v.y() << "." << v.z();
}

} //namespace nix::

#endif //NIX_VERSION_HPP
