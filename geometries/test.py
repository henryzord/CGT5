from geometries import Polygon, Map
from matplotlib import pyplot as plt
import numpy as np


# class MyPolygon(geometries.Polygon):
#     pass


def main():
    testing_points = [(1, 5), (5, 1), (8, 5), (6, 4), (4, 3)]

    convexos = [Polygon([(5, 0), (3, 2), (5, 6), (7, 3)]), Polygon([(2, 0), (2, 6), (7, 6), (7, 0)])]
    corretude_convexos = [[0, 1, 0, 1, 1], [0, 1, 0, 1, 1]]
    concavos = [Polygon([(2, 0), (2, 6), (7, 6), (7, 0), (5, 2)]), Polygon([(2, 0), (4, 6), (9, 4), (6, 0), (5, 2)])]
    corretude_concavos = [[0, 0, 0, 1, 1], [0, 0, 0, 1, 1]]

    # testa se um ponto está dentro de polígono côncavo/convexo
    for i, convex in enumerate(convexos):
        for t_point, rightfulness in zip(testing_points, corretude_convexos[i]):
            answer = convex.isInside(t_point)
            if answer != bool(rightfulness):
                print('point %r is wrong for geometry with points %r' % (t_point, convex))
            # else:
            #     print('point (%.1f, %.1f) is correct' % (t_point[0], t_point[1]))

    for i, concav in enumerate(concavos):
        for t_point, rightfulness in zip(testing_points, corretude_concavos[i]):
            answer = concav.isInside(t_point)
            if answer != bool(rightfulness):
                print('point %r is wrong for geometry with points %r' % (t_point, concav))

    # ------------------------------------------- #

    # testa convex hull
    for pol in concavos:
        # print(concave)
        # print(concave.toConvexHull())
        for i in range(1, pol.n_vertices):
            plt.plot([pol.vertices[i-1][0], pol.vertices[i][0]], [pol.vertices[i-1][1], pol.vertices[i][1]], c='black')
        plt.plot([pol.vertices[-1][0], pol.vertices[0][0]], [pol.vertices[-1][1], pol.vertices[0][1]], c='black')

        corners = np.array(pol.addEnvelope())
        # for i in range(1, pol.n_vertices):
        #     plt.plot([corners[i-1][0], corners[i][0]], [corners[i-1][1], corners[i][1]], c='pink')
        # plt.plot([corners[pol.n_vertices - 1][0], corners[0][0]], [corners[pol.n_vertices - 1][1], corners[0][1]], c='pink')
        #
        # plt.scatter(np.mean(corners[0:pol.n_vertices, 0]), np.mean(corners[0:pol.n_vertices, 1]), c='pink')

        for i in range(1, 4):
            plt.plot([corners[i-1][0], corners[i][0]], [corners[i-1][1], corners[i][1]], c='blue')
        plt.plot([corners[-1][0], corners[0][0]], [corners[-1][1], corners[0][1]], c='blue')

        break

    plt.show()
    # TODO remove!
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
