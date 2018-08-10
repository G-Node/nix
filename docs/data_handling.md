# Data handling
Storing data is one thing, but we want to work with it. The
following examples are meant to illustrate the reading of data and
metadata and the handling of tags. We will use the dummy dataset already used in
the [tagging](./tagging.md) example.

![simple_tag_plot](./images/tag1.png "a system's response to a stimulus")

The code that creates the dummy data and links the *Tag* providing the
context (a stimulus was on in a certain time segment) to the
*DataArray* that actually stores the data.

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

    nix::Tag stim_tag = block.createTag("stimulus", "nix.stimulus_segment", {stim_on});
    stim_tag.extent({stim_off - stim_on});
    stim_tag.addReference(data);

    f.close();
    return 0;
}
```

In this example we know the interesting entities by name, i.e. the
*DataArray* is called **response** and the *Tag* is called
**stimulus**. In cases in which we have no clue about the names, we
just have to browse the file or [search](#finding_stuff) by name and type.

## Reading data
The first and maybe most common problem is to read the data stored in
a *DataArray*.

### Reading all data
In *NIX* when you open a *DataArray* the stored the data is **not**
automatically read from file. This needs to be done separately.

1. open the file, get the *Block* and the *DataArray*
2. Reserve some space to copy the data.
3. Get the data from file.
4. Get the timestamps from the dimension.

The following code snippet shows (one way) how this works.

```c++
#include <nix.hpp>
#include <boost/optional/optional_io.hpp>

void main() {
 // 1. Open file, block, and dataarray
    nix::File file = nix::File::open("tagging1.nix", nix::FileMode::ReadOnly);
    nix::Block block = file.getBlock("demo block");

    if (!block.hasDataArray("response"))
        return -1;
    nix::DataArray responseArray = block.getDataArray("response");

    // 2. reserve some space, will use a std::vector here, hardcoded data type double
    nix::NDSize dataExtent = responseArray.dataExtent();
    std::vector<double> responseData(dataExtent[0]);

    // 3. read the data to the vector
    responseArray.getData(nix::DataType::Double, responseData.data(), dataExtent, nix::NDSize({ 0 }));

    // 4. get the timestamps
    SampledDimension dim = responseArray.getDimension(1).asSampledDimension();
    std::vector<double> time = dim.axis(responseData.size());

    // some output to confirm it worked
    std::cerr << dim.label() << "\t" << responseArray.label() << std::endl;
    std::cerr << dim.unit() << "\t" << responseArray.unit() << std::endl;
    for (size_t i = 0; i < responseData.size(); ++i) {
        std::cerr << time[i] << "\t" << responseData[i] << std::endl;
    }

    file.close();
    return 0;
}
```

