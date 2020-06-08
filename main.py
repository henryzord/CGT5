import argparse
import os

import shapefile
from descartes import PolygonPatch
from matplotlib import pyplot as plt
from matplotlib.collections import PatchCollection


def main(map_path):
    file_name = map_path.split(os.sep)[-1]
    read_path = os.path.join(map_path, file_name)
    geometry = shapefile.Reader(read_path, encoding='latin-1')

    fig, ax = plt.subplots()

    for i, sr in enumerate(geometry.shapeRecords()):
        poly = sr.__geo_interface__
        ax.add_patch(PolygonPatch(poly['geometry'], alpha=0.5, zorder=2, fc='#6699cc', ec='#6699cc'))

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
    main(map_path=args.map_path)

