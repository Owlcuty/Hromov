#include "sort.h"

int main()
{
  FILE* fin = fopen("fdata", "w");
  generation(fin);
  fclose(fin);

  fin = fopen("fdata", "r");

  FILE* fcsv = fopen("res.csv", "w");

  run(fin, fcsv);

  fclose(fin);
  fclose(fcsv);
  return 0;
}
