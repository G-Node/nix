import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

time = np.arange(0., 3.0, 0.001)
stim = np.ones(time.shape)
duration = 0.25
intervals = np.arange(0.25, 5., duration * 3)
base_freq = 5

for i, interval in enumerate(intervals):
    stim[(time >= interval) & (time < interval + duration)] = i + 2

stim *= base_freq
signal = np.sin(time * 2 * np.pi * stim)


fig = plt.figure()
fig.set_size_inches(5.5, 2.5)
ax = fig.add_subplot(111)

l, = ax.plot(time, signal, label="signal")
ax.set_xlim([0., 3.])
ax.set_ylim([-1.45, 1.45])
ax.set_xticks(np.arange(0., 3.01, 0.5))
ax.set_yticks(np.arange(-1.5, 1.6, 0.5))
ax.set_xlabel("time [s]")
ax.set_ylabel("voltage [mV]")

for interval in intervals:
    rect = patches.Rectangle((interval, 1.05), duration, -2.1, alpha=0.5,
                             facecolor="silver", edgecolor='k', lw=0.75, ls='--')
    ax.add_patch(rect)
ax.legend((l, rect), ("signal", "stimulus ON"), loc=1, frameon=False, ncol=2)

ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
fig.subplots_adjust(bottom=0.175, left=0.15, right=0.95, top=0.95)
fig.savefig("../images/multiple_regions.png")
