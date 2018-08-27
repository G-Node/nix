# Finding things

It might be hard to make sense of the contents of a *NIX* file that
has been written by another person or has been automatically created
by a recording tool such
as [relacs](https://github.com/relacs/relacs). **Name** and **Type**
fields of the entities should help the human reader to find their ways
through the file and make sense of the stored content and the
relations between entities.


## Recommendations when opening an unknown NIX file

1. Find out how many *Blocks* are stored and try to understand what
   they stand for. In most cases there will be only one *Block* and in
   the [relacs](https://github.com/relacs/relacs) use-case it will
   represent the recordings made on a single neuron.
2. Within the *Block* look for *Group* entities. As indicated by the
   name, these group other entities and thus indicate some kind of
   common relation between the members of each group.
3. Within *Group*s or, if there are no *Group*s, within the *Block*
   look out for *Tag*s and *MultiTag*s. These link between data
   carrying *DataArrays* and provide semantic background.
4. For example, a *MultiTag* that has a type "stimulus_segment" quite
   clearly indicates that it highlights periods during which a
   stimulus was presented.
5. For *Tag*s and *MultiTag*s take a look at the **references** which
   list all *DataArrays* that are tagged. One may further investigate
   the **features** which should provide more information on the
   tagged regions.
6. Almost all entities can link to additional **metadata** that should
   contain information about the stored data.

## Standardization helps
As mentioned [before](./standardization.md), if the file is created
according to known definitions, finding stuff in an unknown file is
considerably simplified.

For example, in case one looks for the voltage recording made on the
first channel and it is further known that the file is created
e.g. by [relacs](https://github.com/relacs/relacs) then one needs to
look for "nix.data.sampled.V-1" typed *DataArray*s:

```c++
nix::File f = nix::File::open("relacs_demo.nix", nix::FileMode::ReadOnly);
nix::Block b = f.blocks()[0];
std::vector<DataArray> arrays = b.dataArrays(nix::util::TypeFilter<DataArray>("nix.data.sampled.V-1"));
```

The ``nix::util`` namespace defines various filters for:
* IDs
* names
* types
* linked metadata
* linked sources
* and the default filter that accepts all.

Custom filters can be defined by inheriting from
``nix::util::Filter``.

The above said assumes a "data-centered" view. The other way would be
to assume a "metadata-centered" view and to scan the metadata tree and
perform an inverse search from the metadata to the data entities that
refer to the respective section. Accordingly, ``nix::Section`` defines
methods to get the referring *DataArray*s, *Tag*s, etc..

```c++
nix::File f = nix::File::open("relacs_demo.nix", nix::FileMode::ReadOnly);
nix::Section s = f.sections()[0];
std::vector<nix::Block> blocks = s.referringBlocks();
for (auto b : blocks) {
    std::cerr << b;
}
```


[home](./index.md) -- [back](./getting_started.md)
