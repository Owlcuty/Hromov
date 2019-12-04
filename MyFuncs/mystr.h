#include <stdio.h>
#include <assert.h>

const int MAX_LEN_BUF = 256;

int my_strlen(char *str);
void my_strcpy(char *source, char *target);
int my_strstr(char *haystack, char *needle);

int my_strlen(char *str)
{
  assert(str);

  char cnt = 0;
  for (char *chr = str; (*chr) != '\0' && cnt < MAX_LEN_BUF; chr ++, cnt ++) {}

  if (cnt < MAX_LEN_BUF)
    return (int)cnt;

  return NULL;
}

void my_strcpy(char *source, char *target)
{
  assert(source);
  assert(target);

  for (int it = 0;
        source[it] != '\0' && it < MAX_LEN_BUF;
        it ++)
  {
    target[it] = source[it];
  }
}

int my_strstr(char *haystack, char *needle)
{
  assert(haystack);
  assert(needle);

  for (int ch = 0, cnt = 0; haystack[ch] != '\0' && cnt < MAX_LEN_BUF; ch ++, cnt ++)
  {
    int des = 0, wh  = ch; // desired, where
    for (; needle[des] != '\0' && needle[des] == haystack[wh]; des++, wh++);
    if (des > 0 && needle[des++] == '\0')
      return des;
  }

  return -1;
}
