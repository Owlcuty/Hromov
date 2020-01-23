#include <stdio.h>
#include <math.h>

enum Methods
{
  Bisection = 1, Secant, Both
};

double (*func)(double arg) = &sin;

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

  printf("%i:: Enter num of method ( 1 - Bisection; 2 - Secant; 3 - Both ): ", __LINE__);
  scanf("%i", &method);

  solve(method, a, b, eps, param);
}

double apprx(double x, double param)
{
  double f = func(x) - param;
  return f;
}

int solve(int method, double a, double b, double eps, double param)
{
  if (apprx(a, param) * apprx(b, param) >= 0)
  {
    return 1;
  }

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
  double temp = 0;
  double ans = 0;
  while(1)
  {
    temp = b;
    b = (a + temp) / 2;

    if (fabs(apprx(b, param)) < eps)
    {
      ans = b;
      break;
    };
    if (fabs(apprx(a, param)) < eps)
    {
      ans = a;
      break;
    };
    if (apprx(a, param) * apprx(b, param) > 0)
    {
      a = b;
      b = temp;
    }
  }
  printf("%e\n", ans);

  return 1;
}

double solve_chord(double a, double b, double eps, double param)
{
  double ans = 0, k = 0, h = 0; // y = kx + h
  double x0 = 0;

  while (1)
  {
    k = (apprx(a, param) / (apprx(b, param) - apprx(a, param))) * (b - a);
    x0 = a - k;
    printf("%i:: x0 = {%lf}" "\n", __LINE__, x0);

    if (fabs(x0 - a) < eps)
    {
      ans = x0;
      break;
    }
    if (fabs(x0 - b) < eps)
    {
      ans = x0;
      break;
    }

    if (apprx(x0, param) * apprx(b, param) > 0)
    {
      b = x0;
      x0 = a;
    }
      a = x0;
  }
  printf("%e\n", ans);
  return 1;
}
