#include <stdio.h>

#include "mystr.h"

int main()
{
  char* from = "abasadab";
  char*   to = "baadab";

  printf("my_strlen(from) = %d\n", my_strlen(from));
  printf("my_strlen(to) = %d\n", my_strlen(to));
  printf("my_strstr(from, to) = %d\n", my_strstr(from, to));

  char  ff[80];
  char* hrom = "lol";

  my_strcpy(hrom, ff);

  printf("%s\n", ff);
  // examples with str*
}
