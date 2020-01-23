#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <math.h>

typedef int errno_t;

typedef struct
{
  short offset;
  short length;
  char next;
} Node_t;

enum LZFlags {
  Archive_f,
  Unarchive_f
};

extern const size_t Window_Size;
extern const long Search_Buf_Size;
extern const size_t Buf_Size;

extern const size_t MaxFileName;

errno_t archive(char* origin, size_t olen, FILE* fout);
errno_t unarchive(FILE* fin, FILE* fout);
errno_t parser_flags(const int argc, char* argv[], int* flag, char* name_fin, char* name_fout);
errno_t run(const int flag, const char* name_fin, const char* name_fout);
