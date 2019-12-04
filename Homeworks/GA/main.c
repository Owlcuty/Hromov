#include "ga.c"

enum Vars {
  X,
  Y
};

double func(Gen_t vector, int D)
{
  double ans = 100 * (vector[Y] - vector[X] * vector[X]) * (vector[Y] - vector[X] * vector[X]) + (1 - vector[X]) * (1 - vector[X]);
  /** return (vector[X] - 11) * (vector[X] - 11); */
  /** printf("%i:: ans{%lf}" "\n", __LINE__, ans); */
  return ans;
}

int main()
{
  FILE* fout = fopen("res.out", "w");
  double F = 0;

  printf("Value of mutation's force: ");
  if (!scanf("%lf", &F))
  {
    errno = EINVAL;
    perror("F");
    return errno;
  }

  double R = 0;

  printf("Mutatuion probability: ");
  if (!scanf("%lf", &R))
  {
    errno = EINVAL;
    perror("R");
    return errno;
  }

  int D = 0;

  printf("Num of basises: ");
  if (!scanf("%d", &D))
  {
    errno = EINVAL;
    perror("D");
    return errno;
  }

  if (diff_evolution(D, F, R, 1e-4, &func, fout))
  {
    return errno;
  }

  printf("%i:: Oki-doki" "\n", __LINE__);
  fclose(fout);
  return 0;
}
