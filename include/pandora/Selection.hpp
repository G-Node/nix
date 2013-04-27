#ifndef PANDORA_SELECTION_H
#define PANDORA_SELECTION_H


#include <H5Cpp.h>
#include <pandora/PSize.hpp>
#include <pandora/Charon.hpp>

namespace pandora {

class Selection {
public:
  enum class Preset {
    None = 0,
    All  = 1,
  };

  enum class Mode {
    Set  = H5S_SELECT_SET,
    Or   = H5S_SELECT_OR,
    And  = H5S_SELECT_AND,
    Xor  = H5S_SELECT_XOR,
    NotA = H5S_SELECT_NOTA,
    NotB = H5S_SELECT_NOTB
  };

  Selection() {}
  Selection(const H5::DataSpace &ds) : space(ds) {}
  Selection(const Selection &sel) : space(sel.space) {}
  template<typename T> Selection (const T &value);

  Selection& operator=(const Selection &other) { space = other.space; return *this; }

  void select(const PSize &count, const PSize &start, Mode mode = Mode::Set);
  void select(Preset set);
  void offset(const PSSize &offset);

  H5::DataSpace& h5space() { return space; }
  const H5::DataSpace& h5space() const { return space; }
  bool isValid() const { return space.selectValid(); }
  void bounds(PSize &start, PSize &end) const { space.getSelectBounds(&start[0], &end[0]); }
  PSize size() const;
  size_t rank() const {return static_cast<hsize_t>(space.getSimpleExtentNdims()); }

private:
  H5::DataSpace space;
};

template<typename T>
Selection::Selection (const T &value)
{
  Charon<const T> charon(value);
  space = charon.createDataSpace(true);
}

} //namespace

#endif //PANDORA_SELECTION_H