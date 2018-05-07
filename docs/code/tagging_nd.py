import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
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
ax = fig.add_subplot(2, 2, 2, projection='3d')
ax.set_xlim([0, 100])
ax.set_xlabel('width')
ax.set_ylim([0, 100])
ax.set_ylabel('depth')
ax.set_zlabel('height')
ax.set_zlim([0, 100])
for i in range(len(positions)):
    plotcubus(ax, positions[i], extents[i])

ax = fig.add_subplot(221)
ax.plot([0, 100], [0, 100])

plt.show()
