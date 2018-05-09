import numpy as np
import matplotlib.pyplot as plt
import matplotlib.table
from mpl_toolkits.mplot3d import Axes3D
from mpl_toolkits.mplot3d.art3d import Poly3DCollection
import matplotlib.patches as patches
from IPython import embed


def plotcubus(axis, pos, ext, color):
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
    poly.set_facecolor(color)
    axis.add_collection3d(poly)

    # top
    x = [xs, xe, xe, xs, xs]
    y = [ys, ys, ye, ye, ys]
    z = [zs, zs, zs, zs, zs]
    verts = [list(zip(x, y, z))]
    poly = Poly3DCollection(verts, alpha=0.1, lw=0.75,  linestyle='--', edgecolors='gray')
    poly.set_facecolor(color)
    axis.add_collection3d(poly)

    # bottom
    x = [xs, xe, xe, xs, xs]
    y = [ys, ys, ye, ye, ys]
    z = [ze, ze, ze, ze, ze]
    verts = [list(zip(x, y, z))]
    poly = Poly3DCollection(verts, alpha=0.1, lw=0.75,  linestyle='--', edgecolors='gray')
    poly.set_facecolor(color)
    axis.add_collection3d(poly)

    # back
    x = [xs, xs, xe, xe, xs]
    y = [ye, ye, ye, ye, ye]
    z = [zs, ze, ze, zs, zs]
    verts = [list(zip(x, y, z))]
    poly = Poly3DCollection(verts, alpha=0.1, lw=0.75,  linestyle='--', edgecolors='gray')
    poly.set_facecolor(color)
    axis.add_collection3d(poly)

    # left
    x = [xs, xs, xs, xs, xs]
    y = [ys, ye, ye, ys, ys]
    z = [zs, zs, ze, ze, zs]
    verts = [list(zip(x, y, z))]
    poly = Poly3DCollection(verts, alpha=0.1, lw=0.75, edgecolors='gray', linestyle='--')
    poly.set_facecolor(color)
    axis.add_collection3d(poly)

    # right
    x = [xe, xe, xe, xe, xe]
    y = [ys, ye, ye, ys, ys]
    z = [zs, zs, ze, ze, zs]
    verts = [list(zip(x, y, z))]
    poly = Poly3DCollection(verts, alpha=0.1, lw=0.75, edgecolors='gray', linestyle='--')
    poly.set_facecolor(color)
    axis.add_collection3d(poly)
    axis.scatter(xs, ys, zs, marker='.', s=50, facecolor='r', edgecolor='k')
    axis.plot([xs, xe], [ys, ys], [zs, zs], lw=1.5, color='r')
    axis.plot([xs, xs], [ys, ye], [zs, zs], lw=1.5, color='r')
    axis.plot([xs, xs], [ys, ys], [zs, ze], lw=1.5, color='r')
    #axis.text(xs + ext[0]/2, ys, zs + ext[2]/2, 'region')


def plot_2d():
    positions = [[10, 90], [60, 5]]
    extents = [[30, -30], [30, 20]]
    colors = ['dodgerblue', 'crimson']

    fig = plt.figure()
    fig.set_size_inches(5.5, 2.5)
    ax = fig.add_subplot(121)
    ax.set_xlim([0, 100])
    ax.set_xticks([0, 25, 50, 75, 100])
    ax.set_xlabel('width [mm]')
    ax.set_ylim([0, 100])
    ax.set_ylabel('height [mm]')
    ax.set_yticks([0, 25, 50, 75, 100])
    rect = patches.Rectangle((10, 90), 30, -30, alpha=0.25, facecolor="dodgerblue", edgecolor='k',
                             lw=0.75, ls='--')
    ax.add_patch(rect)
    rect = patches.Rectangle((60, 5), 30, 20, alpha=0.25, facecolor="crimson", edgecolor='k',
                             lw=0.75, ls='--')
    ax.add_patch(rect)
    ax.scatter([10, 60], [90, 5], marker=".", s=50, lw=0.25, facecolor="r", edgecolor='k')
    ax.plot([10, 40], [90, 90], lw=1.5, color='r')
    ax.plot([10, 10], [90, 60], lw=1.5, color='r')
    ax.plot([60, 60], [5, 25], lw=1.5, color='r')
    ax.plot([60, 90], [5, 5], lw=1.5, color='r')

    col_labels = ["Pos 1",  "Pos 2"]
    rows_2d = ["width", "height"]
    cell_text = [[positions[0][0], positions[1][0]],
                 [positions[0][1], positions[1][1]]]

    table = matplotlib.table.table(ax, cellText=cell_text, rowLabels=rows_2d,
                                   colLabels=col_labels,
                                   cellLoc="center", bbox=[1.6, 0.6, 0.4, 0.3],
                                   colWidths=[0.2 for c in col_labels],
                                   colColours=colors)
    ax.text(130, 100, "Positions DataArray") #, transform=ax.transAxes , fontsize=10)
    table.auto_set_font_size(False)
    table.set_fontsize(8)

    col_labels = ["Ext 1",  "Ext 2"]
    cell_text = [[extents[0][0], extents[1][0]],
                 [extents[0][1], extents[1][1]]]

    table = matplotlib.table.table(ax, cellText=cell_text, rowLabels=rows_2d,
                                   colLabels=col_labels,
                                   cellLoc="center", bbox=[1.6, 0.1, 0.4, 0.3],
                                   colWidths=[0.2 for c in col_labels],
                                   colColours=colors)
    ax.text(130, 100, "Positions DataArray") #, transform=ax.transAxes , fontsize=10)
    table.auto_set_font_size(False)
    table.set_fontsize(8)
    ax.text(130, 45, "Extents DataArray") #, transform=ax.transAxes , fontsize=10)

    ax.spines["top"].set_visible(False)
    ax.spines['right'].set_visible(False)
    fig.subplots_adjust(bottom=0.175, left=0.15, right=0.95, top=0.95)
    fig.savefig("../images/2d_mtag.png")


def plot_3d():
    positions = [[10, 10, 90], [60, 5, 20]]
    extents = [[30, 40, -30], [30, 60, 20]]
    colors = ['dodgerblue', 'crimson']
    fig = plt.figure()
    fig.set_size_inches(5.5, 2.5)
    ax = plt.subplot2grid((2, 5), (0, 0), colspan=3, rowspan=2, projection='3d')

    ax.set_xlim([0, 100])
    ax.set_xticks([0, 25, 50, 75, 100])
    ax.set_xlabel('width [mm]')
    ax.set_ylim([0, 100])
    ax.set_yticks([0, 25, 50, 75, 100])

    ax.set_ylabel('depth [mm]')
    ax.set_zlabel('height [mm]')
    ax.set_zlim([0, 100])
    ax.set_zticks([0, 25, 50, 75, 100])

    for i in range(len(positions)):
        plotcubus(ax, positions[i], extents[i], colors[i])

    col_labels = ["Pos 1",  "Pos 2"]
    rows_3d = ["width", "height", "depth"]

    cell_text = [[positions[0][0], positions[1][0]], [positions[0][2], positions[1][2]],
                 [positions[0][1], positions[1][1]]]

    table = matplotlib.table.table(ax, cellText=cell_text, rowLabels=rows_3d, colLabels=col_labels,
                                   cellLoc="center", bbox=[1.4, 0.5, 0.25, 0.3],
                                   colWidths=[0.1 for c in col_labels],
                                   colColours=colors)
    ax.text(170, 100, 130.0, "Positions DataArray") #, transform=ax.transAxes , fontsize=10)
    table.auto_set_font_size(False)
    table.set_fontsize(8)

    col_labels = ["Ext 1",  "Ext 2"]
    cell_text = [[extents[0][0], extents[1][0]], [extents[0][2], extents[1][2]],
                 [extents[0][1], extents[1][1]]]
    table = matplotlib.table.table(ax, cellText=cell_text, rowLabels=rows_3d, colLabels=col_labels,
                                   cellLoc="center", bbox=[1.4, -.1, 0.25, 0.3],
                                   colWidths=[0.1 for c in col_labels],
                                   colColours=colors)
    table.auto_set_font_size(False)
    table.set_fontsize(8)
    ax.text(180, 100, 5.0, "Extents DataArray") #, transform=ax.transAxes , fontsize=10)

    fig.subplots_adjust(bottom=0.1, left=0., right=0.95, top=0.95)
    fig.savefig("../images/3d_mtag.png")


if __name__ == "__main__":
    plot_2d()
    plot_3d()
