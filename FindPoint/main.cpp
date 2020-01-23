#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum Fails {
  ScanErr = -111,
  FindErr = -112
};

int set_mtrx(int* mtrx, int rows, int cols);
int find_maxes(int* mtrx, int rows, int cols, int* extr, int* sedle, int* min, int* max);
int set_random_mtrx(int* mtrx, int rows, int cols);

int main(int argc, char* argv[])
{
  int rows = 0, cols = 0;
  printf("%i:: Programm to find saddle point of matrix." "\n" "Enter num of rows and cols [::rows:: ::cols::]: ", __LINE__) ;
  scanf("%i%i", &rows, &cols);

  int* mtrx = (int*)(calloc(rows * cols, sizeof(*mtrx)));
  switch (*(argv[1]))
  {
    case 'o':
      if (!set_mtrx(mtrx, rows, cols))
      {
        return ScanErr;;
      }
      break;
    case 'r':
      if (!set_random_mtrx(mtrx, rows, cols))
      {
        return ScanErr;
      }
      break;
    default:
      printf("%i:: No flags" "\n", __LINE__);
      return -1;
  }
/*   if (!set_mtrx(mtrx, rows, cols)) */
  // {
  //   return ScanErr;
  // }
  // if (!set_random_mtrx(mtrx, rows, cols))
  // {
  //   return ScanErr;
  // }
/*  */

  int* extr = (int*)(calloc(2 * (rows + cols), sizeof(*extr)));
  int sedle = 0, min = 0, max = 0;
  if (find_maxes(mtrx, rows, cols, extr, &sedle, &min, &max))
  {


  for (int y = 0; y < rows; ++y)
  {
    for (int x = 0; x < cols; ++x)
    {
      printf("%3i ", mtrx[y * rows + x]);
    }
    printf("\n");
  }

  printf("\n");

    printf("%i:: There is not seddle point" "\n", __LINE__);
    return 1;
  }

  for (int y = 0; y < rows; ++y)
  {
    for (int x = 0; x < cols; ++x)
    {
      printf("%3i ", mtrx[y * rows + x]);
    }
    printf("\n");
  }

  printf("\n");

  printf("%i:: Seddle point: %d" "\n", __LINE__, sedle);
  printf("%i:: Min {%d}. Max {%d}" "\n", __LINE__, min, max);

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

int find_maxes(int* mtrx, int rows, int cols, int* extr, int* sedle, int* min, int* max)
{
  assert(mtrx);
  assert(extr);
  assert(sedle);
  assert(min);
  assert(max);

  *min = *mtrx;
  *max = *mtrx;

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
      if (mtrx[y * rows + x] < *min)
      {
        *min = mtrx[y * rows + x];
      }
      if (*max < mtrx[y * rows + x])
      {
        *max = mtrx[y * rows + x];
      }
      if (mtrx[y * rows + x] > extr[rows + cols + x])
        extr[rows + cols + x] = mtrx[y * rows + x];
      if (mtrx[y * rows + x] < extr[2 * rows + cols + x])
        extr[2 * rows + cols + x] = mtrx[y * rows + x];
    }
  }

  for (int y = 0, x = rows + cols; y < rows && x < 2 * rows + cols; ++y, ++x) {
    if (extr[y] == extr[x])
    {
      *sedle = extr[y];
      return 0;
    }
  }

  return 1;
}

int set_random_mtrx(int* mtrx, int rows, int cols)
{
  int nums = 0;

  for (int y = 0; y < rows; y ++)
  {
    for (int x = 0; x < cols; x ++)
      *(mtrx + (y * cols) + x) = rand() % 1000;
      /* nums += scanf("%i", mtrx + (y * cols) + x); */
  }

  /* return nums; */
  return 1;
}

