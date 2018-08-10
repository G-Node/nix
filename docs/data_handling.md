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

