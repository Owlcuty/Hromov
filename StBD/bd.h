#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

typedef int errno_t;

typedef struct
{
   char *first_name, *last_name;
   int year, month, day;
   double av_grade;
   void *next;
   int id;
} Stud_t;

enum TypeEnter
{
  FileEnter_t,
  StdinEnter_t
};

enum Action
{
  ReadCSV,
  WriteCSV,
  DropBD,
  AddData,
  DeleteStudent,
  FindStudent,
  ExitAct
};

extern const int MAX_NAME;

Stud_t* read_data(FILE* fdata, int* num_st, int start_id);
errno_t write_csv(FILE* fout, Stud_t* studs, int num_st);
errno_t dump_bd(Stud_t* studs);
void dump_student(Stud_t* stud);
errno_t addafter_stud(Stud_t* stud, int* num_st, FILE* fdata, int type);
errno_t deleteAfter(Stud_t* before, int* num_st);
errno_t find_stud_byLN(Stud_t* studs_bd, Stud_t** student, const char* last_name);
errno_t run();
