#include "lms.h"
#include "modules.h"

static const char *_lms_module_path = LMS_MODULE_PATH;

int 
lms_list_available
(void)
{
  size_t    num_modules = 0;
  module_t  *modules    = modules_read(_lms_module_path, &num_modules);

  if (0 == num_modules)
  {
    printf("No modules available. Check configuration\n");
    printf("Modules path is: %s\n", _lms_module_path);

    return 1;
  }

  for (size_t i=0; i<num_modules; i++)
    printf(
      "%s/%s/%s\n", 
      modules[i].category,
      modules[i].name,
      modules[i].version
    );

  //no point in freeing modules, as we will exit
  //modules_free(modules);
  return 0;
}

int 
lms_list_loaded
(void)
{

  return 0;
}

int 
lms_list_which
(size_t num_names, char **names)
{
  size_t    num_modules = 0;
  module_t  *modules    = modules_read(_lms_module_path, &num_modules);

  if (0 == num_modules)
  {
    printf("No modules available. Check configuration\n");
    printf("Modules path is: %s\n", _lms_module_path);

    return 1;
  }

  for (size_t i=0; i<num_names; i++)
  {
    size_t num_found = 0;
    module_t *list = 
      modules_find_provides(names[i], modules, num_modules, &num_found);

    if (0 == num_found)
    {
      printf("\"%s\" is not provided by any module available.\n", names[i]);
      continue;
    }

    if (1 == num_found)
    {
      printf("\"%s\" is provided by module %s/%s/%s\n", 
        names[i],
        list[0].category,
        list[0].name,
        list[0].version
      );

      free(list);
      continue;
    }

    printf("\"%s\" is provided by modules:\n", names[i]);
    for (size_t j=0; j<num_found; j++)
    {
      printf("  %s/%s/%s\n", 
        list[j].category,
        list[j].name,
        list[j].version
      );
    }
    free(list);
  }

  return 0;
}

int 
lms_module_load
(size_t num_modules, char **modules)
{

  return 0;
}

int
lms_module_unload
(size_t num_modules, char **modules)
{

  return 0;
}
