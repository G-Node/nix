import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as patches

x_start = 40
y_start = 80
x_extent = 50
y_extent = -40

fig = plt.figure()
fig.set_size_inches(5.5, 2.5)
ax = fig.add_subplot(111)

ax.set_xlim([0., 100.])
ax.set_ylim([0., 100.])
ax.set_xticks(np.arange(0, 101, 25))
ax.set_yticks(np.arange(0, 101, 25))
ax.set_xlabel("time [s]")
ax.set_ylabel("voltage [V]")

rect = patches.Rectangle((x_start, y_start), x_extent,y_extent, alpha=0.5, facecolor="silver", edgecolor='k', lw=0.75, ls='--' )
ax.add_patch(rect)
ax.annotate('', xy=(x_start + x_extent, y_start), xytext=(x_start, y_start),
            arrowprops=dict(facecolor='silver', connectionstyle="arc3", arrowstyle="->", lw=1.5))
ax.annotate('', xy=(x_start, y_start + y_extent), xytext=(x_start, y_start),
            arrowprops=dict(facecolor='silver', connectionstyle="arc3", arrowstyle="->", lw=1.5 ))
ax.annotate('position (%i, %i)'%(x_start, y_start), xy=(x_start, y_start), xytext=(5, 95),
            arrowprops=dict(facecolor='silver', connectionstyle="arc3", arrowstyle="->" ))
ax.scatter(x_start, y_start, marker=".", s=40, color='r')
ax.text(55, y_start + 5, "x-extent (%i)"%x_extent)
ax.text(x_start - 5, 75, "y-extent (%i)"%y_extent, rotation=90)
ax.text(x_start + x_extent/2, y_start + y_extent/2, "tagged region", ha="center", va="center")

ax.spines["top"].set_visible(False)
ax.spines["right"].set_visible(False)
fig.subplots_adjust(bottom=0.175, left=0.15, right=0.95, top=0.95)
fig.savefig("../images/2d_tag.png")
