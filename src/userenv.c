#include "userenv.h"
#include "util.h"

#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>

char **
ue_get_loaded
(size_t *num_loaded)
{
  char *env = lms_strdup(getenv("_LMS_MODS"));

  char **result = NULL;
  size_t len    = 0;

  char *tok = strtok(env, ":");
  while (NULL != tok)
  {
    result = realloc(result, sizeof(char*)*(len+1));
    result[len] = lms_strdup(tok);
    len += 1;

    tok = strtok(NULL, ":");
  }

  free(env);

  *num_loaded = len;
  return result;
}

int 
ue_add_module
(module_t m)
{
  //we dump our new env vars at the end into a script
  //which the invoking alias will then source
  size_t slen = strlen(m.category) + 
    strlen(m.name) + 
    strlen(m.version);

  char buf[slen+4]; memset(buf, 0, slen+4);
  
  snprintf(buf, slen+3, "%s/%s/%s", m.category, m.name, m.version);

  ue_append_env(PLATFORM_PATH,  m.prefix, "/bin");
  ue_append_env(PLATFORM_LIBS,  m.prefix, "/lib");
  ue_append_env(PLATFORM_INCL,  m.prefix, "/include");
  ue_append_env("_LMS_MODS",    buf,      "");

  return 0;
}

int 
ue_del_module
(module_t m)
{
  size_t slen = strlen(m.category) + 
    strlen(m.name) + 
    strlen(m.version);

  char buf[slen+4]; memset(buf, 0, slen+4);
  snprintf(buf, slen+3, "%s/%s/%s", m.category, m.name, m.version);

  ue_remove_env(PLATFORM_PATH,  m.prefix);
  ue_remove_env(PLATFORM_LIBS,  m.prefix);
  ue_remove_env(PLATFORM_INCL,  m.prefix);
  ue_remove_env("_LMS_MODS",    buf);

  return 0;
}

void
ue_append_env
(const char *env_var, const char *prefix, const char *postfix)
{
  char *old_env = lms_strdup(getenv(env_var));
  size_t new_len = 
    strlen(old_env) +
    strlen(prefix)  +
    strlen(postfix);

  char buf[new_len+3]; memset(buf, 0, new_len+3);

  if (strlen(old_env))
    snprintf(buf, new_len+2, "%s%s:%s", prefix, postfix, old_env);
  else
    snprintf(buf, new_len+2, "%s%s", prefix, postfix);

  setenv(env_var, buf, 1);
}

void
ue_remove_env
(const char *env_var, const char *prefix)
{
  char *old_env = lms_strdup(getenv(env_var));

  //there should only be one instance of the prefix in env
  char *env_rm = strstr(old_env, prefix);

  //shouldn't get to here.. but lets be safe.
  if (NULL == env_rm)
    return;

  size_t len = 0;
  while (env_rm[len] != ':' && env_rm[len] != '\0')
    len++;

  if (env_rm[len] == '\0')
  {
    //ok then, just add a \0 at the start, there is nothing to preserve
    env_rm[0] = '\0';

    //if we're not at the start of the entire envvar, remove trailing ':'
    if (env_rm != old_env)
    if (env_rm[-1] == ':')
      env_rm[-1] = '\0';
  }
  else
  {
    //shift the remainder up the string
    size_t plen = strlen(&env_rm[len]);
    memmove(env_rm, &env_rm[len], plen);
    env_rm[plen] = '\0';
  }

  setenv(env_var, old_env, 1);
  free(old_env);
}

void
ue_gen_script
(void)
{
  pid_t ppid = getppid();
  size_t len = snprintf(NULL, 0, "/tmp/.lms%d.sh", ppid);
  char buf[len+1]; memset(buf,0,len+1);

  snprintf(buf, len+1, "/tmp/.lms%d.sh", ppid);

  FILE *fp = fopen(buf, "w");
  if (NULL == fp)
  {
    fprintf(stderr, "lms-fatal: unable to open temporary file\n");
    return;
  }

  char *path_env = getenv(PLATFORM_PATH);
  char *incl_env = getenv(PLATFORM_INCL);
  char *libs_env = getenv(PLATFORM_LIBS);
  char *mods_env = getenv("_LMS_MODS");

  fprintf(fp, "export %s=%s\n", PLATFORM_PATH, path_env);
  fprintf(fp, "export %s=%s\n", PLATFORM_INCL, incl_env);
  fprintf(fp, "export %s=%s\n", PLATFORM_LIBS, libs_env);
  fprintf(fp, "export %s=%s\n", "_LMS_MODS",   mods_env);

  fclose(fp);
}
