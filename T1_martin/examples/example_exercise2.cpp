#include <stdio.h>
#include <math.h>

typedef struct vec2
{
    float x, y;
} vec2;

vec2
vec2_create(float x, float y)
{
    vec2 v;
    v.x = x; v.y = y;
    return v;
}

void
vec2_add(vec2 *res, vec2 v0, vec2 v1)
{
    res->x = v0.x + v1.x;
    res->y = v0.y + v1.y;
}

void
vec2_sub(vec2 *res, vec2 v0, vec2 v1)
{
    res->x = v0.x - v1.x;
    res->y = v0.y - v1.y;
}

void
vec2_mul(vec2 *res, vec2 v, float s)
{
    res->x = v.x * s;
    res->y = v.y * s;
}

void
vec2_div(vec2 *res, vec2 v, float s)
{
    res->x = v.x / s;
    res->y = v.y / s;
}

float
vec2_dot(vec2 v0, vec2 v1)
{
    return (v0.x * v1.x) + (v0.y * v1.y);
}

float
vec2_cross(vec2 v0, vec2 v1)
{
    return (v0.x * v1.y) - (v1.x * v0.y);
}

float
vec2_mag(vec2 v)
{
    return sqrtf(vec2_dot(v, v));
}

void
vec2_norm(vec2 *res, vec2 v)
{
    vec2_div(res, v, vec2_mag(v));
}

void
vec2_set(vec2 *vec, float x, float y)
{
    vec->x = x;
    vec->y = y;
}


void
vec2_list_print(vec2 vec_list[], int num_vectors)
{
    for(int i=0; i<num_vectors; ++i)
    {
        printf("{%f, %f}, ", vec_list[i].x, vec_list[i].y);
    }
    printf("\n");
}



int
is_convex(int num_points, vec2 point_list[])
{
    // Function to determine if a polygon is convex. For each consecutive pair
    // of edges of the polygon (each triplet of points), compute the
    // z-component of the cross product of the vectors defined by the edges
    // pointing towards the points in increasing order. The polygon is convex
    // if the z-components of the cross products all have the same sign.
    // WARNING: this function fails for self-intersecting polygons.
    // A -> (v2 - v1), B -> (v3 - v2), A x B
    if(num_points < 4) return 1;

    int sign = 0;
    for(int i=0; i<num_points; ++i)
    {
        vec2 v1, v2;
        vec2_sub(&v1, point_list[(i+1)%num_points], point_list[i]);
        vec2_sub(&v2, point_list[(i+2)%num_points], point_list[(i+1)%num_points]);

        float cross_product = vec2_cross(v1, v2);

        if(i == 0) sign = cross_product > 0;
        else if(sign != (cross_product > 0)) return 0;
    }
    return 1;
}

int
is_inside_convex(int num_points, vec2 point_list[], vec2 point_check)
{
    int sign = 0;
    for(int i=0; i<num_points; ++i)
    {
        vec2 v1, v2;
        vec2_sub(&v1, point_list[i], point_check);
        vec2_sub(&v2, point_list[(i+1)%num_points], point_check);

        // this would also work
        // vec2_sub(&v1, point_list[i], point_check);
        // vec2_sub(&v2, point_list[(i+1)%num_points], point_list[i]);

        float cross_product = vec2_cross(v1, v2);

        if(i == 0) sign = cross_product >= 0;
        else if(sign != (cross_product >= 0)) return 0;
    }

    return 1;
}


int
is_inside_concave(int num_points, vec2 point_list[], vec2 point_check)
{
    int inside = 0;
    for(int i = 0; i < num_points; ++i) {
        vec2 v1 = point_list[i];
        vec2 v2 = point_list[(i+1)%num_points];

        // test if point is between edge's ends in the y axis
        int considered = (v1.y > point_check.y) != (v2.y > point_check.y);
        // test if point is in a half-plane to the left of extended edge.
        // The value on the right of the comparison is the x coordinate of a
        // point on the edge at the same y coordinate as the intersection
        // point. This uses the 2d vector determinant form of a line equation.
        // Given two points that form a line, A = (x1, y1) and B = (x2, y2),
        // and a point P = (x, y) that lies on the line:
        // (x - x1) * (y2 - y1) - (y - y1) * (x2 - x1) = 0
        // (x - x1) * (y2 - y1) = (y - y1) * (x2 - x1)
        // x - x1 = ((y - y1) * (x2 - x1)) / (y2 - y1)
        // x = x1 + ((y - y1) * (x2 - x1)) / (y2 - y1)
        // where y = P.y, coordinate of point we wish to compare
        int in_half_plane = point_check.x < ((((v2.x - v1.x) * (point_check.y - v1.y)) / (v2.y - v1.y)) + v1.x);
        int intersect = considered && in_half_plane;
        if(intersect) inside = !inside;
    }

    return inside;
}

int main(void)
{
    int num_points;
    int convex;

    // CONVEX EXAMPLES
    printf("CONVEX TESTS\n");
    // 3 sides
    vec2 convex1[] = {{0,0}, {5,5}, {5,0}};
    num_points = sizeof(convex1)/sizeof(convex1[0]);
    vec2_list_print(convex1, num_points);
    convex = is_convex(num_points, convex1);

    // 4 sides
    printf("is convex: %d\n", convex);
    vec2 convex2[] = {{0,0}, {0,5}, {5,5}, {5,0}};
    num_points = sizeof(convex2)/sizeof(convex2[0]);
    vec2_list_print(convex2, num_points);
    convex = is_convex(num_points, convex2);
    printf("is convex: %d\n", convex);

    // 5 sides
    vec2 convex3[] = {{0,0}, {0,5}, {5,4}, {5,1}, {3,0}};
    num_points = sizeof(convex3)/sizeof(convex3[0]);
    vec2_list_print(convex3, num_points);
    convex = is_convex(num_points, convex3);
    printf("is convex: %d\n", convex);
    printf("\n");


    // CONCAVE EXAMPLES
    printf("CONCAVE TESTS\n");
    // 4 sides
    vec2 concave1[] = {{0,0}, {0,5}, {2,2}, {5,0}};
    num_points = sizeof(concave1)/sizeof(concave1[0]);
    vec2_list_print(concave1, num_points);
    convex = is_convex(num_points, concave1);
    printf("is convex: %d\n", convex);

    // 5 sides
    vec2 concave2[] = {{0,0}, {0,5}, {5,4}, {5,1}, {3,2}};
    num_points = sizeof(concave2)/sizeof(concave2[0]);
    vec2_list_print(concave2, num_points);
    convex = is_convex(num_points, concave2);
    printf("is convex: %d\n", convex);
    printf("\n");


    int is_inside;
    // POINT INSIDE EXAMPLES
    // convex
    printf("INSIDE CONVEX TESTS\n");
    vec2 polygon_convex[] = {{0,0}, {10,0}, {10,10}, {0,10}};
    num_points = sizeof(polygon_convex)/sizeof(polygon_convex[0]);
    printf("convex polygon: ");
    vec2_list_print(polygon_convex, num_points);
    printf("\n");

    vec2 test1[] = {{0,0}, {10,5}, {5,5}, {3,3}, {-5,0}, {-3,-3}};
    for(int i=0; i<sizeof(test1)/sizeof(test1[0]); ++i)
    {
        is_inside = is_inside_convex(num_points, polygon_convex, test1[i]);
        printf("(%f, %f): %d\n", test1[i].x, test1[i].y, is_inside);
    }
    printf("\n");

    // concave
    printf("INSIDE CONCAVE TESTS\n");
    vec2 polygon_concave[] = {{4,1}, {8,1}, {8,5}, {5,4}, {3,6}, {2,3}, {2,2}};
    num_points = sizeof(polygon_concave)/sizeof(polygon_concave[0]);
    printf("concave polygon: ");
    vec2_list_print(polygon_concave, num_points);
    printf("\n");

    int is_inside2;
    vec2 test2[] = {{4,3}, {3,5}, {2,2}, {6,1}, {8,3}, {3,1}, {1,4}, {6,5}};
    for(int i=0; i<sizeof(test2)/sizeof(test2[0]); ++i)
    {
        is_inside = is_inside_concave(num_points, polygon_concave, test2[i]);
        printf("(%f, %f): %d\n", test2[i].x, test2[i].y, is_inside);
    }

    return 0;
}
