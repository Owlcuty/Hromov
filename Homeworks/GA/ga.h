#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <errno.h>

extern const unsigned int Multi_N;
extern const size_t MAXERA;
extern const long double EPS;
typedef double* Gen_t;

int find_desired(Gen_t v1, Gen_t v2, int D, double (*func)(Gen_t vector, int D));
int generation(Gen_t* gens, int D);
int mutation_cross(Gen_t *v, Gen_t v1, Gen_t v2, Gen_t v3, int D, double F, double R, double (*func)(Gen_t vector, int D));
int diff_evolution(int D, double F, double R, double eps, double (*func)(Gen_t vector, int D), FILE* fout);
int inversion(Gen_t *vector, Gen_t v, int D, double eps, double (*func)(Gen_t vector, int D));
void print_xmean_disp(FILE* fout, Gen_t *gens, int D, double *disp);
