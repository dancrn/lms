#include "base.h"

int 
lms_list_available
(void);

int 
lms_list_loaded
(void);

int 
lms_list_which
(size_t num_modules, char **modules);

int 
lms_module_load
(size_t num_modules, char **modules);

int
lms_module_unload
(size_t num_modules, char **modules);
