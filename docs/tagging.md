# Tagging Points and regions in recorded data

The *DataArray*s store data, but this is not all that is needed to
store scientific data. Suppose, we are recording the response of a
system to a certain stimulus.

![simple_tag_plot](./images/tag1.png "a system's response to a stimulus")

We may want to store:

1. The recorded system's response before, during, and after the stimulus.
2. The time, the stimulus was on.

In the *NIX* data model *Tag* entities are used to tag regions in a
*DataArray*. For this the *Tag* refers to the *DataArray* and
stores **position** and **extent** of the highlighted segment.

![simple_tag_plot](./images/tag2.png "a system's response to a stimulus")

```c++
#include <nix.hpp>
#include <numeric>

int main() {
    double interval = 0.01;
    double stim_on = 0.5;
    double stim_off = 2.5;
    std::vector<double> response(350);
    std::iota(response.begin(), response.end(), interval);
    std::transform(response.begin(), response.end(), response.begin(),
                   [stim_on, stim_off](double x){return (x >= stim_on && x < stim_off) ? 1 : 0;});

    nix::File f = nix::File::open("tagging1.nix", nix::FileMode::Overwrite);
    nix::Block block = f.createBlock("demo block", "nix.demo");

    nix::DataArray data = block.createDataArray("response", "nix.sampled", response);
    data.label("voltage");
    data.unit("mV");

    nix::SampledDimension dim = data.appendSampledDimension(interval);
    dim.label("time");
    dim.unit("s");

    nix::Tag stim_tag = block.createTag("Stimulus", "nix.stimulus_segment", {stim_on});
    stim_tag.extent({stim_off - stim_on});
    stim_tag.addReference(data);

    return 0;
}
```

**Note!** Position and extent of the tagged region are given as
``std::vector``. Extent is optional, not setting it tags a point not
a segment of the referenced data.


### Handling of units
In the above example we do not explicitly specify a unit for position
and extent. They are implicitly assumed to be given in the same unit
as the *DataArray*. It is, however, possible to specify a unit (or
rather a unit for each dimension), if this is more convenient. For
example, it would be possible to give position and extent in *ms*. The
*NIX* does not validate at this point. It is user responsibility that
units match. In order to work smoothly, it is best to use only SI
units and compounds of these.

```c++
    nix::Tag stim_tag = block.createTag("Stimulus", "nix.stimulus_segment", {stim_on});
    stim_tag.extent({stim_off - stim_on});
    stim_tag.units({"ms"});
```

The same principle shown here for 1-D data extends to two or more
dimensions. The only difference is that one has to provide vectors of
positions/extents that match in length to the number of dimensions of
the referenced *DataArray*.


[home](./index.md) -- [back](./getting_started.md)
