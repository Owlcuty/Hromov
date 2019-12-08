#include "lz77.h"

int main(int argc, char* argv[])
{
  int flag = 0;
  char* name_fin = (char*)calloc(MaxFileName + 1, sizeof(*name_fin));
  if (!name_fin)
  {
    errno = ENOMEM;
    perror("name_fin"); return errno;
  }

  char* name_fout = (char*)calloc(MaxFileName + 1, sizeof(*name_fout));
  if (!name_fout)
  {
    errno = ENOMEM;
    perror("name_fout");
    return errno;
  }

  parser_flags(argc, argv, &flag, name_fin, name_fout);

  run(flag, name_fin, name_fout);

  free(name_fin);
  free(name_fout);

  return 0;
}
