import numpy as np
import matplotlib.pyplot as plt


def plot_signal(signal, time, ax):
    ax.plot(time, signal, label='voltage', zorder=0)
    ax.set_xlim([0., 1.])
    ax.set_ylim([-1.75, 1.75])
    ax.set_xticks(np.arange(0, 1.01, 0.25))
    ax.set_yticks(np.arange(-1., 1.1, 1.))
    ax.set_xlabel("time [s]")
    ax.set_ylabel("voltage [mV]")
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)


def plot_events(events, ax):
    ax.scatter(events, np.ones(events.shape) * 0.5, marker=".", s=75, color='red',
               label='threshold crossings', zorder=1)
    ax.set_xlim([0., 1.])
    ax.set_xticks(np.arange(0, 1.01, 0.25))
    ax.set_yticks([0.5])
    ax.set_yticklabels([])
    ax.set_xlabel("time [s]")
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)


def plot_mtag(signal, time, events, ax):
    plot_signal(signal, time, ax)
    plot_events(events, ax)
    ax.set_xlim([0., 1.])
    ax.set_ylim([-1.75, 1.75])
    ax.set_xticks(np.arange(0, 1.01, 0.25))
    ax.set_yticks(np.arange(-1., 1.1, 1.0))
    ax.set_yticklabels(np.arange(-1., 1.1, 1.0))
    ax.set_xlabel("time [s]")
    ax.set_ylabel("voltage [mV]")
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)


def plot_links(ax):
    ax.set_xlim([0, 1])
    ax.set_xticks([])
    ax.set_ylim([0, 1])
    ax.set_yticks([])
    ax.text(-.2, 0.65, "MultiTag.reference", horizontalalignment="left")
    ax.text(-.2, 0.35, "MultiTag.positions", horizontalalignment="left")
    ax.annotate('', xy=(0.5, 1.75), xytext=(0.2, 0.825),
                arrowprops=dict(facecolor='silver', edgecolor="gray", shrink=0.05, width=1, headwidth=8),
                annotation_clip=False)

    ax.annotate('', xy=(0.5, -.9), xytext=(0.2, 0.25),
                arrowprops=dict(facecolor='silver', edgecolor="gray", shrink=0.05, width=1, headwidth=8),
                annotation_clip=False)

    ax.annotate('', xy=(1.0, 0.5), xytext=(0.6, 0.5),
                arrowprops=dict(facecolor='silver', edgecolor="gray", shrink=0.05, width=5, headwidth=15),
                annotation_clip=False)

    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    ax.spines["bottom"].set_visible(False)
    ax.spines["left"].set_visible(False)


if __name__ == "__main__":
    interval = 0.001
    time = np.arange(0., 1.0, interval)
    freq = 5.
    signal = np.sin(time * 2 * np.pi * freq) + np.sin(time * 2 * np.pi * 2 * freq) * 0.4
    events = time[(signal > 0.5) & (np.roll(signal, -1) <= 0.5)]

    fig = plt.figure()
    fig.set_size_inches(5.5, 4.5)

    signal_ax = plt.subplot2grid((5, 7), (0, 0), colspan=3)
    event_ax = plt.subplot2grid((5, 7), (4, 0), colspan=3)
    mtag_ax = plt.subplot2grid((5, 7), (2, 4), colspan=3)
    link_ax = plt.subplot2grid((5, 7), (2, 0), colspan=3)

    plot_signal(signal, time, signal_ax)
    signal_ax.set_title("DataArray: Signal", loc="left")

    plot_events(events, event_ax)
    event_ax.set_title("DataArray: Events", loc="left")

    plot_mtag(signal, time, events, mtag_ax)
    mtag_ax.set_title("MultiTag", loc="right")

    plot_links(link_ax)

    fig.subplots_adjust(bottom=0.12, left=0.095, right=0.99, top=0.95)
    plt.savefig("../images/mtag_concept.png")
