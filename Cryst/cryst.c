#include "cryst.h"


errno_t fill_base_trans(Pos_t** vectors, int* num_base, Vector_t* *trans, int* num_trans, Pos_t** last, int* num_coord, FILE* fdata)
{
    printf("%i:: Here" "\n", __LINE__);
  *num_trans = 0;
  fscanf(fdata, "%d", num_trans);
  printf("%i:: %d *transes" "\n", __LINE__, *num_trans);
  *trans = (Vector_t*)calloc(*num_trans, sizeof(**trans));
  if (!trans)
  {
    errno = ENOMEM;
    perror("trans");
    return errno;
  }

    printf("%i:: Here" "\n", __LINE__);
  for (Vector_t* tran = *trans; tran < *trans + *num_trans; tran ++)
  {
    fscanf(fdata, "%lf,%lf,%lf", &(tran->x), &(tran->y), &(tran->z));
    printf("%i:: (%lf, %lf, %lf)" "\n", __LINE__, tran->x, tran->y, tran->z);
  }

  *num_base = 0;
  fscanf(fdata, "%d", num_base);
  *vectors = (Pos_t*)calloc(*num_trans * *num_base * 30, sizeof(**vectors));
  if (!*vectors)
  {
    errno = ENOMEM;
    perror("*vectors");
    return errno;
  }
  *last = *vectors;
  for (Pos_t* v = *vectors; v < *vectors + *num_base; v ++)
  {
    fscanf(fdata, "%lf,%lf,%lf", &(v->x), &(v->y), &(v->z));
  }

  *num_coord = 0;
  fscanf(fdata, "%d", num_coord);

  (*last) += *num_base;

  return 0;
}

void translate(const Pos_t vector, Pos_t* tov, const Vector_t trans)
{
  tov->x = vector.x + trans.x;
  tov->y = vector.y + trans.y;
  tov->z = vector.z + trans.z;
}

errno_t build_vectors(Pos_t* vectors, const int num_base, Pos_t* last, Vector_t* trans, const int num_trans, const int num_coord)
{
  assert(vectors);
  assert(num_base  != 0);
  assert(last);
  assert(trans);
  assert(num_trans != 0);
  assert(num_coord != 0);

  int N = num_base * num_trans * (num_coord + 2);
  for (Pos_t* v = vectors; v < vectors + N; v ++)
  {
    if (last >= vectors + N - 2) break;
    for (Vector_t* tran = trans; tran < trans + num_trans; tran ++)
    {
      translate(*v, last, *tran);
      for (Pos_t* tmp = vectors; tmp < last; tmp ++)
        {
          if (!vectorcmp(*tmp, *last))
          {
            goto contue;
          }
        }

      last++;
contue:
      continue;
    }
  }

  return 0;
}

double get_dist(const Pos_t v1, const Pos_t v2)
{
  return sqrt((v1.x - v2.x) * (v1.x - v2.x) +
              (v1.y - v2.y) * (v1.y - v2.y) +
              (v1.z - v2.z) * (v1.z - v2.z));
}

void cmp_vectors(Vector_t* to, const Vector_t from)
{
  to->x = from.x;
  to->y = from.y;
  to->z = from.z;
}

int cmp_dist(const void* d1, const void* d2)
{
  const double dist1 = ((Dist_t*)d1)->dist;
  const double dist2 = ((Dist_t*)d2)->dist;

  printf("%i:: %lf ? %lf" "\n", __LINE__, dist1, dist2);
  if (dist1 < dist2)
  {
    return -1;
  }
  else if (dist1 - dist2 < 1e-6)
  {
    return 0;
  }
  else
  {
    return 1;
  }
}

errno_t build_dists(Dist_t* dists, Pos_t* vectors, const int N, const int num_coord)
{
  printf("%i:: Vector1Base (%lf, %lf, %lf)" "\n", __LINE__, vectors[0].x,
                                                            vectors[0].y,
                                                            vectors[0].z);
  double dist_now = 0;
  int num_dist = 0;
  int was_dist = 0;

  for (Pos_t* v = vectors + 1; v < vectors + N; v ++)
  {
    was_dist = 0;
    dist_now = get_dist(*vectors, *v);
    for (Dist_t* dist = dists; dist < dists + num_dist; dist ++)
    {
      if (dist->dist == dist_now)
      {
        was_dist = 1;
        cmp_vectors(&((dist->coord)[dist->size++]), *v);
        break;
      }
    }
    if (!was_dist)
    {
      dists[num_dist].coord = (Pos_t*)calloc(N, sizeof(*dists[num_dist].coord));
      if (!dists[num_dist].coord)
      {
        errno = ENOMEM;
        perror("dists[0]->coord");
        return errno;
      }
      cmp_vectors(dists[num_dist].coord, *v);
      dists[num_dist].size = 1;
      dists[num_dist].dist = dist_now;
      num_dist++;
    }
  }

  qsort(dists, num_coord, sizeof(Dist_t), cmp_dist);

  return 0;
}

void print_vector(const Vector_t v)
{
  printf("(%lf, %lf, %lf)", v.x, v.y, v.z);
}

int vectorcmp(const Vector_t v1, const Vector_t v2)
{
  return !(v1.x == v2.x && v1.y == v2.y && v1.z == v2.z);
}

errno_t dump_coords(Dist_t* dists, const int num_coord)
{
  int now_coord = 1;
  for (Dist_t* dist = dists; dist < dists + num_coord; dist ++)
  {
    printf("\n");
    printf("%i:: %d coord sphere (Dist = %lf):" "\n", __LINE__, now_coord++, dist->dist);
    printf("  %i:: %d atoms:" "\n", __LINE__, dist->size);
    for (Pos_t* v = dist->coord; v < dist->coord + dist->size; v ++)
    {
      if (v == dist->coord) printf("    ");
      else printf(" ");
      print_vector(*v);
      printf(" ");
    }
    printf("\n");
  }

  return 0;
}
