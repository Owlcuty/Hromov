#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <assert.h>

typedef double pos_t;
typedef double dist_t;

double find_int(double a, double b, double eps, double (* f) (double x));
double trapez_square(pos_t y1, pos_t y2, dist_t step);
double sum_squares(pos_t fromX, pos_t toX, dist_t step,  double (* f) (double x));
double calc(double x);
double fmaxabs(double a, double b);

int main() // Add ERRORS
{

  // scanf("%d", &x);

  double ans = find_int(0, acos(-1), 1e-7, &calc);

  printf("ans = %f\n", ans);
}


double calc(double x)
{
  // return exp(x) - exp(1);
  return sin(x);
}

double fmaxabs(double a, double b)
{
  return fabs(a) > fabs(b) ? a : b;
}

double find_int(double a, double b, double eps, double (* f) (double x))
{
  dist_t step = 1;

  // printf("%i:: First[%lf]{%lf} ~ Second[%lf]{%lf}" "\n", __LINE__, a, trapez_square(f(a + step), f(a), step), b, trapez_square(f(a), f(a - step), step));
  // printf("%i:: Integral[%lf -> %lf][step = %lf] = %lf" "\n", __LINE__, a, b, step, sum_squares(a, b, step, f));

  for (; fabs(trapez_square(f(a + 2 * step), f(a + step), step) - trapez_square(f(a + step), f(a),            step) -
                 trapez_square(f(b),            f(b - step), step) - trapez_square(f(b - step), f(b - 2 * step), step)) > eps; step /= 2);

  return sum_squares(a, b, step, f);
}

double trapez_square(pos_t y1, pos_t y2, dist_t step)
{
  // assert(y1 < y2);

  return (y1 + y2) * step / 2;
}

double sum_squares(pos_t fromX, pos_t toX, dist_t step,  double (* f) (double x))
{
  double sum = 0;

  for (pos_t x = fromX + step; x <= toX; x += step)
  {
    sum += trapez_square(f(x), f(x - step), step);
  }

  return sum;
}
