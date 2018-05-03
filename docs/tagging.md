# Tagging points and regions

The *DataArray*s store data, but this is not all that is needed to
store scientific data. We may want to highlight points or regions in
the data and link it to further information.

This is done using the *Tag* and the *MultiTag* for tagging single or
mutliple points or regions, respectively.


## Single points/regions

### Tagging in 1D

Suppose, we are recording the response of a system to a certain
stimulus.

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
    std::iota(response.begin(), response.end(), 0.);
    std::transform(response.begin(), response.end(), response.begin(),
                   [interval](double x){ return x * interval; });
    std::transform(response.begin(), response.end(), response.begin(),
                   [stim_on, stim_off](double x){ return (x >= stim_on && x < stim_off) ? 1 : 0; });

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


#### Handling of units
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

### Tagging in 2D

The same principle shown above for 1-D data extends to two or more
dimensions. The only difference is that one has to provide vectors of
positions/extents that match in length to the number of dimensions of
the referenced *DataArray*.

![2d_tag_plot](./images/2d_tag.png "tagging in 2-D")

```c++
#include <nix.hpp>
#include <nix/hydra/multiArray.hpp>

int main() {
    // create 2D dummy data
    typedef boost::multi_array<double, 2> array_type;
    typedef array_type::index index;
    array_type data(boost::extents[100][100]);

    int values = 0;
    for(index i = 0; i < 100; ++i)
        for(index j = 0; j < 100; ++j)
            data[i][j] = values++;

    // create a new file, enable compression
    nix::File f = nix::File::open("2_d_tag.nix", nix::FileMode::Overwrite, "hdf5", nix::Compression::DeflateNormal);
    nix::Block b = f.createBlock("demo block", "nix.demo");

    nix::DataArray array = b.createDataArray("2D data", "nix.sampled.2d", data);
    SampledDimension dim = array.appendSampledDimension(1.);
    dim.label("width");
    dim.unit("mm");
    dim = array.appendSampledDimension(1.);
    dim.label("height");
    dim.unit("mm");

    nix::Tag tag = b.createTag("tagged region", "nix.region", {40., 80.});
    tag.extent({50., -40.});
    tag.addReference(array);
    return 0;
}
```

## Multiple points/regions

### Tagging multiple points in 1D

Consider the following situation: A signal has been recorded and
within this signal, certain events have been detected (figure below).

![multiple_points_plot](./images/multiple_points.png "multiple events")

For storing this kind of data we need two *DataArrays*, the first
stores the recorded signal, the other the events. Finally, a
*MultiTag* entity is used to link both. One can use the event times
stored in one of the *DataArrays* to tag multiple points in the
signal.

```c++
#include <nix.hpp>
#include <numeric>

int main() {
    // create dummy data
    std::vector<double> time(1000);
    std::vector<double> voltage;
    std::vector<double> threshold_crossings;
    double threshold = 0.5;
    double interval = 0.001;
    double pi = 3.1415;
    double freq = 5.;

    std::iota(time.begin(), time.end(), 0.);
    std::transform(time.begin(), time.end(), time.begin(),
                   [interval](double t){ return t * interval; });
    std::transform(time.begin(), time.end(), std::back_inserter(voltage),
                   [pi, freq](double t){ return std::sin(t * freq * 2 * pi) + std::sin(t * freq * 4 * pi) * 0.4; });
    for (size_t i = 0; i < voltage.size() - 1; ++i) {
        if (voltage[i] <= threshold && voltage[i+1] > threshold) {
            threshold_crossings.push_back(time[i]);
        }
    }

    // open a file, create a block that will host the data
    nix::File f = nix::File::open("mtag_test.nix", nix::FileMode::Overwrite, "hdf5",
                                  nix::Compression::DeflateNormal);
    nix::Block b = f.createBlock("demo block", "demo");

    // create two DataArrays, one for the signal, the other one for the events
    nix::DataArray signal = b.createDataArray("signal", "nix.sampled", voltage);
    signal.label("voltage");
    signal.unit("mV");

    nix::SampledDimension dim = signal.appendSampledDimension(interval);
    dim.label("time");
    dim.unit("s");

    nix::DataArray events = b.createDataArray("threshold crossings", "nix.event_times", threshold_crossings);
    events.label("time");
    events.unit("s");

    events.appendAliasRangeDimension();

    // create the MultiTag entity to link signal and events
    nix::MultiTag mtag = b.createMultiTag("event tag", "nix.event_tag", events);
    mtag.addReference(signal);

    f.close();
    return 0;
}
```

Creating the *MultiTag* is very similar to the creation of the simpler
*Tag* above. The main difference is that the tagged positions are not
stored in the *MultiTag* itself but we use the event *DataArray*
(events in the code example) for this purpose. Finally, the signal
*DataArray* is added to the list of references.


### Tagging multiple regions in 1D

In the following exampled we want to plot multiple regions, in which ,
for example a stimulus was switched on.

![multiple_regions_plot](./images/multiple_regions.png "multiple regions")

For storing such data we again need one *DataArray* to store the
recorded signal. Storing the regions is similar to the approach for
the simpler *Tag*. We store the *positions* and the
**extents**. Accordingly, **two** additional *DataArray*s are
needed. The first of which stores the positions and the second the
extents.

```c++


```
## Adding features


## Retrieving tagged data



[home](./index.md "g-node.github.io/nix") -- [back](./getting_started.md "NIX Introduction")
