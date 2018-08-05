## Annotating datasets

Storing data is one thing. Providing essential information about the
data is the other. In order to allow later reuse of once-acquired data
and to achieve reproducibility of scientific results we also need the
means to of storing sufficient metadata.

In the *NIX* [data model](./data_model.md) we allow annotation of data
on several levels. It is possible to store global information, for
example about the experimental session, and also to annotate the data
itself. In the extreme it would be possible to annotate individual
data points.

We implemented the (*odml*)[http://g-node.github.io/python-odml] data
model for metadata for this purpose. In brief, metadata are stored as
extended key-value pairs that are grouped. Accordingly, three entities
are defined: *Section*, *Property*, and *Value*. *Sections* can
contain *Properties* **and** sub*Sections*. Thus, they represent a
tree structure. *Properties* in turn can store several *Values*.

Almost all other *NIX* entities can link to *Sections* indicating that
the metadata stored in the linked *Section* (and its sub*Sections*)
relates to this entity. The following example shows how metadata can
be added.

```c++
#include <nix.hpp>

int main() {
    // create a nix file
    nix::File f = nix::File::open("metadata_demo.nix", nix::FileMode::Overwrite);

    nix::Section global_mdata = f.createSection("session info", "recording");
    global_mdata.repository("https://portal.g-node.org/odml/terminologies/v1.0/");

    // add a simple string property
    nix::Value author("John Doe");
    global_mdata.createProperty("author name", author);

    // add a Property with a numeric value that has a unit and an estimation uncertainty
    nix::Value temp(21.5);
    temp.uncertainty(0.5);
    temp.unit("K"); // using degree sign is tricky, unicode symbols are not supported... sorry
    nix::Property p = global_mdata.createProperty("ambient temperature", temp);

    return 0;
}

```


[home](./index.md) -- [back](./getting_started.md)
