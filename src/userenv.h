#include "base.h"

#if defined (__APPLE__)
  #define PLATFORM_PATH "PATH"
  #define PLATFORM_INCL "C_INCLUDE_PATH"
  #define PLATFORM_LIBS "DYLD_LIBRARY_PATH"
  #define PLATFORM_DOCS "MANPATH"
#else
  #define PLATFORM_PATH "PATH"
  #define PLATFORM_INCL "C_INCLUDE_PATH"
  #define PLATFORM_LIBS "LD_LIBRARY_PATH"
  #define PLATFORM_DOCS "MANPATH"
#endif

#define LMS_EXPORTS_VAR "_LMS_EXPORTS"
#define LMS_MODULES_VAR "_LMS_MODULES"

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

char *
ue_get_env_var
(module_t m, size_t i);

void
ue_add_exports
(const char *env_var);
