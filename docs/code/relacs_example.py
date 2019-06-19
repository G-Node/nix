import nixio as nix
import numpy as np
import matplotlib.pyplot as plt


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
    dim = eod.dimensions[0]

    plt.plot(dim.axis(samples), trace[:samples], label=eod.name)
    plt.xlabel("%s [%s]" % (dim.label, dim.unit))
    plt.ylabel("%s [%s]" % (trace.label, trace.unit))
    plt.legend()
    plt.show()

    f.close()


def using_tags(filename):
    f = nix.File.open(filename, nix.FileMode.ReadOnly)
    b = f.blocks[0]
    eod = b.data_arrays["V-1"]
    dim = eod.dimensions[0]
    pass


if __name__ == "__main__":
    example_file = "../examples/relacs_data.nix"
    # show_file_info(example_file)
    # print_subject_metadata(example_file)
    # plot_data_snippet(example_file)
