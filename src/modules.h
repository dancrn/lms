#include "base.h"
#include <ftw.h>

module_t *
modules_find_provides
(const char *name, module_t *modules, size_t num_modules, size_t *num_found);

module_t
module_find
(module_t *modules, size_t num_modules, char *ident);

module_t *
modules_read
(const char *path, size_t *num_read);

int
ftw_callback
(const char *path, const struct stat * finfo, int flag);

module_t
module_read
(const char *path, int *ret);

int
module_compare
(const void *a, const void *b);

int 
module_compare_str
(module_t a, char *b);

void
modules_free
(module_t *modules, size_t num_modules);

char *
lms_strdup
(const char *str);

bool
file_is_json
(const char *path);
