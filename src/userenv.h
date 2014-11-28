#include "base.h"

#if defined (__APPLE__)
  #define PLATFORM_PATH "PATH"
  #define PLATFORM_INCL "C_INCLUDE_PATH"
  #define PLATFORM_LIBS "DYLD_LIBRARY_PATH"
#else
  #define PLATFORM_PATH "PATH"
  #define PLATFORM_INCL "C_INCLUDE_PATH"
  #define PLATFORM_LIBS "LD_LIBRARY_PATH"
#endif

char **
ue_get_loaded
(size_t *num_loaded);

void
ue_gen_script
(void);

int 
ue_add_module
(module_t module);

int 
ue_del_module
(module_t m);

void
ue_append_env
(const char *env_var, const char *prefix, const char *postfix);

void
ue_remove_env
(const char *env_cat, const char *prefix);
