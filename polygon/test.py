import polygons


def main():
    pontos = [(1, 5), (5, 1), (8, 5), (6, 4), (4, 3)]

    convexos = [[(5, 0), (3, 2), (5, 6), (7, 3)], [(2, 0), (2, 6), (7, 6), (7, 0)]]
    corretude_convexos = [[0, 1, 0, 1, 1], [0, 1, 0, 1, 1]]
    concavos = [[(2, 0), (2, 6), (7, 6), (7, 0), (5, 2)], [(2, 0), (4, 6), (9, 4), (6, 0), (5, 2)]]
    corretude_concavos = [[0, 0, 0, 1, 1], [0, 0, 0, 1, 1]]

    # testa se um ponto está dentro de polígono côncavo/convexo
    # for i, convex in enumerate(convexos):
    #     for ponto, cor in zip(pontos, corretude_convexos[i]):
    #         res = polygons.isInsideConvexPolygon(convex, ponto)
    #         if res != cor:
    #             print('point %r is wrong for polygon %r' % (ponto, convex))
    #
    # for i, concave in enumerate(concavos):
    #     for ponto, cor in zip(pontos, corretude_concavos[i]):
    #         res = polygons.isInsideConcavePolygon(concave, ponto)
    #         if res != cor:
    #             print('point %r is wrong for polygon %r' % (ponto, concave))

    # testa convex hull
    # for concave in concavos:
    #     print(polygons.convexHull(concave))

    some_polygons = [
        [(0, 4), (3, 0), (4, 3)],
        [(3, 0), (7, 2), (4, 3)],
        [(4, 3), (7, 2), (7, 5)],
        [(0, 4), (4, 3), (3, 7)],
        [(4, 3), (3, 7), (7, 5)]
    ]

    some_points = [(3, 2), (5, 2), (6, 3), (2, 5), (5, 5)]
    polygons.slabAlgorithm(some_polygons, some_points)


if __name__ == '__main__':
    main()
