#include "lz77.h"

const size_t Window_Size = 2048; //16; // 2048;
const size_t Buf_Size = 512; // 10; // 512;
const long Search_Buf_Size = Window_Size - Buf_Size;

const size_t MaxFileName = 255;

errno_t archive(char* orig, size_t olen, FILE* fout)
{
  assert(orig);
  assert(fout);

  char* search_buf = (char*)calloc(Search_Buf_Size, sizeof(*search_buf));
  Node_t* nodes = (Node_t*)calloc(olen, sizeof(*nodes));

  char* tmp_p = NULL;
  char* overlap = NULL;
  char* tmp_start = NULL;

  size_t TESTLEN = 0;

  for (int now = 0; now < olen; now ++)
  {
    memcpy(search_buf,
           orig     + (int)fmax(0, now - Search_Buf_Size),
           (int)fmin(now, Search_Buf_Size));
    while (tmp_p = (char*)memmem(search_buf, (int)fmin(now, Search_Buf_Size), orig + now, nodes[now].length + 1))
    {
        printf("%i:: --START TESTLEN { %zu }" "\n", __LINE__, TESTLEN);
        overlap = tmp_p;
        for (int lol = 0; lol < (int)fmin(now, Search_Buf_Size); lol++)
        {
          if ((tmp_start = (char*)memmem(search_buf + lol, (int)fmin(now, Search_Buf_Size) - lol, orig + now, nodes[now].length + 1)) != tmp_p)
          {
            if (!tmp_start) break;
            tmp_p = (char*)memmem(search_buf + lol, (int)fmin(now, Search_Buf_Size) - lol, orig + now, nodes[now].length + 1);
          }
        }
        overlap = tmp_p;
      nodes[now].length++;
    }

    if (overlap)
    {
      nodes[now].offset = (int)fmin(now, Search_Buf_Size) - (int)(overlap - search_buf);
    }
    nodes[now].next = orig[now + nodes[now].length];
    if (!nodes[now].next)
    {
      printf("%i::%d ------------------------------------" "\n", __LINE__, now + nodes[now].length);
      printf("%i:: Nodes{ %d; %d; %c}" "\n", __LINE__, nodes[now].offset, nodes[now].length, nodes[now].next);
      printf("%i:: NodesBefore{ %c }" "\n", __LINE__, orig[now - nodes[now].offset]);
    }
    fwrite(&nodes[now], 1, sizeof(Node_t), fout);
  printf("%i:: NODE[%zu]: offset{%zu}, len{%zu}, next{%c}" "\n", __LINE__, now, nodes[now].offset, nodes[now].length, nodes[now].next);
    now += nodes[now].length;
    TESTLEN += nodes[now].length + 1;
    printf("%i:: --END TESTLEN { %zu }" "\n", __LINE__, TESTLEN);
  }

  free(search_buf);
  free(nodes);
  return 0;
}


errno_t unarchive(FILE* fin, FILE* fout)
{
  long int fin_len = 0;
  fseek(fin, 0, SEEK_END);
  fin_len = ftell(fin);
  fseek(fin, 0, SEEK_SET);

  size_t node_len = fin_len / sizeof(Node_t);

  printf("%i:: (node_len) %zu" "\n", __LINE__, node_len);

  Node_t *nodes;

  nodes = (Node_t*)calloc(node_len + 1, sizeof(Node_t));
  if (!nodes)
  {
    errno = ENOMEM;
    perror("nodes");
    return errno;
  }

  fread(nodes, sizeof(Node_t),  node_len + 1, fin);

  size_t orig_len = 0;

  for (size_t now = 0; now < node_len; now ++)
  {
    orig_len += nodes[now].length + !(!nodes[now].next);
  }

  printf("%i:: (orig_len) %zu" "\n", __LINE__, orig_len);


  char* orig = (char*)calloc(orig_len + 1, sizeof(*orig));
  if (!orig)
  {
    errno = ENOMEM;
    perror("orig");
    return errno;
  }

  for (size_t now = 0, chr = 0; now < node_len && chr < orig_len; now ++)
  {
    printf("%i:: start_chr = %d" "\n", __LINE__, chr);
    if (!nodes[now].offset)
    {
      orig[chr++] = nodes[now].next;
      continue;
    }
    if (nodes[now].length > nodes[now].offset)
    {
      for (int time = 0; time < nodes[now].length / nodes[now].offset; time ++)
      {
        memcpy(orig + chr, orig + chr - nodes[now].offset,
            nodes[now].offset);
        chr += nodes[now].offset;
        printf("%i:: len > nodes -- CHR = %d" "\n", __LINE__, chr);
      }
      continue;
    }
    memcpy(orig + chr, orig + chr - nodes[now].offset,
        nodes[now].length);
    chr += nodes[now].length;
    orig[chr++] = nodes[now].next;
    printf("%i:: chr = %d" "\n", __LINE__, chr);
  }

  fwrite(orig, sizeof(char), orig_len, fout);

  free(nodes);
  free(orig);

  return 0;
}

errno_t parser_flags(const int argc, char* argv[], int* flag, char* name_fin, char* name_fout)
{
  assert(argv);
  assert(flag);
  assert(name_fin);
  assert(name_fout);

  if (argc == 1)
  {
    errno = EINVAL;
    perror("No args");
    return errno;
  }

  for (int arg = 1; arg < argc; arg ++)
  {
    if (!strcmp(argv[arg], "-a") || !strcmp(argv[arg], "--archive"))
    {
      *flag = Archive_f;
    }
    else if (!strcmp(argv[arg], "-u") || !strcmp(argv[arg], "--unarchive"))
    {
      *flag = Unarchive_f;
    }
    else if (!strcmp(argv[arg], "-s") || !strcmp(argv[arg], "--src"))
    {
      if (arg == argc - 1)
      {
        errno = EINVAL;
        perror("No argument for -s // --src");
        return errno;
      }
      if (strlen(argv[arg + 1]) > MaxFileName)
      {
        errno = ENAMETOOLONG;
        perror("File '-s//--src'");
        return errno;
      }
      sprintf(name_fin, "%s", argv[++arg]);
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
      sprintf(name_fout, "%s", argv[++arg]);
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

errno_t run(const int flag, const char* name_fin, const char* name_fout)
{
  printf("%i:: SIZEOF(%d)" "\n", __LINE__, sizeof(Node_t));
  FILE* fin = fopen(name_fin, "r");
  if (!fin)
  {
    errno = ENOENT;
    perror("fin(read 'name_fin')");
    return errno;
  }

  long int olen = 0;

  fseek(fin, 0, SEEK_END);
  olen = ftell(fin);
  fseek(fin, 0, SEEK_SET);

  char* origin = (char*)calloc(olen + 1, 1);

  FILE* fout = fopen(name_fout, "w");

  fread(origin, 1, olen, fin);

  printf("%i:: OLEN {%zu}" "\n", __LINE__, olen);

  switch(flag)
  {
    case Archive_f:
      archive(origin, olen, fout);
      break;
    case Unarchive_f:
      unarchive(fin, fout);
      break;
    default:
      errno = EINVAL;
      perror("flag");
      return errno;
  }

  fclose(fin);
  fclose(fout);
  free(origin);

  return 0;
}
