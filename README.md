lms
===
lite module system.

usage
=====
lms can be used to list available modules, load modules, and unload modules. nothing really happens, if i'm honest. some environment variables are exported (and are not persistent), in particular, the `LD_LIBRARY_PATH`, `PATH`, and `C_INCLUDE_PATH` variables. that's kind of it, really.

    $ module list

will list available modules.

    $ module which gfortran

this will list what modules provide the `gfortran` binary. note, `which` will search through each module to see which ones provide the *binary* requested. this is not a search of module names. to load that module, run

    $ module load languages/gcc/4.9.1

then `gfortran` should work. note that the above and
    
    $ module load gcc

are equivilent, provided there is no ambiguity in only specifying the latter. finally, unloading modules is achieved with

    $ module unload gcc

