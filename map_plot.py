import argparse
import copy
import os

import shapefile
from descartes import PolygonPatch
from geometries import Polygon, Map
from geometries import rotatePoint
from matplotlib import pyplot as plt
from matplotlib.patches import Polygon as pltPolygon
from matplotlib import animation
import numpy as np
from functools import reduce


def old_main(map_path):
    file_name = map_path.split(os.sep)[-1]
    read_path = os.path.join(map_path, file_name)
    geometry = shapefile.Reader(read_path, encoding='latin-1')

    fig, ax = plt.subplots()

    for i, sr in enumerate(geometry.shapeRecords()):
        poly = sr.__geo_interface__
        ax.add_patch(PolygonPatch(poly['geometry'], alpha=0.5, zorder=2, fc='#6699cc', ec='#6699cc'))

    ax.axis('scaled')
    plt.show()


def test_map_insertion():
    fig, ax = plt.subplots()

    x_lim = [-1, 8]
    y_lim = [-1, 8]

    ax.set_xlim(-1, 8)
    ax.set_ylim(-1, 8)

    init_pos = np.array([3, 2], dtype=np.float64)
    speed = np.array([0.25, 0.3], dtype=np.float64)

    myMap = Map(polygons=[
        Polygon([(0, 4), (3, 0), (4, 3)]),
        Polygon([(3, 0), (7, 2), (4, 3)]),
        Polygon([(4, 3), (7, 2), (7, 5)]),
        Polygon([(0, 4), (4, 3), (3, 7)]),
        Polygon([(4, 3), (3, 7), (7, 5)])
    ])

    polygons_plot_data = [np.array(pol.vertices)[:, :2].astype(np.float32) for pol in myMap.polygons]

    # generates data for current time
    def data_gen():
        t = data_gen.t
        count = 0
        while count < 360:
            count += 15
            t += 1.0
            new_pos = init_pos
            new_pos[0] = init_pos[0] + (speed[0] * t)  # + ((0.01 * t)**2.)/2.
            new_pos[1] = init_pos[1] + (speed[1] * t)  # + ((0.01 * t) ** 2.) / 2.

            if new_pos[0] > max(x_lim):
                speed[0] *= -1
                new_pos[0] = max(x_lim)
            elif new_pos[0] < min(x_lim):
                speed[0] *= -1
                new_pos[0] = min(x_lim)

            if new_pos[1] > max(y_lim):
                speed[1] *= -1
                new_pos[1] = max(y_lim)
            elif new_pos[1] < min(y_lim):
                speed[1] *= -1
                new_pos[1] = min(y_lim)

            yield t, new_pos

    data_gen.t = 0

    # intialize two line objects (one in each axes)
    sct = ax.scatter([], [], lw=2)

    # the same axes initalizations as before (just now we do it for both of them)

    ax.set_axis_off()

    # initialize the data arrays

    def run(data):
        # update the data
        t, p = data

        # inside_index = myMap.checkInside(p)
        # inside_index = -1

        patches = [ax.scatter(p[0], p[1], ec='black', c='white')]

        for i, original_data in enumerate(polygons_plot_data):
            if myMap.checkInside(tuple(p)) == i:
                pltpol0 = pltPolygon(original_data, fill=True, color='#88d2db', ec='black', alpha=0.4, zorder=0)
            else:
                pltpol0 = pltPolygon(original_data, fill=False, ec='black', alpha=0.4, zorder=0)

            patches += [ax.add_patch(pltpol0)]

        # ax.figure.canvas.draw()

        return patches

    ani = animation.FuncAnimation(fig, run, data_gen, blit=True, interval=100, repeat=True)

    ax.axis('scaled')
    plt.show()


def main(map_path):
    file_name = map_path.split(os.sep)[-1]
    read_path = os.path.join(map_path, file_name)
    geometry = shapefile.Reader(read_path, encoding='latin-1')

    fig, ax = plt.subplots()

    polygons_plot_data = []
    polygons_obj = []

    for i, sr in enumerate(geometry.shapeRecords()):
        descartes_poly = sr.__geo_interface__
        abc = descartes_poly['geometry']['coordinates']
        while isinstance(abc[0][0], tuple):
            abc = abc[0]
        abc = list(abc)
        polygons_plot_data += [np.array(abc).astype(np.float64)]
        polygons_obj += [Polygon(abc)]

        # ax.add_patch(PolygonPatch(poly['geometry'], alpha=0.5, zorder=2, fc='#6699cc', ec='#6699cc'))

    polygons_plot_data = np.array(polygons_plot_data)

    x_lim = [reduce(min, map(lambda x: min(x[:, 0]), polygons_plot_data)), reduce(max, map(lambda x: max(x[:, 0]), polygons_plot_data))]
    y_lim = [reduce(min, map(lambda x: min(x[:, 1]), polygons_plot_data)), reduce(max, map(lambda x: max(x[:, 1]), polygons_plot_data))]

    ax.set_xlim(*x_lim)
    ax.set_ylim(*y_lim)

    init_pos = np.array((x_lim[0] + 1, y_lim[0] + 1), dtype=np.float64)
    speed = np.array([0.25, 0.3], dtype=np.float64)

    myMap = Map(polygons=polygons_obj)

    # generates data for current time
    def data_gen():
        t = data_gen.t
        count = 0
        while count < 360:
            count += 15
            t += 1.0
            new_pos = init_pos
            new_pos[0] = init_pos[0] + (speed[0] * t)  # + ((0.01 * t)**2.)/2.
            new_pos[1] = init_pos[1] + (speed[1] * t)  # + ((0.01 * t) ** 2.) / 2.

            if new_pos[0] > max(x_lim):
                speed[0] *= -1
                new_pos[0] = max(x_lim)
            elif new_pos[0] < min(x_lim):
                speed[0] *= -1
                new_pos[0] = min(x_lim)

            if new_pos[1] > max(y_lim):
                speed[1] *= -1
                new_pos[1] = max(y_lim)
            elif new_pos[1] < min(y_lim):
                speed[1] *= -1
                new_pos[1] = min(y_lim)

            yield t, new_pos

    data_gen.t = 0

    # intialize two line objects (one in each axes)
    sct = ax.scatter([], [], lw=2)

    # the same axes initalizations as before (just now we do it for both of them)

    ax.set_axis_off()

    # initialize the data arrays

    def run(data):
        # update the data
        t, p = data

        # inside_index = myMap.checkInside(p)
        # inside_index = -1

        patches = [ax.scatter(p[0], p[1], ec='black', c='white')]

        for i, original_data in enumerate(polygons_plot_data):
            inside_index = -1 if not polygons_obj[i].isInside(tuple(p)) else i
        #     inside_index = myMap.checkInside(tuple(p))
            print(inside_index)
            if inside_index == i:
                pltpol0 = pltPolygon(original_data, fill=True, color='#88d2db', ec='black', alpha=0.4, zorder=0)
            else:
                pltpol0 = pltPolygon(original_data, fill=False, ec='black', alpha=0.4, zorder=0)

            patches += [ax.add_patch(pltpol0)]

        # ax.figure.canvas.draw()

        return patches

    ani = animation.FuncAnimation(fig, run, data_gen, blit=True, interval=100, repeat=True)

    ax.axis('scaled')
    plt.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description='script for collapsing metrics'
    )

    parser.add_argument(
        '--map-path', action='store', required=True,
        help='Path to a folder where shapefile map is stored.'
    )

    args = parser.parse_args()
    # old_main(map_path=args.map_path)
    main(map_path=args.map_path)

