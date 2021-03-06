const size_t NumOfArray = 5000;
errno_t (*sortings[4])(double *arr, int arr_size, int *n_cmp, int *n_move) = {&sort_bubble, &sort_select, &sort_insert, &sort_qsort};

errno_t iswrong(double *arr, int *n_cmp, int *n_move)
{
  if (!arr)
  {
    errno = EINVAL;
    perror("arr");
    return errno;
  }
  if (!n_cmp)
  {
    errno = EINVAL;
    perror("n_cmp");
    return errno;
  }
  if (!n_move)
  {
    errno = EINVAL;
    perror("n_move");
    return errno;
  }

  return 0;
}

int cmp_reverse(const void* x1, const void* x2)
{
  return ( *(double*)x2 - *(double*)x1 );
}

errno_t generation(FILE* fout)
{
  double* data = (double*)calloc(NumOfArray, sizeof(*data));
  if (!data)
  {
    errno = ENOMEM;
    perror("data");
    return errno;
  }

  for (size_t el = 0; el < NumOfArray; el ++)
  {
    data[el] = rand();
  }

  fwrite(data, sizeof(*data), NumOfArray, fout);
  qsort(data, NumOfArray, sizeof(*data));
  fwrite(data, sizeof(*data), NumOfArray, fout);
  qsort(data, NumOfArray, sizeof(*data), cmp_reverse);
  fwrite(data, sizeof(*data), NumOfArray, fout);

  return 0;
}

errno_t sort_bubble(double *arr, int arr_size, int *n_cmp, int *n_move);
{
  if (iswrong(arr, n_cmp, n_move))
  {
    return errno;
  }

  int tmp = 0;
  for (int i = 0; i < arr_size; i ++)
  {
    for (int j = i + 1; j < arr_size; j ++)
    {
      (*n_cmp)++;
      if (arr[j] < arr[i])
      {
        t = arr[j];
        arr[j] = arr[i];
        arr[i] = t;
        (*n_move)++;
      }
    }
  }

  if (iswrong(arr, n_cmp, n_move))
  {
    return errno;
  }

  return 0;
}

errno_t sort_select(double *arr, int arr_size, int *n_cmp, int *n_move)
{
  if (iswrong(arr, n_cmp, n_move))
  {
    return errno;
  }

  double mn = arr[0];
  int ind_mn = 0;

  for (int i = 0; i < arr_size; i ++)
  {
    for (int j = i + 1; j < arr_size; j ++)
    {
      (*n_cmp)++;
      if (arr[j] < mn)
      {
        mn = arr[j];
        ind_mn = j;
      }
    }
    arr[ind_mn] = arr[i];
    arr[i] = mn;
    (*n_move)++;
  }

  if (iswrong(arr, n_cmp, n_move))
  {
    return errno;
  }
}

errno_t sort_insert(double *arr, int arr_size, int *n_cmp, int *n_move)
{
  if (iswrong(arr, n_cmp, n_move))
  {
    return errno;
  }

  double key = 0;
  int i = 0;

  for (int j = 1; j < arr_size; j ++)
  {
    key = arr[j];
    i = j - 1;
    if (!(i >= 0 && arr[i] > key))
    {
      (*n_cmp)++;
    }
    while (i >= 0 && arr[i] > key)
    {
      arr[i + 1] = arr[i];
      i--;
      arr[i + 1] = key;
      (*n_cmp)++;
      (*n_move)++;
    }
  }

  if (iswrong(arr, n_cmp, n_move))
  {
    return errno;
  }
}

errno_t sort_qsort(double *arr, int arr_size, int *n_cmp, int *n_move)
{
  if (iswrong(arr, n_cmp, n_move))
  {
    return errno;
  }


  if (iswrong(arr, n_cmp, n_move))
  {
    return errno;
  }
}

errno_t  my_qsort(double *arr, int first, int last, int *n_cmp, int *n_move)
{
  if (iswrong(arr, n_cmp, n_move))
  {
    return errno;
  }

  int l = first;
  int r = last;
  double mida = arr[(l + r) / 2];

  do {
    if (!(arr[l] < mida)) (*n_cmp)++;
    while (arr[l] < mida)
    {
      l++;
      (*n_cmp)++;
    }
    if (!(arr[r] > mida)) (*n_cmp)++;
    while (arr[r] > mida)
    {
      r--;
      (*n_cmp)++;
    }

    if (l <= r)
    {
      if (arr[l] > arr[r])
      {
        double tmp = arr[r];
        arr[r] = arr[l];
        arr[l] = tmp;
        (*n_move)++;
      }
      l++;
      r--;
    }
    (*n_cmp) += 3;
  } while (l <= r);

  (*n_cmp) += 2;

  if (l < last)
  {
    my_qsort(arr, l, last);
  }
  if (first < r)
  {
    my_qsort(arr, first, r);
  }

  if (iswrong(arr, n_cmp, n_move))
  {
    return errno;
  }
  return 0;
}

errno_t write_csv(double* arr, int arr_size, FILE* fcsv)
{
  for (int el = 0; el < arr_size; el ++)
  {
    fprintf("%f;", arr[el]);
  }

  return 0;
}

errno_t sort(double* arr, int arr_size, FILE* fcsv)
{
  int* n_cmps = (int*)calloc(NumOfArray, sizeof(*n_cmps));
  if (!n_cmps)
  {
    errno = ENOMEM;
    perror("n_cmps");
    return errno;
  }
  int* n_moves = (int*)calloc(NumOfArray, sizeof(*n_moves));
  if (!n_moves)
  {
    errno = ENOMEM;
    perror("n_moves");
    return errno;
  }

  for (int func = 0; func < 4; func ++)
  {
    for (int el = 0; el < arr_size; el ++)
    {
      (sortings[func])(arr, el, &n_cmps[el], &n_moves[el]);
    }
    for (int el = 0; el < arr_size; el ++)
    {
      fprintf(fcsv, "%f;", n_cmps[el]);
    }
    fprintf(fcsv, "\n");
    for (int el = 0; el < arr_size; el ++)
    {
      fprintf(fcsv, "%f;", n_moves[el]);
    }
    fprintf(fcsv, "\n");
  }

  free(n_cmps);
  free(n_moves);
  return 0;
}

errno_t run(FILE* fdata, FILE* fcsv)
{
  double* rnd = (double*)calloc(NumOfArray, sizeof(*rnd));
  if (!rnd)
  {
    errno = ENOMEM;
    perror("rnd");
    return errno;
  }

  fread(rnd, sizeof(*rnd), NumOfArray, fdata);

  double* ordr = (double*)calloc(NumOfArray, sizeof(*ordr));
  if (!ordr)
  {
    errno = ENOMEM;
    perror("ordr");
    return errno;
  }

  fread(ordr, sizeof(*ordr), NumOfArray, fdata);

  double* rvrs = (double*)calloc(NumOfArray, sizeof(*rvrs));
  if (!rvrs)
  {
    errno = ENOMEM;
    perror("rvrs");
    return errno;
  }

  fread(rvrs, sizeof(*rvrs), NumOfArray, fdata);

  sort(ordr, NumOfArray, fcsv);
  sort(rvrs, NumOfArray, fcsv);
  sort(rnd, NumOfArray, fcsv);

  free(rnd);
  free(ordr);
  free(rvrs);
}
