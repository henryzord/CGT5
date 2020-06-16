from geometries import Polygon, Map


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
    for concave in concavos:
        print(concave.toConvexHull())

    # ------------------------------------------- #

    some_polygons = [
        Polygon([(0, 4), (3, 0), (4, 3)]),
        Polygon([(3, 0), (7, 2), (4, 3)]),
        Polygon([(4, 3), (7, 2), (7, 5)]),
        Polygon([(0, 4), (4, 3), (3, 7)]),
        Polygon([(4, 3), (3, 7), (7, 5)])
    ]

    myMap = Map(polygons=some_polygons)
    print('n_polygons:', myMap.n_polygons)

    for i, pol in enumerate(myMap.polygons):
        print('polygon #%d: %r' % (i, pol))

    some_points = [(3, 2), (5, 2), (6, 3), (2, 5), (5, 5)]
    for point in some_points:
        print(myMap.checkInside(point))

    # for i in range(len(some_points)):
    #     print('point %r is inside geometries %d' % (some_points[i], indices[i]))

    # pol = Polygon([(0, 4), (3, 0), (4, 3)])
    # pol.increase_meter(how_much=3)
    # print(pol.n_vertices)
    # print(pol.vertices)


if __name__ == '__main__':
    main()
