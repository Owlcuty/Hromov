#include "bd.h"

const int MAX_NAME = 255;

void freeStud(Stud_t* stud)
{
  free(stud->first_name);
  free(stud->last_name);
}

Stud_t* read_data(FILE* fdata, int* num_st, int start_id)
{
  assert(fdata);
  assert(num_st);

  long int file_len = 0;
  int id = start_id;

  // if (fseek..)
    fseek(fdata, 0, SEEK_END);

  // if (ftell..)
    file_len = ftell(fdata);

  // if (fseek..)
    fseek(fdata, 0, SEEK_SET);

  char* data = (char*)(calloc(file_len + 1, sizeof(*data)));

  fread(data, 1, file_len, fdata);

  size_t num_stud = 0;

  sscanf(data, "%zu;", &num_stud);
  data = strchr(data, ';') + 1;
  printf("%i:: Num_stud = %zu" "\n", __LINE__, num_stud);
  printf("%i:: Data {%s}" "\n", __LINE__, data);

  Stud_t* studs = (Stud_t*)calloc(num_stud * 2, sizeof(*studs));
  if (!studs)
  {
    errno = ENOMEM;
    perror("studs");
    return NULL;
  }

  for (Stud_t* stud = studs; stud <= studs + num_stud; stud ++)
  {
    if (stud - studs)
    {
      (stud - 1)->next = stud;
    }
    else
    {
      stud->id = -1;
      continue;
    }
    stud->id = id++;
    printf("%i:: Student ID: %d" "\n", __LINE__, stud->id);
    stud->last_name = (char*)calloc(MAX_NAME + 1, sizeof(char));
    printf("%i:: Student ID: %d" "\n", __LINE__, stud->id);
    if (!stud->last_name)
    {
      errno = ENOMEM;
      perror("stud->ln");
      return NULL;
    }
    printf("%i:: Student ID: %d" "\n", __LINE__, stud->id);
    stud->first_name = (char*)calloc(MAX_NAME + 1, sizeof(char));
    if (!stud->first_name)
    {
      errno = ENOMEM;
      perror("stud->fn");
      return NULL;
    }
    printf("%i:: Student ID: %d. {%s}" "\n", __LINE__, stud->id, data);
    int cntscan = 0;
    if ((cntscan = sscanf(data, "%[^;];%[^;];%d.%d.%d;%lf;", stud->last_name, stud->first_name, &(stud->year), &(stud->month), &(stud->day), &(stud->av_grade))) != 6)
    {
      printf("%i:: Invalid data; Cnt = %d" "\n", __LINE__, cntscan);
      errno = EINVAL;
      return NULL;
    }

    data = strchr(strchr(strchr(strchr(strchr(strchr(data, ';') + 1, ';'), '.') + 1, '.'), ';') + 1, ';') + 1;

    printf("%i:: Student ID: %d" "\n", __LINE__, stud->id);
    printf("%i:: " "\n", __LINE__);
  }

  (*num_st) += num_stud;
  return (studs + 1);
}

errno_t write_csv(FILE* fout, Stud_t* studs, int num_st)
{
  assert(fout);
  assert(studs);

  fprintf(fout, "%d;", num_st);
  for (Stud_t* stud = studs; stud; stud = (Stud_t*)(stud->next))
  {
    fprintf(fout, "%s;%s;%d.%d.%d;%lf;", stud->last_name,
                                         stud->first_name,
                                         stud->year,
                                         stud->month,
                                         stud->day,
                                         stud->av_grade);
  }

  return 0;
}

errno_t dump_bd(Stud_t* studs)
{
  for (Stud_t* stud = studs; stud; stud = (Stud_t*)(stud->next))
  {
    printf("%i:: Student ID: %d" "\n", __LINE__, stud->id);
    printf("%i:: %s %s" "\n", __LINE__, stud->last_name, stud->first_name);
    printf("%i:: Year of birth: %d.%d.%d" "\n", __LINE__, stud->year,
                                                          stud->month,
                                                          stud->day);
    printf("%i:: GPA: %lf" "\n", __LINE__, stud->av_grade);

    printf("\n");
  }

  return 0;
}

void dump_student(Stud_t* stud)
{
    printf("%i:: Student ID: %d" "\n", __LINE__, stud->id);
    printf("%i:: %s %s" "\n", __LINE__, stud->last_name, stud->first_name);
    printf("%i:: Year of birth: %d.%d.%d" "\n", __LINE__, stud->year,
                                                          stud->month,
                                                          stud->day);
    printf("%i:: GPA: %lf" "\n", __LINE__, stud->av_grade);

    printf("\n");
}

errno_t addafter_stud(Stud_t* stud, int* num_st, FILE* fdata, int type)
{
  assert(stud);
  assert(num_st);
  assert(fdata);

  Stud_t* new_stud = (Stud_t*)calloc(1, sizeof(*new_stud));
  if (!new_stud)
  {
    errno = ENOMEM;
    perror("new_stud");
    return errno;
  }

  new_stud->last_name = (char*)calloc(MAX_NAME + 1, sizeof(*stud->last_name));
  if (!new_stud->last_name)
  {
    errno = ENOMEM;
    perror("stud->last_name");
    return errno;
  }

  new_stud->first_name = (char*)calloc(MAX_NAME + 1, sizeof(*stud->first_name));
  if (!new_stud->first_name)
  {
    errno = ENOMEM;
    perror("stud->first_name");
    return errno;
  }

  switch (type)
  {
    case FileEnter_t:
      new_stud = read_data(fdata, num_st, stud->id + 1);
      if (!new_stud)
      {
        errno = ENOMEM;
        perror("new_stud");
        return errno;
      }
      break;
    case StdinEnter_t:
      printf("%i:: Lastname: ", __LINE__);
      scanf("%s", new_stud->last_name);
      printf("%i:: {%s}" "\n", __LINE__, new_stud->last_name);
      printf("%i:: Firstname: ",  __LINE__);
      scanf("%s", new_stud->first_name);

      printf("%i:: Birth (yyyy.mm.dd): ", __LINE__);
      scanf("%4d.%2d.%2d", &(new_stud->year),
                           &(new_stud->month),
                           &(new_stud->day));

      printf("%i:: GPA: ", __LINE__);
      scanf("%lf", &(new_stud->av_grade));

      new_stud->id = stud->id + 1;

      (*num_st) ++;

      printf("%i:: Added" "\n", __LINE__);
      break;
    default:
      printf("%i:: Smth wrong" "\n", __LINE__);
      return -1;
  }

  stud->next = new_stud;

  return 0;
}

errno_t deleteAfter(Stud_t* before, int* num_st)
{
  assert(before);

  if (!before->next)
  {
    printf("%i:: Out of range" "\n", __LINE__);
    return -1;
  }

  Stud_t* temp = ((Stud_t*)(before->next))->next;
  freeStud(before->next);
  before->next = temp;

  (*num_st) --;

  return 0;
}

errno_t find_stud_byLN(Stud_t* studs_bd, Stud_t** student, const char* last_name)
{
  assert(studs_bd);

  for (Stud_t* stud = studs_bd; stud; stud = (Stud_t*)(stud->next))
  {
    if (!strcmp(stud->last_name, last_name))
    {
      *student = stud;
      return 0;
    }
  }

  printf("%i:: No student with last name {%s}" "\n", __LINE__, last_name);
  return 1;
}

errno_t run()
{
  int menu_mod = 1;
  int flag = -1;
  FILE* file = NULL;
  Stud_t* studs = NULL;
  char filename[255];
  char last_name[255];
  int num_stud = 0;
  Stud_t* last_stud = NULL;
  Stud_t* tmp_stud = NULL;
  Stud_t* to_find = NULL;

  while(1)
  {
    if (menu_mod)
    {
      printf("%i:: 1>Read csv" "\n", __LINE__);
      printf("%i:: 2>Write csv" "\n", __LINE__);
      printf("%i:: 3>Drop BD" "\n", __LINE__);
      printf("%i:: 4>Add data" "\n", __LINE__);
      printf("%i:: 5>Delete student" "\n", __LINE__);
      printf("%i:: 6>Find student" "\n", __LINE__);
      printf("%i:: 7>Exit" "\n", __LINE__);
      menu_mod = 0;
    }
    printf("%i:: Action: ", __LINE__);
    scanf("%i", &flag);
    printf("\n");
    switch (flag - 1)
    {
      case ReadCSV:
        printf("%i:: Filename: ", __LINE__);
        scanf("%s", filename);
        printf("\n");
        file = fopen(filename, "r");
        studs = read_data(file, &num_stud, 0);
        menu_mod = 1;
        fclose(file);
        break;
      case WriteCSV:
        printf("%i:: Filename: ", __LINE__);
        scanf("%s", filename);
        printf("\n");
        file = fopen(filename, "w");
        write_csv(file, studs, num_stud);
        menu_mod = 1;
        fclose(file);
        break;
      case DropBD:
        dump_bd(studs);
        menu_mod = 1;
        break;
      case AddData:
        for (last_stud = studs; last_stud; last_stud = (Stud_t*)(last_stud->next))
          if (!last_stud->next)
            break;
        printf("%i:: 1>File" "\n", __LINE__);
        printf("%i:: 2>Console" "\n", __LINE__);
        scanf("%i", &flag);
        flag--;
        if (flag == FileEnter_t)
        {
          printf("%i:: Filename: ", __LINE__);
          scanf("%s", filename);
          printf("\n");
          file = fopen(filename, "r");
        }
        addafter_stud(last_stud, &num_stud, file, flag);
        if (flag == FileEnter_t)
          fclose(file);
        menu_mod = 1;
        break;
      case DeleteStudent:
        printf("%i:: ID of student: ", __LINE__);
        scanf("%d", &flag);
        tmp_stud = studs - 1;
        for (; ((Stud_t*)(tmp_stud->next))->id != flag; tmp_stud = (Stud_t*)(tmp_stud->next));
        deleteAfter(tmp_stud, &num_stud);
        menu_mod = 1;
        break;
      case FindStudent:
        printf("%i:: Lastname: ", __LINE__);
        scanf("%s", last_name);
        if (!find_stud_byLN(studs, &to_find, last_name))
          dump_student(to_find);
        menu_mod = 1;
        break;
      case ExitAct:
        printf("%i:: Thanks for using us!" "\n", __LINE__);
        return 0;
      default:
        printf("%i:: Smth wrong" "\n", __LINE__);
        return -1;
    }
    printf("\n");
  }

  return 0;
}
