import os
import nixio as nix
import numpy as np
import matplotlib.pyplot as plt
import requests


def show_file_info(filename):
    f = nix.File.open(filename, nix.FileMode.ReadOnly)
    print("file: %s \n\tformat: %s \n\tformat version: %s \n\tlibrary version %s"
          % (filename, f.format, f.version, nix.__version__))

    print("\tnumber of Blocks: %i"% len(f.blocks))
    print("\tnumber of DataArrays: %i" % len(f.blocks[0].data_arrays))
    print("\tnumber of MultiTags: %i" % len(f.blocks[0].multi_tags))
    print("\tnumber of Tags: %i" % len(f.blocks[0].tags))

    print("\tRelacs specific information:")
    traces = [da for da in f.blocks[0].data_arrays if "relacs.data.sampled" in da.type]
    print("\t\tnumber of regularly sampled traces: %i with %i data samples" %
          (len(traces), sum([da.shape[0] for da in traces])))
    print("\t\tnumber of RePro runs: %i" % len([t for t in f.blocks[0].tags if "repro_run" in t.type]))
    print("\t\tnumber of unique stimuli: %i" %
          len([mt for mt in f.blocks[0].multi_tags if "relacs.stimulus" in mt.type]))

    f.close()


def print_subject_metadata(filename):
    f = nix.File.open(filename, nix.FileMode.ReadOnly)
    b = f.blocks[0]
    sections = b.metadata.find_sections(lambda sec : "subject" in sec.type.lower())
    for sec in sections:
        sec.pprint(max_depth=-1)
    f.close()


def plot_data_snippet(filename, trace_name="V-1", samples=5000):
    f = nix.File.open(filename, nix.FileMode.ReadOnly)
    b = f.blocks[0]
    trace = b.data_arrays[trace_name]
    dim = trace.dimensions[0]

    plt.plot(dim.axis(samples), trace[:samples], label=trace.name)
    plt.xlabel("%s [%s]" % (dim.label, dim.unit))
    plt.ylabel("%s [%s]" % (trace.label, trace.unit))
    plt.legend()
    plt.show()

    f.close()


def get_baseline_data(filename):
    f = nix.File.open(filename, nix.FileMode.ReadOnly)
    b = f.blocks[0]
    tags = [t for t in b.tags if "baseline" in t.name.lower() and "relacs.repro_run" in t.type]
    if len(tags) < 1:
        return
    tag = tags[0]

    data = tag.retrieve_data("V-1")[:]  # FIXME: beyond 1.5 this will be tag.data("V-1")
    trace = tag.references["V-1"]
    dim = trace.dimensions[0]

    plt.plot(dim.axis(len(data), dim.index_of(tag.position[0])), data)
    plt.xlabel("%s [%s]" % (dim.label, dim.unit))
    plt.ylabel("%s [%s]" % (trace.label, trace.unit))
    plt.show()

    f.close()


def plot_fi_curve(filename):
    f = nix.File.open(filename, nix.FileMode.ReadOnly)
    b = f.blocks[0]

    tags = [t for t in b.tags if "ficurve" in t.name.lower() and "relacs.repro_run" in t.type]
    if len(tags) < 1:
        return
    tag = tags[0]
    g = b.groups[tag.name]
    mtags = g.multi_tags
    if len(mtags) < 1:
        return
    mtag = mtags[0]

    intensity_feature = [ft for ft in mtag.features if "_intensity" in ft.data.name.lower()][0]
    rates = np.zeros(mtag.positions.shape)
    intensities = np.zeros(mtag.positions.shape)

    for i in range(mtag.positions.shape[0]):
        duration = mtag.extents[i]
        spikes = mtag.retrieve_data(i, "Spikes-1")[:]
        rates[i] = len(spikes) / duration
        intensities[i] = mtag.retrieve_feature_data(i, intensity_feature.data.name)[:]

    plt.scatter(intensities, rates)
    plt.xlabel("stimulus [mV/cm]")
    plt.ylabel("firing rate [Hz]")
    plt.show()

    f.close()


def load_data():
    success = False
    example_url = "https://gin.g-node.org/G-Node/nix-examples/raw/master/relacs_simulation/relacs_data.nix"
    print("Dowloading example file from gin.g-node.org:")
    r = requests.get(example_url, stream=True)
    if r.status_code != requests.codes.ok:
        return success
    read_bytes = 0
    with open("relacs_data.nix", 'wb') as f:
        for chunk in r.iter_content(chunk_size=256):
            read_bytes += 256
            print("Downloaded %i bytes" % read_bytes, end="\r")
            f.write(chunk)
        success = True
        print("\nDownload succeeded!")

    return success


if __name__ == "__main__":
    example_file = "relacs_data.nix"
    if not os.path.exists(example_file):
        if not load_data():
            print("Failed to find example data!")
            pass
    show_file_info(example_file)
    print_subject_metadata(example_file)
    plot_data_snippet(example_file)
    get_baseline_data(example_file)
    plot_fi_curve(example_file)
