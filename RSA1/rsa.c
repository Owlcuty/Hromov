#include "rsa.h"

const int MaxFileName = 255;

/*!
 Находит d для такого k, что fi * k + 1 % e == 0
 \param[in] f_e Функция Эйлера от n = p1 + p2
 \param[in] e   Открытая экспонента
 \return Код ошибки
 */
errno_t find_d(mpz_t* d, const mpz_t f_e, const mpz_t e)
{
  assert(d);

  mpz_t k;
  mpz_init_set_ui(k, 0);

  mpz_t tmp;
  mpz_init(tmp);

  mpz_t tmp2;
  mpz_init(tmp2);

  for (; mpz_cmp(k, f_e) < 0; mpz_add_ui(k, k, 1))
  {
    mpz_mul(tmp, k, f_e);
    mpz_add_ui(tmp, tmp, 1);
    mpz_set(tmp2, tmp);
    mpz_div(tmp2, tmp2, e);
    mpz_mod(tmp, tmp, e);
    if (!mpz_cmp_ui(tmp, 0))
    {
      mpz_set(*d, tmp2);
      mpz_clear(k);
      mpz_clear(tmp);
      mpz_clear(tmp2);
      return 0;
    }
  }

  mpz_clear(k);
  mpz_clear(tmp);
  mpz_clear(tmp2);
  return -1;
}

void func_eul(mpz_t* f_e, mpz_t p1, mpz_t p2)
{
  assert(f_e);

  mpz_sub_ui(p1, p1, 1);
  mpz_sub_ui(p2, p2, 1);

  mpz_mul(*f_e, p1, p2);
}

void init_n(mpz_t* n, const mpz_t p1, const mpz_t p2)
{
  assert(n);

  mpz_init(*n);
  mpz_mul(*n, p1, p2);
}

void init_OKey(OKey_t* open_key)
{
  assert(open_key);

  mpz_init(open_key->e);
  mpz_init(open_key->n);
}

void destr_OKey(OKey_t* open_key)
{
  assert(open_key);

  mpz_clear(open_key->e);
  mpz_clear(open_key->n);
}

void init_CKey(CKey_t* close_key)
{
  assert(close_key);

  mpz_init(close_key->d);
  mpz_init(close_key->n);
}

void destr_CKey(CKey_t* close_key)
{
  assert(close_key);

  mpz_clear(close_key->d);
  mpz_clear(close_key->n);
}



errno_t gen_keys(OKey_t* open_key, CKey_t* close_key, FILE* fdata)
{
  assert(open_key);
  assert(close_key);
  assert(fdata);

  FILE* foutOK;

  if (!(foutOK = fopen("Open_key", "w")))
  {
    errno = ENOENT;
    perror("foutOK(write)");
    return ENOENT;
  }

  FILE* foutCK;

  if (!(foutCK = fopen("Close_key", "w")))
  {
    errno = ENOENT;
    perror("foutCK(write)");
    return ENOENT;
  }

  init_OKey(open_key);
  init_CKey(close_key);

  char tmp[255];
  fscanf(fdata, "%s", tmp);
  /** mpz_set_str(open_key->e, "7597111", 10); */
  mpz_set_str(open_key->e, tmp, 10);

  mpz_t p1, p2;
  mpz_init_set_str(p1, "10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 10);
  mpz_nextprime(p1, p1);
  mpz_init_set_str(p2, "1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000", 10);
  mpz_nextprime(p2, p2);
  /** mpz_init_set_str(p1, "2143", 10); //ADD FROM FILE */
  /** fscanf(fdata, "%s", tmp); */
  /** mpz_init_set_str(p1, tmp, 10); //ADD FROM FILE */
  /** mpz_init_set_str(p2, "4231", 10); //ADD FROM FILE */
  /** fscanf(fdata, "%s", tmp); */
  /** mpz_init_set_str(p2, tmp, 10); //ADD FROM FILE */

  mpz_mul(open_key->n, p1, p2);
  mpz_mul(close_key->n, p1, p2);

  mpz_t f_e;
  mpz_init(f_e);

  func_eul(&f_e, p1, p2);

  if (find_d(&(close_key->d), f_e, open_key->e))
  {
    return -1;
  }

  gmp_fprintf(foutOK, "%Zd,%Zd", open_key->e, open_key->n);
  gmp_fprintf(foutCK, "%Zd,%Zd", close_key->d, close_key->n);

  destr_OKey(open_key);
  destr_CKey(close_key);
  mpz_clear(p1);
  mpz_clear(p2);
  mpz_clear(f_e);

  fclose(foutOK);
  fclose(foutCK);

  return 0;
}

errno_t read_open_key(OKey_t* open_key, FILE* fokey)
{
  assert(open_key);
  assert(fokey);

  if (gmp_fscanf(fokey, "%Zd,%Zd", open_key->e, open_key->n) != 2)
  {
    errno = EINVAL;
    perror("open_key");
    return errno;
  }

  return 0;
}

errno_t read_close_key(CKey_t* close_key, FILE* fckey)
{
  assert(close_key);
  assert(fckey);

  if (gmp_fscanf(fckey, "%Zd,%Zd", close_key->d, close_key->n) != 2)
  {
    errno = EINVAL;
    perror("close_key");
    return errno;
  }

  return 0;
}

errno_t encrypt(char* origin, const OKey_t open_key, FILE* fout)
{
  assert(origin);
  assert(fout);

  mpz_t encrypted;
  mpz_init(encrypted);

  mpz_t m; // symbol
  mpz_init(m);

  for (size_t chr = 0; chr < strlen(origin); chr ++)
  {
    mpz_set_ui(m, origin[chr]);
    mpz_powm(encrypted, m, open_key.e, open_key.n);
    gmp_fprintf(fout, "%Zd ", encrypted);
  }

  mpz_clear(encrypted);
  mpz_clear(m);
  return 0;
}

size_t decrypt(FILE* fin, const CKey_t close_key, char* to_orig, const size_t to_len)
{
  assert(fin);
  assert(to_orig);

  size_t now = 0;
  mpz_t tmp;
  mpz_init(tmp);

  while (!feof(fin) && now < to_len)
  {
    if (!gmp_fscanf(fin, "%Zd", tmp))
      break;
    mpz_powm(tmp, tmp, close_key.d, close_key.n);
    to_orig[now] = mpz_get_ui(tmp);
    now ++;
  }

  mpz_clear(tmp);
  return now;
}

errno_t parser_flags(const int argc, char* argv[], int* flag, char* ffrom_name, char* fto_name)
{
  assert(argv);
  assert(flag);
  assert(ffrom_name);
  assert(fto_name);

  if (argc == 1)
  {
    errno = EINVAL;
    perror("No args");
    return errno;
  }

  for (int arg = 1; arg < argc; arg ++)
  {
    if (!strcmp(argv[arg], "-g") || !strcmp(argv[arg], "--gen"))
    {
      *flag = Generation_f;
    }
    else if (!strcmp(argv[arg], "-e") || !strcmp(argv[arg], "--encrypt"))
    {
      *flag = Encryption_f;
    }
    else if (!strcmp(argv[arg], "-d") || !strcmp(argv[arg], "--decrypt"))
    {
      *flag = Decryption_f;
    }
    else if (!strcmp(argv[arg], "-f") || !strcmp(argv[arg], "--from"))
    {
      if (arg == argc - 1)
      {
        errno = EINVAL;
        perror("No argument for -f // --from");
        return errno;
      }
      if (strlen(argv[arg + 1]) > MaxFileName)
      {
        errno = ENAMETOOLONG;
        perror("File '-f//--from'");
        return errno;
      }
      sprintf(ffrom_name, "%s", argv[++arg]);
    }
    else if (!strcmp(argv[arg], "-t") || !strcmp(argv[arg], "--to"))
    {
      if (arg == argc - 1)
      {
        errno = EINVAL;
        perror("No argument for -t // --to");
        return errno;
      }
      if (strlen(argv[arg + 1]) > MaxFileName)
      {
        errno = ENAMETOOLONG;
        perror("File '-t//--to'");
        return errno;
      }
      sprintf(fto_name, "%s", argv[++arg]);
    }
    else
    {
      printf("%i:: argvs{%s}" "\n", __LINE__, argv[arg]);
      errno = EINVAL;
      perror("argvs(flags)");
      return errno;
    }
  }

  return 0;
}

errno_t run(const int flag, FILE* ffrom, FILE* fto)
{
  assert(ffrom);
  assert(flag == Generation_f || fto);

  OKey_t open_key  = {};
  CKey_t close_key = {};
  FILE* fokey = NULL;
  FILE* fckey = NULL;
  size_t ffrom_len = 0;

  char* orig_from = NULL;
  size_t orig_len;

  fseek(ffrom, 0, SEEK_END);
  ffrom_len = ftell(ffrom);
  fseek(ffrom, 0, SEEK_SET);

  switch (flag)
  {
    case Generation_f:
      gen_keys(&open_key, &close_key, ffrom);
      break;
    case Encryption_f:
      fokey = fopen("Open_key", "r");
      if (!fokey)
      {
        errno = ENOENT;
        perror("fokey(read 'Open_key')");
        return errno;
      }
      read_open_key(&open_key, fokey);

      orig_from = (char*)calloc(ffrom_len + 1, sizeof(*orig_from));
      if (!orig_from)
      {
        errno = ENOMEM;
        perror("orig_from");
        return errno;
      }
      fread(orig_from, ffrom_len, sizeof(char), ffrom);
      encrypt(orig_from, open_key, fto);

      free(orig_from);

      break;
    case Decryption_f:
      fckey = fopen("Close_key", "r");
      if (!fckey)
      {
        errno = ENOENT;
        perror("fckey(read 'Close_key')");
        return errno;
      }
      read_close_key(&close_key, fckey);

      orig_from = (char*)calloc(ffrom_len, sizeof(*orig_from));
      if (!orig_from)
      {
        errno = ENOMEM;
        perror("orig_from");
        return errno;
      }
      orig_len = decrypt(ffrom, close_key, orig_from, ffrom_len);
      fwrite(orig_from, orig_len - 1, sizeof(char), fto);

      free(orig_from);

      break;
    default:
      return flag;
  }

  return 0;
}
