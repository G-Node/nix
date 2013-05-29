#include <pandora/Selection.hpp>

namespace pandora {

void Selection::offset(const PSSize &offset)
{
  space.offsetSimple(offset.data());
}

void Selection::select(const PSize &count, const PSize &start, Mode mode)
{
  H5S_seloper_t op = static_cast<H5S_seloper_t>(mode);
  space.selectHyperslab(op, count.data(), start.data());
}

void Selection::select(Preset set)
{

  switch(set) {
    case Preset::None:
        space.selectNone();
        return;
    case Preset::All:
        space.selectAll();
        return;
  }
}

PSize Selection::size() const
{
  size_t rank = this->rank();
  PSize start(rank), end(rank);
  bounds(start, end);
  return (end - start) + 1;
}

} //namespace pandora