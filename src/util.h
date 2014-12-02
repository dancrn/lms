#include "base.h"

//duplicate a string (returns "" on NULL)
char *
lms_strdup
(const char *str);

//copy entire file into string
char *
lms_readfile
(const char *path);
