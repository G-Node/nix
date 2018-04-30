import numpy as np
import matplotlib.pyplot as plt

freq = 5.
time = np.arange(0., 1.0, 0.01)
sine = np.sin(2 * np.pi * freq * time)

fig = plt.figure()
fig.set_size_inches(5.5, 2.5)
ax = fig.add_subplot(111)

ax.plot(time, sine, color="dodgerblue", marker='.', markersize=5, label="sinewave",
        ls="solid", lw=1.25)
ax.set_xlim([0., 1.])
ax.set_ylim([-1.35, 1.35])
ax.set_xlabel("time [s]")
ax.set_ylabel("voltage [mV]")
ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
ax.legend(loc=1, frameon=False)
fig.subplots_adjust(left=0.125, bottom=0.175, right=0.95, top=0.95)
fig.savefig("../images/regular_sampled.png")
