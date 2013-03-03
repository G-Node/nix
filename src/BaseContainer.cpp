#include "BaseContainer.hpp"

using namespace pandora;

bool BaseContainer::attrExists(std::string name)
{
  return H5Aexists(h5group.getId(), name.c_str());
}

bool BaseContainer::objectExists(std::string path)
{
  htri_t ret = H5Lexists(h5group.getId(), path.c_str(), H5P_DEFAULT);
  if (!ret)
    return false;
  
  ret = H5Oexists_by_name(h5group.getId(), path.c_str(), H5P_DEFAULT);
  return ret;
}
