import numpy as np
import matplotlib.pyplot as plt
import matplotlib.table
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import matplotlib.patches as patches
from IPython import embed


def plotcubus(axis, pos, ext):
    xs = pos[0]
    ys = pos[1]
    zs = pos[2]
    xe = xs + ext[0]
    ye = xs + ext[1]
    ze = zs + ext[2]

    # front
    x = [xs, xs, xe, xe, xs]
    y = [ys, ys, ys, ys, ys]
    z = [zs, ze, ze, zs, zs]
    verts = [list(zip(x, y, z))]
    poly = Poly3DCollection(verts, alpha=0.1, lw=0.75, linestyle='--', edgecolors='gray')
    poly.set_facecolor('dodgerblue')
    axis.add_collection3d(poly)

    # top
    x = [xs, xe, xe, xs, xs]
    y = [ys, ys, ye, ye, ys]
    z = [zs, zs, zs, zs, zs]
    verts = [list(zip(x, y, z))]
    poly = Poly3DCollection(verts, alpha=0.1, lw=0.75,  linestyle='--', edgecolors='gray')
    poly.set_facecolor('dodgerblue')
    axis.add_collection3d(poly)

    # bottom
    x = [xs, xe, xe, xs, xs]
    y = [ys, ys, ye, ye, ys]
    z = [ze, ze, ze, ze, ze]
    verts = [list(zip(x, y, z))]
    poly = Poly3DCollection(verts, alpha=0.1, lw=0.75,  linestyle='--', edgecolors='gray')
    poly.set_facecolor('dodgerblue')
    axis.add_collection3d(poly)

    # back
    x = [xs, xs, xe, xe, xs]
    y = [ye, ye, ye, ye, ye]
    z = [zs, ze, ze, zs, zs]
    verts = [list(zip(x, y, z))]
    poly = Poly3DCollection(verts, alpha=0.1, lw=0.75,  linestyle='--', edgecolors='gray')
    poly.set_facecolor('dodgerblue')
    axis.add_collection3d(poly)

    # left
    x = [xs, xs, xs, xs, xs]
    y = [ys, ye, ye, ys, ys]
    z = [zs, zs, ze, ze, zs]
    verts = [list(zip(x, y, z))]
    poly = Poly3DCollection(verts, alpha=0.1, lw=0.75, edgecolors='gray', linestyle='--')
    poly.set_facecolor('dodgerblue')
    axis.add_collection3d(poly)

    # right
    x = [xe, xe, xe, xe, xe]
    y = [ys, ye, ye, ys, ys]
    z = [zs, zs, ze, ze, zs]
    verts = [list(zip(x, y, z))]
    poly = Poly3DCollection(verts, alpha=0.1, lw=0.75, edgecolors='gray', linestyle='--')
    poly.set_facecolor('dodgerblue')
    axis.add_collection3d(poly)


positions = [[10, 10, 90], [60, 5, 20]]
extents = [[30, 40, -30], [30, 60, 20]]

fig = plt.figure()
ax = plt.subplot2grid((4, 4), (0, 0), colspan=3, rowspan=2, projection='3d')
ax2 = plt.subplot2grid((4, 4), (2,0), colspan=3, rowspan=2)

ax2.scatter([10, 60], [60, 5], marker=".", s=40, color='r', zorder=2)
ax2.set_xlim([0, 100])
ax2.set_ylim([0, 100])
rect = patches.Rectangle((10, 90), 30, -30, alpha=0.5, facecolor="dodgerblue", edgecolor='k',
                         lw=0.75, ls='--')
ax2.add_patch(rect)
rect = patches.Rectangle((60, 5), 30, 20, alpha=0.5, facecolor="dodgerblue", edgecolor='k',
                         lw=0.75, ls='--')
ax2.add_patch(rect)

#ax.annotate('', xy=(x_start + x_extent, y_start), xytext=(x_start, y_start),
#            arrowprops=dict(facecolor='silver', connectionstyle="arc3", arrowstyle="->", lw=1.5))
#ax.annotate('', xy=(x_start, y_start + y_extent), xytext=(x_start, y_start),
#            arrowprops=dict(facecolor='silver', connectionstyle="arc3", arrowstyle="->", lw=1.5 ))
#ax.annotate('position (%i, %i)'%(x_start, y_start), xy=(x_start, y_start), xytext=(5, 95),
#            arrowprop

#ax = fig.add_subplot(2, 2, 2, projection='3d')
ax.set_xlim([0, 100])
ax.set_xlabel('width')
ax.set_ylim([0, 100])
ax.set_ylabel('depth')
ax.set_zlabel('height')
ax.set_zlim([0, 100])
for i in range(len(positions)):
    plotcubus(ax, positions[i], extents[i])


rows_2d = ["width", "height"]
rows_3d = ["width", "height", "depth"]

#ax = fig.add_subplot(223)
col_labels = ["Position 1",  "Position 2"]
cell_text = [[positions[0][0], positions[1][0]], [positions[0][1], positions[1][1]],
             [positions[0][2], positions[1][2]]]

table = matplotlib.table.table(ax, cellText=cell_text, rowLabels=rows_3d, colLabels=col_labels,
                               cellLoc="center", bbox=[1.2, 0., 0.5, 0.3])
table = matplotlib.table.table(ax, cellText=cell_text, rowLabels=rows_3d, colLabels=col_labels,
                               cellLoc="center", bbox=[1.2, .5, 0.5, 0.3])
table.auto_set_font_size(False)
table.set_fontsize(8)
#table.AXESPAD = 0.5

plt.show()
