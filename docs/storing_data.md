# Storing data

1. We want to be able to store **n-dimensional** data structures.
2. The data structures must be **self-explanatory**, that is, it must
  contain sufficient information to draw a basic plot of the data.

![sampled_plot](./images/regular_sampled.png "simple plot")
Considering the simple plot above, we can list all information that it shows, or, the other way round, what needs to be stored.

* the data (voltage measurements)
* the y-axis labeling, i.e. label (voltage) and unit (mV)
* the x-axis labeling, i.e. label (time) and unit (s)
* the x-positions for each data point
* a title/legend

In this, and in most cases, it would be inefficient to store x-, and
y-positions of each data point. The voltage measurements have been
done in regular (time) intervals. Thus, we rather need to store the
measured values and a definition of the x-axis consisting of offset,
sampling interval, label, and unit.

This is exactly the approach chosen in NIX. For each dimension of the
data a **dimension descriptor** must be given. In NIX we define three
(and a half) dimension descriptors:

1. *SampledDimension*: Used if a dimension is sampled in regular intervals.
2. *RangeDimension*: Used if the dimension is sampled in irregular
   intervals. There is a special case, the *AliasRangeDimension* which
   will be discussed below.
3. *SetDimension*: Used for dimensions, that represent categories rather than physical entities.


## The DataArray

The *DataArray* is the most central entity of the NIX data model. As
almost all other NIX-entities it requires a *name* and a *type*. Both
are not restricted but names must be **unique** inside a
*Block*. *type* information can be used to introduce some semantic
meaning and domain-specificity. Upon creation the *DataArray* will
have a unique ID.

The *DataArray* stores the actual data, together with label and
unit. In addition the *DataArray* needs a dimension descriptor for
each dimension. The following snippet shows a simple way how to create
a *DataArray* and store some dummy data in it.

```c++
#include <nix.hpp>

int main() {
    // create some dummy data
    std::vector<double> time(1000);
    std::vector<double> voltage;
    double interval = 0.01;
    double pi = 3.1415;
    double freq = 1.5;
    std::iota(time.begin(), time.end(), interval);
    std::transform(time.begin(), time.end(), std::back_inserter(voltage),
                   [pi, freq](double t) { return std::sin(t * freq * 2 * pi); });

    // open a file, create a block that will host the data
    nix::File f = nix::File::open("test.nix", nix::FileMode::Overwrite, "hdf5",
                                  nix::Compression::DeflateNormal);
    nix::Block b = f.createBlock("demo block", "demo");

    // create a DataArray
    nix::DataArray array = b.createDataArray("sinewave", "nix.sampled", voltage);
    array.label("voltage");
    array.unit("mV");
    nix::SampledDimension dim = array.appendSampledDimension(interval);
    dim.label("time");
    dim.unit("s");

    f.close();
    return 0;
}
```

In the example shown above, the NIX library will figure out the
dimensionality of the data, the correct size of the data and its data
type. The data type and the dimensionality are fixed, the actual size
of the *DataArray* can be changed.

In case you need more control, *DataArrays* can be created empty for
later filling e.g. during data acquisition.

```c++
nix::DataArray array = block.createDataArray("name", "type", nix::DataType::Double, {100});
```

The resulting *DataArray* will have an initial size (100 elements)
which will be automatically resized, if required. NIX will further try
to convert passed data to the defined data type, if possible.

Data can be set with a call like this:
``array.setData(voltage);``

Writing/Replacing subsets can be done by providing the *count* and the
*offset* of the data chunk.


# Dimensions

We can store n-dimensional data in *DataArrays* and for each dimension
of the data we must provide an **dimension descriptor*. The following
introduces the individual descriptors.

## SampledDimension

![sampled_plot](./images/regular_sampled.png "simple plot")

The same situation as before, the data has been sampled in regular
intervals. That is, the time between successive data points is always
the same. The x-axis can be fully described by only a few parameters:

1. sampling interval
2. offset
3. label
4. unit

The *SampledDimension* entity is used in such situations and needs to
be added to the *DataArray* entity when it is created:

```c++
nix::DataArray array = block.createDataArray("name", "nix.sampled", voltages);
array.label("voltage");
array.unit("mV");

nix::SampledDimension dim = array.appendSampledDimension(sampling_interval);
dim.label("time");
dim.unit("s");
dim.offset(0.0);   // not needed, it is 0.0 by default
```

**Why sampling interval, not sampling rate?** Because the interval is
the more **general** term, it can also be applied to dimensions that
do not extend in time but, for example space.


## RangeDimension
![range_plot](./images/irregular.png "another simple plot")

Similar situation as before, but this time the temporal distance
between the sampled voltages is not regular. Storing this kind of data
is not as efficient as in the regularly sampled case. The following
information needs to be stored to describe the dimension:

1. the x-positions of the data point, i.e. *ticks*
2. the label
3. the unit

In this kind of dimension we store a *range* of ticks, therefore the
name **RangeDimension**. It needs to be added to the **DataArray**
when it is created.

```c++
std::vector<double> ticks = {1.2, 2.7, 3.4, 4.0, 5.1};
nix::DataArray array = block.createDataArray("name", "nix.irregular_sampled", voltages);
array.label("voltage");
array.unit("mV");

nix::RangeDimension dim = array.appendRangeDimension(ticks);
dim.label("time");
dim.unit("s");
```

The *ticks* of a **RangeDimension** must be numeric and ascending.


### AliasRangeDimension

A special case of a **RangeDimension** that is used when storing something equivalent to event times.

![alias_range_plot](./images/alias_range.png "simple plot marking events")

In the plot above, each dot marks the occurrence of an event. In such
a case it is basically the x-values that are of interest. It would be
most inefficient to store them twice, 1st as values and 2nd as ticks
in the dimension descriptor.

The **AliasRangeDimension** is used in such situations, internally it
is a **RangeDimension** whose information is tied to the information
stored in the **DataArray** itself. Changing the ticks, label or unit
of the dimension descriptor will change the **DataArray** itself. Adding
an **AliasRangeDimension** is straight forward:

```c++
nix::DataArray array = block.createDataArray("name", "nix.irregular_sampled", event_times);
array.label("time");
array.unit("s");

nix::RangeDimension dim = array.appendAliasRangeDimension();
```

**Note!** An **AliasRangeDimension** is only permitted, if the data is
1-D and the values are numeric, an exception will be thrown otherwise.


## SetDimension

![set_dim_plot](./images/set_dimension.png "simple plot with categories")

If we need to store data that falls into categories, i.e. the
x-positions are not numeric or the dimension does not have a natural
order, a **SetDimension** is used. It merely stores a label for each
entry along the described dimension.

```c++
std::vector<std::string> labels = {"A", "B", "C", "D", "E"};

nix::DataArray array = block.createDataArray("temperatures", "nix.categorical", temperatures);
array.label("temperature");
array.unit("K")

SetDimension dim = array.appendSetDimension();
dim.labels(labels);
```



[home](./index.md "nix github.io home") -- [back](./getting_started.md "Getting started")
