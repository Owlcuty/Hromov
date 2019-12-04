#include "rsa.c"

int main(int argc, char* argv[])
{
  int flag = 0;
  char* ffrom_name = (char*)calloc(MaxFileName, 1);
  char* fto_name = (char*)calloc(MaxFileName, 1);

  if (parser_flags(argc, argv, &flag, ffrom_name, fto_name))
  {
    printf("%i:: Ooops" "\n", __LINE__);
    return errno;
  }

  FILE* ffrom = fopen((const char*)ffrom_name, "r");
  if (!ffrom)
  {
    errno = ENOENT;
    perror("ffrom(read)");
    return errno;
  }

  FILE* fto = fopen((const char*)fto_name, "w");
  if (!fto && flag != Generation_f)
  {
    errno = ENOENT;
    perror("fto(read '')");
    return errno;
  }


  if (run(flag, ffrom, fto))
  {
    printf("%i:: Oooops" "\n", __LINE__);
  }

 return 0;
}
