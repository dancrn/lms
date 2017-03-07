#include "main.h"
#include "lms.h"
#include "modules.h"

int 
main
(int num_args, char **args)
{
  if (1 == num_args)
  {
    fprintf(stderr, "Error: No action specified\n");
    lms_help();

    return 1;
  }

  if (!strcmp(args[1], "avail"))
    return lms_list_available();
  else if (!strcmp(args[1], "list"))
    return lms_list_loaded();
  else if (!strcmp(args[1], "ls"))
    return lms_list_loaded();
  else if (!strcmp(args[1], "help"))
    return lms_help(), 0;

  //at this point, if we only have 2 args, we can't have any useful action
  if (2 == num_args)
  {
    fprintf(stderr, "Error: No parameter provided for action\n");
    lms_help();
    return 1;
  }

  if (!strcmp(args[1], "which"))
    return lms_list_which(num_args-2, &args[2]);
  else if (!strcmp(args[1], "load") || !strcmp(args[1], "add"))
    return lms_module_load(num_args-2, &args[2]);
  else if (!strcmp(args[1], "unload") || !strcmp(args[1], "rm") || !strcmp(args[1], "del"))
    return lms_module_unload(num_args-2, &args[2]);

  fprintf(stderr, "Error: Unknown action \"%s\"\n", args[1]);
  lms_help();

  return 1;
}

void
lms_help
(void)
{
  const char *help_str = {
    "Usage:\n"
    "  module [action]\n"
    "\n"
    "Where action is one of:\n"
    "  avail            - Lists available modules\n"
    "  list             - Lists loaded modules\n"
    "  which [binary]   - Lists module(s) that provide [binary]\n"
    "  load [module]    - Loads [module] (fully qualified name is not needed)\n"
    "  unload [module]  - Removes a module (if loaded)\n"
    "\n"
    "Aliases for list:\n"
    "  ls\n"
    "Aliases for load:\n"
    "  add\n"
    "\n"
    "Aliases for unload:\n"
    "  del, rm\n"
  };

  fprintf(stdout, "%s\n", help_str);
}
