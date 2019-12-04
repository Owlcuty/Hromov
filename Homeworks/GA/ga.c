#include "ga.h"

const unsigned int Multi_N = 10;
/** const size_t MAXERA = 3000; */
const long double EPS = 1e-6;

int find_desired(const Gen_t v1, const Gen_t v2, const int D, double (*func)(Gen_t vector, int D))
{
  assert(v1);
  assert(v2);
  assert(func);

  if (func(v1, D) < func(v2, D))
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int generation(Gen_t* gens, const int D)
{
  assert(gens);

  for (int indiv = 0; indiv < Multi_N * D; indiv ++)
  {
    gens[indiv] = (Gen_t)calloc(D, sizeof(Gen_t));
    if (!gens[indiv])
    {
      errno = ENOMEM;
      perror("gens[indiv]");
      return errno;
    }
    for (int basis = 0; basis < D; basis ++)
    {
      gens[indiv][basis] = rand() % 300;
      /** printf("%lf, ", gens[indiv][basis]); */
    }
    /** printf("\n"); */
  }

  return 0;
}

int mutation_cross(Gen_t *v, Gen_t v1, Gen_t v2, Gen_t v3, const int D, const double F, const double R, double (*func)(Gen_t vector, int D))
{
  assert(v);
  assert(v1);
  assert(v2);
  assert(v3);
  assert(func);

  Gen_t new_v = (Gen_t)calloc(D, sizeof(Gen_t));

  if (!new_v)
  {
    errno = ENOMEM;
    perror("new_v");
    return errno;
  }

  for (int basis = 0; basis < D; basis ++)
  {
    /** printf("%i:: V = %lf + %lf * (%lf - %lf)" "\n", __LINE__, v1[basis], F, v2[basis], v3[basis]); */
    /** printf("%i:: %lf" "\n", __LINE__, (double)(rand()) / RAND_MAX); */
    if (((double)(rand()) / RAND_MAX) <= R) continue;
    new_v[basis] = v1[basis] + F * (v2[basis] - v3[basis]);
  }

  if (find_desired(new_v, *v, D, func))
  {
    /** printf("%i:: New min" "\n", __LINE__); */
    Gen_t temp = *v;
    *v = new_v;
    free(temp);
  }
  else
  {
    /** printf("%i:: Old min" "\n", __LINE__); */
    free(new_v);
  }

  return 0;
}

int inversion(Gen_t *vector, Gen_t v, const int D, const double eps, double (*func)(Gen_t vector, int D))
{
  assert(vector);
  assert(func);

  static int sign = 0;
  sign = (sign + 1) % 2;

  Gen_t new_v = (Gen_t)calloc(D, sizeof(Gen_t));

  if (!new_v)
  {
    errno = ENOMEM;
    perror("new_v");
    return errno;
  }

  for (int basis = 0; basis < D; basis ++)
  {
    if (sign)
      new_v[basis] = v[basis] + eps;
    else
      new_v[basis] = v[basis] - eps;
  }

  if (find_desired(new_v, *vector, D, func))
  {
    /** printf("%i:: New min" "\n", __LINE__); */
    Gen_t temp = *vector;
    *vector = new_v;
    free(temp);
  }
  else
  {
    /** printf("%i:: Old min" "\n", __LINE__); */
    free(new_v);
  }

  return 0;
}

void print_xmean_disp(FILE* fout, Gen_t *gens, const int D, double *disper)
{
  assert(fout);
  assert(disper);

  *disper = 0;


  for (int basis = 0; basis < D; basis ++)
  {
    fprintf(fout, "%d component: ", basis);
    double mean = 0;
    double disp = 0;
    for (int indiv = 0; indiv < Multi_N * D; indiv ++)
    {
      mean += gens[indiv][basis];
    }
    mean /= Multi_N * D;
    fprintf(fout, "(MEAN: %lf, ", mean);

    for (int indiv = 0; indiv < Multi_N * D; indiv ++)
    {
      disp += (gens[indiv][basis] - mean) * (gens[indiv][basis] - mean);
    }
    disp /= Multi_N * D;
    *disper += disp;
    fprintf(fout, "DISP: %lf)\n", disp);
  }
  *disper /= D;
}

int diff_evolution(const int D, const double F, const double R, const double eps,
    double (*func)(Gen_t vector, int D), FILE* fout)
{
  assert(fout);

  FILE * fileTZ = fopen("fileTZ.out", "w");
  if (!fileTZ)
  {
    errno = ENOENT;
    return errno;
  }

  double min = 1e8;

  Gen_t* gens = (Gen_t*)calloc(Multi_N * D, sizeof(*gens));

  if (generation(gens, D))
  {
    return errno;
  }

  size_t MAXERA = 30000;
  int scattered = 1;

  for (int era = 0; era < MAXERA; era ++)
  {
    fprintf(fout, "Era{%d}: --------------------\n", era);
    /** printf("%i:: Multi_N{%d} -- D{%d}" "\n", __LINE__, Multi_N, D); */
    for (int indiv = 0; indiv < Multi_N * D; indiv ++)
    {
      /** printf("%i:: Indiv{%i}, ", __LINE__, indiv); */
      int v1 = 0, v2 = 0, v3 = 0;
      do
      {
        v1 = rand() % (Multi_N * D);
      } while (indiv == v1);
      do
      {
        v2 = rand() % (Multi_N * D);
      } while (indiv == v2 || v1 == v2);
      do
      {
        v3 = rand() % (Multi_N * D);
      } while (indiv == v3 || v1 == v3 || v2 == v3);

      if (mutation_cross(&(gens[indiv]), gens[v1], gens[v2], gens[v3], D, F, R, func))
      {
        return errno;
      }
      /** printf("\n"); */
      if (inversion(&(gens[indiv]), gens[indiv], D, 0.3, func))
      {
        return errno;
      }
      /** printf("\n"); */

      // ---
    /** for (int basis = 0; basis < D; basis ++) */
    /** { */
      /** printf("%lf, ", gens[indiv][basis]); */
    /** } */
    /** printf("\n"); */
      // ----

    if (func(gens[indiv], D) < min)
    {
      min = func(gens[indiv], D);
    }
      /** printf("Era{%i}:: gens[%i]: %lf" "\n", era, indiv, func(gens[indiv], D)); */
      fprintf(fout, "(%lf, %lf)" "\n", gens[indiv][0], gens[indiv][1]);
    }
    fprintf(fout, "\n");

    double disp = 0;
    fprintf(fileTZ, "%i:: ERA{%i}:" "\n", __LINE__, era);
    print_xmean_disp(fileTZ, gens, D, &disp);
    if (disp < EPS && scattered)
    {
      MAXERA = era * 2;
      fprintf(fileTZ, "---- %i:: DISP = 0. Scattered. ERA{%d/%zu} ----------" "\n", __LINE__, era, MAXERA);
      scattered = 0;
    }
  }

  /** printf("%i:: MIN = %lf" "\n", __LINE__, min); */

  free(gens);
  fclose(fileTZ);
  return 0;
}

