lms
===
lite module system.

configuration
=============
lms will by default install to `/usr/local/bin`. to set the modules path, you must edit `src/base.h`. by default, the modules path is `/etc/lms-modules`.

installation
============
build lms with make;

    $ make

install with 

    # make install

this will install the lms binary, along with a supporting script. you'll want to edit `/etc/profile` to source `/usr/local/bin/lms_module_script`, so that lms will work. if you call lms, then by default, nothing will happen.

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

module files
============
a module is simply a JSON string in a file ending with `.json`. this is an example for gcc 4.8.2:

    {
      "name"      : "gcc",
      "version"   : "4.9.1",
      "category"  : "languages",
      "prefix"    : "/srv/modules/gcc/4.9.1",
      "provides"  : [ "gcc", "c++", "cpp", "g++", "gfortran" ]
    }

multiple modules in a module file is currently not supported.
