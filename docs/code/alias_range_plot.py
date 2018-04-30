import matplotlib.pyplot as plt
import numpy as np

if __name__ == "__main__":
    event_times = np.arange(1, 20, 2.5)
    event_times += np.random.randn(len(event_times))*0.5

    fig = plt.figure()
    fig.set_size_inches(5.5, 1.25)
    ax = fig.add_subplot(111)
    ax.scatter(event_times, np.ones(event_times.shape), s=20, marker="o")
    ax.set_ylim([0.95, 1.05])
    ax.set_xticks(np.arange(0.0, 21., 5.))
    ax.set_yticks([1.0])
    ax.set_yticklabels([])
    ax.set_xlabel("time [s]")
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)

    fig.subplots_adjust(left=0.05, bottom=0.35, right=0.95, top=0.95)
    fig.savefig('../images/alias_range.png')
