#include <stdio.h>
#include <math.h>

enum Methods
{
  Bisection = 1, Secant, Both
};

double func = exp;

double apprx(double x, double param);

int solve(int method, double a, double b, double eps, double param);
double solve_bis(double a, double b, double eps, double param);
double solve_chord(double a, double b, double eps, double param);

int main()
{
  double param = 0;

  printf("%i:: The program to solve function f(x) - c = 0." "\n" "Enter param 'c' to find x: ", __LINE__);
  scanf("%lf", &param);

  double a = 0, b = 0;

  printf("%i:: Enter start of segment: ", __LINE__);
  scanf("%lf", &a);

  printf("%i:: Enter end of segment: ", __LINE__);
  scanf("%lf", &b);

  double eps = 0;

  printf("%i:: Enter allowable error: ", __LINE__);
  scanf("%lf", &eps);

  int method = 0;

  printf("%i:: Enter num of method ( 1 - Bisection; 2 - Secant; 3 - Both ):" "\n", __LINE__);
  scanf("%i", &method);

  solve(Bisection, a, b, eps, param);
  solve(Secant,    a, b, eps, param);
  solve(Both,      a, b, eps, param);
}

double apprx(double x, double param)
{
  double f = func(x) - param;
  return f;
}

int solve(int method, double a, double b, double eps, double param)
{
  if (apprx(a, param) * apprx(b, param) >= 0)
    return 1;

  switch (method)
  {
    case Bisection:
      solve_bis(a, b, eps, param);
      break;
    case Both:
      solve_bis(a, b, eps, param);
    case Secant:
      solve_chord(a, b, eps, param);
      break;
    default:
      return 1;
  }

  //Out

  return 0;
}

double solve_bis(double a, double b, double eps, double param)
{
  //
}

double solve_chord(double a, double b, double eps, double param);
