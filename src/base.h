#pragma once
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

#define LMS_MODULE_PATH "/etc/lms-modules"

typedef struct _module_t
{
  char    *name;
  char    *version;
  char    *category;
  char    *prefix;
  
  size_t  num_provides;
  char    **provides;
} module_t;
