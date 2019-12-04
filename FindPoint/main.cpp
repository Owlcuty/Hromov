#include <stdio.h>
#include <stdlib.h>

enum Fails {
  ScanErr = -111,
  FindErr = -112
};

int set_mtrx(int* mtrx, int rows, int cols);
int find_maxes(int* mtrx, int rows, int cols, int* extr);

int main()
{
  int rows = 0, cols = 0;
  printf("%i:: Programm to find saddle point of matrix." "\n" "Enter num of rows and cols [::rows:: ::cols::]: ", __LINE__) ;
  scanf("%i%i", &rows, &cols);

  int* mtrx = (int*)(calloc(rows * cols, sizeof(*mtrx)));
  if (!set_mtrx(mtrx, rows, cols))
  {
    return ScanErr;
  }

  int* extr = (int*)(calloc(2 * (rows + cols), sizeof(*extr)));
  find_maxes(mtrx, rows, cols, extr);

  for (int y = 0; y < rows; ++y)
  {
    for (int x = 0; x < cols; ++x)
    {
      printf("%i ", mtrx[y * rows + x]);
    }
    printf("\n");
  }

  printf("\n");

  for (int y = 0; y < rows; ++y) {
    printf("%i ", extr[y]);
  }

  printf("\n");

  for (int x = rows; x < rows + cols; ++x) {
    printf("%i ", extr[x]);
  }

  printf("\n");

  for (int y = rows + cols; y < 2 * rows + cols; ++y) {
    printf("%i ", extr[y]);
  }

  printf("\n");

  for (int x = 2 * rows + cols; x < 2 * (rows + cols); ++x) {
    printf("%i ", extr[x]);
  }

  printf("\n");


  free(mtrx);
}


int set_mtrx(int* mtrx, int rows, int cols)
{
  int nums = 0;

  for (int y = 0; y < rows; y ++)
  {
    for (int x = 0; x < cols; x ++)
      *(mtrx + (y * cols) + x) = x * x - y * y;
      /* nums += scanf("%i", mtrx + (y * cols) + x); */
  }

  /* return nums; */
  return 1;
}

int find_maxes(int* mtrx, int rows, int cols, int* extr)
{

  for (int y = 0; y < rows; y ++)
  {
    for (int x = 0; x < cols; x ++)
    {
      if (mtrx[y * rows + x] < extr[y])
        extr[y] = mtrx[y * rows + x];
      if (mtrx[y * rows + x] > extr[rows + y])
        extr[rows + y] = mtrx[y * rows + x];
    }
  }

  for (int x = 0; x < cols; x ++)
  {
    for (int y = 0; y < rows; y ++)
    {
      if (mtrx[y * rows + x] < extr[rows + cols + y])
        extr[rows + cols + y] = mtrx[y * rows + x];
      if (mtrx[y * rows + x] > extr[2 * rows + cols + y])
        extr[2 * rows + cols + y] = mtrx[y * rows + x];
    }
  }

}
