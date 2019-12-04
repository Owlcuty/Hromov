struct Stud
{
   char *first_name, *last_name;
   int year, month, day;
   double av_grade;
   struct stud *next;
};

Stud* read_data(FILE* fdata)
{
  long int file_len = 0;

  // if (fseek..)
    fseek(fdata, 0, SEEK_END);

  // if (ftell..)
    file_len = ftell(fdata);

  // if (fseek..)
    fseek(fdata, 0, SEEK_SET);

  char* data = (char*)(calloc(file_len, sizeof(*data)));

  size_t num_stud = 0;

  sscanf(data, "%zu", &num_stud);

  for (size_t stud = 0; stud < num_stud; stud ++)
  {

  }
}
