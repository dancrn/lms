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

  //if we only have 2 args, we can't have any useful action
  if (2 == num_args)
  {
    fprintf(stderr, "Error: No parameter provided for action\n");
    lms_help();
    return 1;
  }

  if (!strcmp(args[1], "which"))
    return lms_list_which(num_args-2, &args[2]);
  else if (!strcmp(args[1], "load"))
    return lms_module_load(num_args-2, &args[2]);
  else if (!strcmp(args[1], "unload"))
    return lms_module_unload(num_args-2, &args[2]);

  fprintf(stderr, "Error: Unknown action \"%s\"\n", args[1]);
  lms_help();
  return 1;
}

void
lms_help
(void)
{
  printf("print some help here\n");
}
