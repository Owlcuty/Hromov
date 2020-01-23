#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <math.h>

typedef int errno_t;

typedef struct
{
  double x;
  double y;
  double z;
} Vector_t;

typedef Vector_t Pos_t;

typedef struct
{
  double dist;
  Pos_t* coord;
  int size;
} Dist_t;

errno_t fill_base_trans(Pos_t** vectors, int* num_base, Vector_t** trans, int* num_trans, Pos_t** last, int* num_coord, FILE* fdata);
void translate(const Pos_t vector, Pos_t* tov, const Vector_t trans);
errno_t build_vectors(Pos_t* vectors, const int num_base, Pos_t* last, Vector_t* trans, const int num_trans, const int num_coord);
double get_dist(const Pos_t v1, const Pos_t v2);
void cmp_vectors(Vector_t* to, const Vector_t from);
errno_t build_dists(Dist_t* dists, Pos_t* vectors, const int N, const int num_coord);
void print_vector(const Vector_t v);
errno_t dump_coords(Dist_t* dists, const int num_coord);
int vectorcmp(const Vector_t v1, const Vector_t v2);
