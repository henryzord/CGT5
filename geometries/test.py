from geometries import Polygon, Map
from matplotlib import pyplot as plt
from matplotlib.patches import Polygon as pltPolygon
import numpy as np


def test_envelope_collision_detection(axes: np.array, base_index: int):
    # plots envelope
    polA = Polygon([(2, 3), (3, 2), (4, 3)])
    polB = Polygon([(2.75, 4), (2, 6), (4, 5)])
    polC = Polygon([(2, 5), (3, 4), (4, 5)])
    envelope = np.array(polA.getEnvelope())

    colorAB = '#e3a3d5' if polA.checkEnvelopeIntersection(polB) else '#88d2db'
    colorBC = '#e3a3d5' if polB.checkEnvelopeIntersection(polC) else '#88d2db'

    axes[base_index].add_patch(pltPolygon(np.array(polA.vertices)[:, :2].astype(np.float32), fill=True, color=colorAB, alpha=0.4))
    axes[base_index].add_patch(pltPolygon(np.array(polB.vertices)[:, :2].astype(np.float32), fill=True, color=colorAB, alpha=0.4))
    axes[base_index].set_xlim(0, 6)
    axes[base_index].set_ylim(0, 7)
    axes[base_index].set_axis_off()

    axes[base_index + 1].add_patch(pltPolygon(np.array(polB.vertices)[:, :2].astype(np.float32), fill=True, color=colorBC, alpha=0.4))
    axes[base_index + 1].add_patch(pltPolygon(np.array(polC.vertices)[:, :2].astype(np.float32), fill=True, color=colorBC, alpha=0.4))
    axes[base_index + 1].set_xlim(0, 6)
    axes[base_index + 1].set_ylim(0, 7)
    axes[base_index + 1].set_axis_off()

    return axes, base_index + 2


def test_convex_hull(axes: np.array, base_index: int):

    concavos = [Polygon([(2, 0), (2, 6), (7, 6), (7, 0), (5, 2)]), Polygon([(2, 0), (4, 6), (9, 4), (6, 0), (5, 2)])]

    # testa convex hull
    for i, pol in enumerate(concavos):
        original_data = np.array(pol.vertices)[:, :2].astype(np.float32)
        convex_data = np.array(pol.toConvexHull().vertices)[:, :2].astype(np.float32)

        axes[base_index + i].set_xlim(min(original_data[:, 0]), max(original_data[:, 0]))
        axes[base_index + i].set_ylim(min(original_data[:, 1]), max(original_data[:, 1]))

        pltpol0 = pltPolygon(original_data, fill=False, ec='black', alpha=0.4, zorder=0)
        axes[base_index + i].add_patch(pltpol0)

        pltpol1 = pltPolygon(convex_data, fill=True, ec='#88d2db', alpha=0.4, zorder=1)
        axes[base_index + i].add_patch(pltpol1)

        axes[base_index + i].set_axis_off()

    return axes, base_index + len(concavos)


def test_points_inside_polygons(axes: np.array, base_index: int):
    """
    Testa se um ponto está dentro de polígono côncavo/convexo.

    :param axes: Onde projetar os gráficos
    :param base_index: o menor índice dos gráficos a ser usado
    :rtype: tuple
    :return: Uma tupla onde o primeiro membro é axes atualizado e o segundo o próximo índice utilizável
    """

    testing_points = [(1, 5), (5, 1), (8, 5), (6, 4), (4, 3)]

    convexos = [Polygon([(5, 0), (3, 2), (5, 6), (7, 3)]), Polygon([(2, 0), (2, 6), (7, 6), (7, 0)])]
    corretude_convexos = [[0, 1, 0, 1, 1], [0, 1, 0, 1, 1]]
    concavos = [Polygon([(2, 0), (2, 6), (7, 6), (7, 0), (5, 2)]), Polygon([(2, 0), (4, 6), (9, 4), (6, 0), (5, 2)])]
    corretude_concavos = [[0, 0, 0, 1, 1], [0, 0, 0, 1, 1]]

    for i, convex in enumerate(convexos):
        pltpol = pltPolygon(np.array(convex.vertices)[:, :2].astype(np.float32), fill=False, ec='black', alpha=0.4)
        axes[base_index + i].add_patch(pltpol)
        axes[base_index + i].set_axis_off()

        for t_point, rightfulness in zip(testing_points, corretude_convexos[i]):
            axes[base_index + i].scatter(
                t_point[0], t_point[1], c='#88d2db' if convex.isInside(t_point) == bool(rightfulness) else '#e3a3d5'
            )

    for i, concav in enumerate(concavos):
        pltpol = pltPolygon(np.array(concav.vertices)[:, :2].astype(np.float32), fill=False, ec='black', alpha=0.4)
        axes[base_index + len(convexos) + i].add_patch(pltpol)
        axes[base_index + len(convexos) + i].set_axis_off()

        for t_point, rightfulness in zip(testing_points, corretude_concavos[i]):
            axes[base_index + len(convexos) + i].scatter(
                t_point[0], t_point[1], c='#88d2db' if concav.isInside(t_point) == bool(rightfulness) else '#e3a3d5'
            )

    return axes, base_index + len(convexos) + len(concavos)


def main():
    fig, axes = plt.subplots(nrows=3, ncols=3)
    axes = np.ravel(axes)

    base_index = 0
    axes, base_index = test_points_inside_polygons(axes, base_index=base_index)
    axes, base_index = test_convex_hull(axes, base_index=base_index)
    axes, base_index = test_envelope_collision_detection(axes, base_index=base_index)



    plt.show()
    exit(0)

    # ------------------------------------------- #

    fig, ax = plt.subplots()

    some_polygons = [
        Polygon([(0, 4), (3, 0), (4, 3)]),
        Polygon([(3, 0), (7, 2), (4, 3)]),
        Polygon([(4, 3), (7, 2), (7, 5)]),
        Polygon([(0, 4), (4, 3), (3, 7)]),
        Polygon([(4, 3), (3, 7), (7, 5)])
    ]

    some_points = [(3, 2), (5, 2), (6, 3), (2, 5), (5, 5)]
    # some_npy_points = np.array(some_points)
    # ax.scatter(some_npy_points[:, 0], some_npy_points[:, 1])
    # for pol in some_polygons:
    #     for i in range(1, pol.n_vertices):
    #         ax.plot([pol.vertices[i-1][0], pol.vertices[i][0]], [pol.vertices[i-1][1], pol.vertices[i][1]], c='black')
    #     ax.plot([pol.vertices[-1][0], pol.vertices[0][0]], [pol.vertices[-1][1], pol.vertices[0][1]], c='black')

    myMap = Map(polygons=some_polygons)

    for point in some_points:
        print('point %r is inside polygon %d' % (point, myMap.checkInside(point)))

    # plt.show()


if __name__ == '__main__':
    main()
