#include "util.h"

char *
lms_strdup
(const char *str)
{
  if (NULL == str)
    return strdup("");

  size_t len = strlen(str);
  char *result = calloc(sizeof(char), len+1);
  if (NULL == result)
    fprintf(stderr, "lms: fatal: calloc failed\n"), exit(255);

  memcpy(result, str, len);

  return result;
}

char *
lms_readfile
(const char *path)
{
  FILE *fp = fopen(path, "r");
  if (NULL == fp)
    return NULL;

  //read entire file into string
  fseek(fp, 0, SEEK_END);
  const size_t len = ftell(fp);
  fseek(fp, 0, SEEK_SET);

  //check for overflow
  if (SIZE_MAX == len)
    return NULL;

  char *result = calloc(sizeof(char), len+1);
  if (NULL == result)
    return NULL;

  const size_t nb = fread(result, sizeof(char), len, fp);
  fclose(fp);

  if (nb != len)
  {
    free(result);
    return NULL;
  }

  return result;
}
