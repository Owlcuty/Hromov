#include "cryst.h"

int main(int argc, char* argv[])
{
  FILE* fdata = fopen(argv[1], "r");
  if (!fdata)
  {
    errno = ENOENT;
    perror("fdata(read)");
    return errno;
  }

  Pos_t* vectors  = NULL;
  Pos_t* last     = NULL;
  Vector_t* trans = NULL;
  int num_base, num_trans, num_coord;

  fill_base_trans(&vectors, &num_base, &trans, &num_trans, &last, &num_coord, fdata);
  build_vectors(vectors, num_base, last, trans, num_trans, num_coord);

  int N = num_base * num_trans * (num_coord + 2);
  printf("%i:: N = %d" "\n", __LINE__, N);

  Dist_t* dists = (Dist_t*)calloc(N, sizeof(*dists));
  if (!dists)
  {
    errno = ENOMEM;
    perror("dists");
    return errno;
  }

  build_dists(dists, vectors, N, num_coord);

  dump_coords(dists, num_coord);

  free(vectors);
  free(trans);
}
