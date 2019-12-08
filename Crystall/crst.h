#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <math.h>


typedef int errno_t;
typedef double Dist_t;

extern const double EPS;

typedef struct
{
  double x;
  double y;
  double z;
} Vector_t;

typedef struct
{
  Dist_t dist;
  Vector_t v_cs;
  size_t num;
} Bond_t;

typedef struct
{
  Vector_t v;
  Bond_t* bonds;
  size_t num_coord_s;
  size_t size;
} Graph_t;


errno_t read_data(FILE* fdata, size_t* num_bs, Vector_t* bases, size_t* num_vt, Vector_t* trans);
