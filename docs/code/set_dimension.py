import numpy as np
import matplotlib.pyplot as plt

if __name__ == "__main__":
    data = np.random.randint(0, 373, 5)
    labels = ["A", "B", "C", "D", "E"]

    fig = plt.figure()
    fig.set_size_inches(5.5, 2.5)
    ax = fig.add_subplot(111)
    ax.bar(range(len(labels)), data, align="center", tick_label=labels)
    ax.spines["top"].set_visible(False)
    ax.spines["right"].set_visible(False)
    ax.set_ylabel("temperature [K]")

    fig.subplots_adjust(left=0.175, bottom=0.125, right=0.95, top=0.95)
    fig.savefig('../images/set_dimension.png')
