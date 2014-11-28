#include "modules.h"
#include "json.h"
#include "util.h"

//i don't like globals, but ftw offers no other choice..
size_t    _ftw_num_read = 0;
module_t  *_ftw_module  = NULL;

module_t *
modules_read
(const char *path, size_t *num_read)
{
  if (NULL == num_read)
    return NULL;

  *num_read = 0;

  if (NULL == path)
    return NULL;

  int ret = ftw(path, &ftw_callback, 1);
  if (ret)
    return NULL;

  *num_read        = _ftw_num_read;
  module_t *result = _ftw_module;

  _ftw_num_read    = 0;
  _ftw_module      = NULL;
  
  return result;
}

int
ftw_callback
(const char *path, const struct stat * finfo, int flag)
{
  //only want to open json files
  if (FTW_F != flag)
    return 0;

  if (!file_is_json(path))
    return 0;

  //read module data
  int ret = 0;
  module_t module = module_read(path, &ret);
  if (ret)
  {
    fprintf(stderr, "lms: warning: %s bad module (%d)\n", path, ret);
    return 0;
  }

  //grow the module list
  size_t len        = _ftw_num_read;
  _ftw_module       = realloc(_ftw_module, sizeof(module_t)*(len+1));
  _ftw_module[len]  = module;
  _ftw_num_read     += 1;

  return 0;
}

module_t
module_read
(const char *path, int *ret)
{
  module_t result = {0};

  if (NULL == ret)
    return result;

  if (NULL == path)
    return *ret = 1, result;

  char *str_mod = lms_readfile(path);
  if (NULL == str_mod)
    return *ret = 2, result;

  size_t len = strlen(str_mod);

  //parse into json
  json_value *jsn_mod = json_parse(str_mod, len);
  free(str_mod);

  if (NULL == jsn_mod)
    return *ret = 6, result;

  json_value *jsn_name      = json_find(jsn_mod, "name",     json_string);
  json_value *jsn_version   = json_find(jsn_mod, "version",  json_string);
  json_value *jsn_category  = json_find(jsn_mod, "category", json_string);
  json_value *jsn_prefix    = json_find(jsn_mod, "prefix",   json_string);
  json_value *jsn_provides  = json_find(jsn_mod, "provides", json_array);

  //ensure our values have correct type
  if (
    NULL == jsn_name     ||
    NULL == jsn_version  ||
    NULL == jsn_category ||
    NULL == jsn_prefix   ||
    NULL == jsn_provides
  )
  {
    *ret = 7;
    json_value_free(jsn_mod);
  }

  //read module data
  result.num_provides = jsn_provides->u.array.length;
  result.provides = calloc(sizeof(char *), result.num_provides);

  for (size_t i=0; i<result.num_provides; i++)
  {
    json_value *val = jsn_provides->u.array.values[i];
    if (json_string != val->type)
    {
      for (size_t j=i; j>0; j--)
        free(result.provides[j-1]);

      free(result.provides);
      json_value_free(jsn_mod);
      *ret = 8;
      return result = (module_t) {0};
    }

    result.provides[i] = lms_strdup(val->u.string.ptr);
  }

  result.name     = lms_strdup(jsn_name->u.string.ptr);
  result.version  = lms_strdup(jsn_version->u.string.ptr);
  result.category = lms_strdup(jsn_category->u.string.ptr);
  result.prefix   = lms_strdup(jsn_prefix->u.string.ptr);

  json_value_free(jsn_mod);
  return result;
}

module_t *
modules_find_provides
(const char *name, module_t *modules, size_t num_modules, size_t *num_found)
{
  if (NULL == num_found)
    return NULL;

  *num_found = 0;
  
  if (NULL == name)
    return NULL;

  if (NULL == modules)
    return NULL;
  
  size_t   len     = 0;
  module_t *result = NULL;

  for (size_t i=0; i<num_modules; i++)
  {
    const module_t m = modules[i];
    for (size_t j=0; j<m.num_provides; j++)
    {
      if (strcmp(m.provides[j], name))
        continue;

      result = realloc(result, sizeof(module_t)*(len+1));
      result[len] = m;
      len += 1;
      break;
    }
  }

  *num_found = len;
  return result;
}

int 
module_compare_str
(module_t a, char *b)
{
  size_t slen = strlen(a.category) + 
    strlen(a.name) + 
    strlen(a.version);

  char buf[slen+4]; memset(buf, 0, slen+4);
  
  snprintf(buf, slen+3, "%s/%s/%s", a.category, a.name, a.version);

  return strcmp(buf, b);
}

module_t
module_find
(module_t *modules, size_t num_modules, char *ident)
{
  for (size_t i=0; i<num_modules; i++)
    if (!module_compare_str(modules[i], ident))
      return modules[i];

  fprintf(stderr, 
    "lms-fatal: could not find module with identifier \"%s\"\n", 
    ident
  );
  exit(1);

  return (module_t) {0};
}

void
modules_free
(module_t *modules, size_t num_modules)
{
  if (NULL == modules)
    return;

  if (0 == num_modules)
    return;

  for (size_t i=0; i<num_modules; i++)
  {
    for (size_t j=0; j<modules[i].num_provides; j++)
      free(modules[i].provides[j]);
    
    free(modules[i].provides);
    free(modules[i].name);
    free(modules[i].version);
    free(modules[i].category);
    free(modules[i].prefix);
  }

  free(modules);
}

bool
file_is_json
(const char *path)
{
  //check if file ends in .json
  if (NULL == path)
    return false;

  size_t len = strlen(path);

  //file can't just be called ".json"
  if (len < 5)
    return false;

  if (!strcmp(&path[len-5], ".json"))
    return true;

  return false;
}
