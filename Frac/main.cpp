#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>


enum FracErrors
{
  FileErr,
  FileSeekErr,
  FileTellErr,
  CallocErr,
  InvalidInputData,
  NoDataInStack,
  StackOverflow,
  DivByZero,
  SqrtNegative
};

enum FracFlags
{
  Seq,
  Num,
  Base
};

const char* FracStrErrors[] = {
  "%i::%s:: Error file detecting/creating" "\n",
  "%i::%s:: Error fseek FILE{'%s'}" "\n",
  "%i::%s:: Error ftell FILE{'%s'}" "\n",
  "%i::%s:: Error calloc" "\n",
  "%i::%s:: Expected {int, ..., long double}" "\n",
  "%i::%s:: Expected data in 'heap'(Stack_t cpu)" "\n",
  "%i::%s:: Can't add value. StackOverflow" "\n",
  "%i::%s:: Division by zero" "\n"
};

const char* BaseNameFiles = "base";
const int MaxLenBuf = 102400;

int retErr(int FileErr);
int parceArgv(int argc, char* argv[], int* flag, int* arg, char* base_str);
int cutOnFiles(FILE* orig, int flag, int arg, char* base_str);

int min(int a, int b);

int main(int argc, char* argv[])
{
  FILE * origin;
  if (!(origin = fopen("App", "r")))
  {
    printf("%i:: Oops" "\n", __LINE__);
    return retErr(FileErr);
  }

  int flag = 0;
  int arg = 0;
  char base_str[MaxLenBuf];

  if (parceArgv(argc, argv, &flag, &arg, base_str))
  {
    return -1;
  }

  printf("%i:: FLAGS {[%d, %d], [Base = '%s']" "\n", __LINE__, flag, arg, base_str);

  if (!cutOnFiles(origin, flag, arg, base_str))
  {
    fclose(origin);
    return -1;
  }

  fclose(origin);
  return 0;
}


int min(int a, int b)
{
  return (a < b) ? a : b;
}

int retErr(int err)
{
  printf(FracStrErrors[err], __LINE__, __FILE__);
  return err;
}

int parceArgv(int argc, char* argv[], int* flag, int* arg, char* base_str)
{
  assert(argv);
  assert(flag);
  assert(arg);
  assert(strlen(base_str) < MaxLenBuf);

  *flag = Num;                      // Default flag
  *arg  = 2;                        // Default arg
  strcpy(base_str, BaseNameFiles);  // Default base

  for (int data = 1; data < argc; data ++)
  {
    if (argv[data][0] != '-') continue;
    switch (argv[data][1])
    {
      case 's':
        *flag = Seq;

        assert(data + 1 < argc); // Milder pls

        *arg = atoi(argv[++data]);
        break;
      case 'n':
        *flag = Num;

        assert(data + 1 < argc);

        *arg = atoi(argv[++data]);
        break;
      case 'b':
        assert(data + 1 < argc);

        strcpy(base_str, argv[++data]);
        break;
      default:
        printf("%i:: Sorry, smth wrong" "\n", __LINE__);
    }
  }

  return 0;
}

int cutOnFiles(FILE* orig, int flag, int arg, char* base_str)
{
  assert(orig);
  assert(base_str);

  long int file_len = 0;

  if (fseek(orig, 0, SEEK_END) < 0)
  {
    printf("%i:: Oops" "\n", __LINE__);
    return retErr(FileSeekErr);
  }
  if ((file_len = ftell(orig)) == -1L)
  {
    printf("%i:: Oops" "\n", __LINE__);
    return retErr(FileTellErr);
  }
  if (fseek(orig, 0, SEEK_SET) < 0)
  {
    printf("%i:: Oops" "\n", __LINE__);
    return retErr(FileSeekErr);
  }

  char res[file_len + 1];
  if (fread(res, sizeof(char), (size_t)(file_len), orig) < (size_t)file_len)
  {
    return retErr(FileErr);
  }

  int write_size = arg * (flag == Seq) + (file_len / arg) * (flag == Num) + 1;

  printf("%i:: %i" "\n", __LINE__, write_size);

  printf("%i:: %zu" "\n", __LINE__, file_len);
  for (long int pos = 0, num = 0; pos < file_len && num < arg; pos += write_size, num ++)
  {
    char name_file[MaxLenBuf + 10];
    sprintf(name_file, "%s_%zu", base_str, num);
    FILE * out_file;
    if (!(out_file = fopen(name_file, "w")))
    {
    printf("%i:: Oops" "\n", __LINE__);
      retErr(FileErr);
    }
    fwrite(res + pos, sizeof(char), min(write_size, file_len - write_size * num), out_file);
    // fwrite("\n", sizeof(char), 1, out_file);
    fclose(out_file);
  }

  return 0;
}
