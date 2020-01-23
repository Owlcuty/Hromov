#include "sort.h"

int main()
{
  FILE* fin = fopen("fdata", "w");
  generation(fin);
  fclose(fin);

  fin = fopen("fdata", "r");

  FILE* fcsv = fopen("res2.csv", "w");

  errno = EINVAL;

  run(fin, fcsv);

  fclose(fin);
  fclose(fcsv);
  return 0;
}
