import numpy as np
import matplotlib.pyplot as plt

interval = 0.001
time = np.arange(0., 1.0, interval)
freq = 5.
signal = np.sin(time * 2 * np.pi * freq) + np.sin(time * 2 * np.pi * 2 * freq) * 0.4
events = time[(signal > 0.5) & (np.roll(signal, -1) <= 0.5)]

fig = plt.figure()
fig.set_size_inches(5.5, 2.5)
ax = fig.add_subplot(111)
ax.plot(time, signal, label='voltage', zorder=0)
ax.scatter(events, np.ones(events.shape) * 0.5, marker=".", s=75, color='red',
           label='threshold crossings',zorder=1)

ax.set_xlim([0., 1.])
ax.set_ylim([-1.75, 1.75])
ax.set_xticks(np.arange(0, 1.01, 0.25))
ax.set_yticks(np.arange(-1.5, 1.6, 0.5))
ax.set_xlabel("time [s]")
ax.set_ylabel("voltage [mV]")
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
fig.subplots_adjust(bottom=0.175, left=0.15, right=0.95, top=0.95)
ax.legend(loc=3, frameon=False, ncol=2)

fig.savefig("../images/multiple_points.png")
