#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <limits.h>

typedef long long ll;

double Taylor_exp(double x, int* power, double eps);

int main()
{
  double x = 0;
  double eps = 0;

  printf("Enter eps: ");
  scanf("%lf", &eps);

  printf("x for exp(x) = ");
  scanf("%lf", &x);
  printf("\n");

  int powerTaylor = 0;

  printf("%i:: exp(%lf) = %lf", __LINE__, x, Taylor_exp(x, &powerTaylor, eps));
  printf("; Pow of Taylor eq = %i" "\n", powerTaylor);
}

double Taylor_exp(double x, int* power, double eps)
{

  double e = 0;
  ll fact = 1;
  double numerator = 0;

  while (abs(exp(x) - e) >= eps && fact < LLONG_MAX && (numerator = pow(x, *power)) < LLONG_MAX)
  {
    e += numerator / fact;

    printf("%i:: Pow of Taylor equation = %i, exp(%lf) = %lf; Eps = %f" "\n", __LINE__, *power, x, e, abs(e - exp(x)));
    (*power)++;
    fact *= (*power);
  }

  printf("%i:: Eps = %f" "\n", __LINE__, abs(e - exp(x)));
  return e;
}


