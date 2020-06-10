import polygons


def main():
    pontos = [(1, 5), (5, 1), (8, 5), (6, 4), (4, 3)]

    convexos = [[(5, 0), (3, 2), (5, 6), (7, 3)], [(2, 0), (2, 6), (7, 6), (7, 0)]]
    corretude_convexos = [[0, 1, 0, 1, 1], [0, 1, 0, 1, 1]]
    concavos = [[(2, 0), (2, 6), (7, 6), (7, 0), (5, 2)], [(2, 0), (4, 6), (9, 4), (6, 0), (5, 2)]]
    corretude_concavos = [[0, 0, 0, 1, 1], [0, 0, 0, 1, 1]]

    for i, convex in enumerate(convexos):
        for ponto, cor in zip(pontos, corretude_convexos[i]):
            res = polygons.isInsideConvexPolygon(len(convex), convex, ponto)
            if res != cor:
                print('point %r is wrong for polygon %r' % (ponto, convex))

    for i, concave in enumerate(concavos):
        for ponto, cor in zip(pontos, corretude_concavos[i]):
            res = polygons.isInsideConcavePolygon(len(concave), concave, ponto)
            if res != cor:
                print('point %r is wrong for polygon %r' % (ponto, concave))


if __name__ == '__main__':
    main()
