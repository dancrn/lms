#include "lms.h"
#include "modules.h"
#include "userenv.h"

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

  modules_free(modules, num_modules);
  return 0;
}

int 
lms_list_loaded
(void)
{
  size_t num_loaded = 0;
  char   **list     = ue_get_loaded(&num_loaded);

  if (0 == num_loaded)
  {
    printf("No modules loaded\n");
    return 0;
  }

  for (size_t i=0; i<num_loaded; i++)
    printf("%s\n", list[i]), free(list[i]);

  free(list);
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

  //no point in freeing modules, as we will exit
  //modules_free(modules);
  return 0;
}

int 
lms_module_load
(size_t num_names, char **names)
{
  size_t num_modules = 0;
  module_t *modules = modules_read(_lms_module_path, &num_modules);

  if (0 == num_modules)
  {
    printf("No modules available. Check configuration\n");
    printf("Modules path is: %s\n", _lms_module_path);

    return 1;
  }

  size_t num_loaded = 0;
  char   **loaded     = ue_get_loaded(&num_loaded);

  for (size_t i=0; i<num_names; i++)
  {
    //find module(s)
    size_t num_found  = 0;
    module_t *list    = 
      lms_fuzzy_search(names[i], num_modules, modules, &num_found);

    if (0 == num_found)
    {
      fprintf(stderr, "No module with name \"%s\"\n", names[i]);
      return 1;
    }

    if (1 < num_found)
    {
      fprintf(stderr, "\"%s\" is ambigious. Matching modules:\n", names[i]);

      for(size_t j=0; j<num_found; j++)
        fprintf(stderr,"%s/%s/%s\n", 
          list[j].category,
          list[j].name,
          list[j].version
        );

      return 1;
    }

    //check for duplicate modules
    bool duplicate = false;
    for (size_t j=0; j<num_loaded; j++)
      if (!module_compare_str(*list, loaded[j]))
        duplicate = true;

    //just do nothing..
    if (duplicate)
      continue;

    //check for duplicate provides
    //..some day

    //load module
    ue_add_module(*list);

    free(list);
  }

  for (size_t i=0; i<num_loaded; i++)
    free(loaded[i]);

  free(loaded);

  ue_gen_script();

  return 0;
}

int
lms_module_unload
(size_t num_modules, char **modules)
{
  //get loaded modules

  //check if module is loaded

  //unload

  return 0;
}

module_t *
lms_fuzzy_search
(const char *name, size_t num_modules, module_t *modules, size_t *num_found)
{
  //split id into tokens, try a few searches, return all possible matches.
  char *tmp = lms_strdup(name);
  char *s1 = strtok(tmp,  "/");
  char *s2 = strtok(NULL, "/");
  char *s3 = strtok(NULL, "/");

  size_t ns = !!s1 + !!s2 + !!s3;

  size_t len        = 0;
  module_t *result  = NULL;

  //forgive the mild abuse of switch (this is hardly duff's device though..)
  for (size_t i=0; i<num_modules; i++)
  {
    module_t m = modules[i];
    switch (ns)
    {
      case 3:
      if (strcmp(s1, m.category) || strcmp(s2, m.name) || strcmp(s3, m.version))
        break;
      
      case 2:
      if (ns == 2)
      if (strcmp(s1, m.name) || strcmp(s2, m.version))
      if (strcmp(s1, m.category) || strcmp(s2, m.name))
        break;

      case 1:
      if (ns == 1)
      if (strcmp(s1, m.name))
        break;

      result = realloc(result, sizeof(module_t)*(len+1));
      result[len] = m;
      len += 1;

      default:
        break;
    }
  }

  free(tmp);

  *num_found = len;
  return result;
}
