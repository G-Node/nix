# File handling

The *File* entity encapsulates all information of a dataset. *File*
entities keep some general information such as creation time and date, format
version etc. The entry-point to a *NIX* file.

## File modes

A *NIX* file can be opened in one of three modes:

1. **Overwrite** Used for creating a new file or overwriting any content in an
   existing one!
2. **ReadWrite** Opens a file if it exists or otherwise creates a new one. Does
   **not** overwrite existing content.
3. **ReadOnly** Opens an existing file. The content can not be changed. Will
   raise an error when the provided path is invalid (e.g. the file doesn't exist).

## Creating a new file.

```c++
#include <nix.hpp>

int main() {
    std::string file_path = "./test.nix";
    nix::File f = nix.File.open(file_path, nix::FileMode::Overwrite);
    std::cout << f << std::endl;
    f.close();
    return 0;
}
```

Alternatively, the ``nix::FileMode::ReadWrite`` flag can be used.

## Open existing files.
Use ``nix::FileMode::ReadWrite`` or ``nix::FileMode::ReadOnly`` to
open an existing file to work with the data. If you want to overwrite
*any* existing content in order to replace the file use
``nix::FileMode::Overwite``.

Trying to open an non-existing file in *ReadOnly* mode will lead to an error!

## Selecting a storage backend
So far, the *NIX* data model is only fully supported
using [HDF5](https://www.hdfgroup.org). The library design, though, is
meant for multiple backend support. We have an **experimental**, not
fully implemented file-system backend, that stores data in a folder
structure.

The desired backend must be selected upon opening of a file:
```c++
nix::File f = nix::File::open("test.nix", nix::FileMode::Overwrite, "hdf5");
```

**HDF5 is the default backend.**

## Enabling compression

By default, data stored inside a *NIX* file will not be compressed. You
can switch on compression during file opening.

```c++
nix::File f = nix::File::open("test.nix", nix::FileMode::Overwrite, "hdf5", nix::Compression::DeflateNormal);
```

Compression is handled transparently by the hdf5-library, no further
user interaction is required. Compression reduces reading and writing
performance. You can choose to switch off compression on individual
*DataArray*s by passing the ``nix::Compression::None`` flag when
creating them.


[home](./index.md) -- [back](./getting_started.md)
