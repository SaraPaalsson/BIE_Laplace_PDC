set(CMAKE_Fortran_COMPILER "/opt/cray/craype/2.2.1/bin/ftn")
set(CMAKE_Fortran_COMPILER_ARG1 "")
set(CMAKE_Fortran_COMPILER_ID "GNU")
set(CMAKE_Fortran_COMPILER_VERSION "4.9.1")
set(CMAKE_Fortran_COMPILER_WRAPPER "CrayPrgEnv")
set(CMAKE_Fortran_PLATFORM_ID "")
set(CMAKE_Fortran_SIMULATE_ID "")
set(CMAKE_Fortran_SIMULATE_VERSION "")

set(CMAKE_AR "/usr/bin/ar")
set(CMAKE_RANLIB "/usr/bin/ranlib")
set(CMAKE_COMPILER_IS_GNUG77 1)
set(CMAKE_Fortran_COMPILER_LOADED 1)
set(CMAKE_Fortran_COMPILER_WORKS TRUE)
set(CMAKE_Fortran_ABI_COMPILED TRUE)
set(CMAKE_COMPILER_IS_MINGW )
set(CMAKE_COMPILER_IS_CYGWIN )
if(CMAKE_COMPILER_IS_CYGWIN)
  set(CYGWIN 1)
  set(UNIX 1)
endif()

set(CMAKE_Fortran_COMPILER_ENV_VAR "FC")

set(CMAKE_Fortran_COMPILER_SUPPORTS_F90 1)

if(CMAKE_COMPILER_IS_MINGW)
  set(MINGW 1)
endif()
set(CMAKE_Fortran_COMPILER_ID_RUN 1)
set(CMAKE_Fortran_SOURCE_FILE_EXTENSIONS f;F;f77;F77;f90;F90;for;For;FOR;f95;F95)
set(CMAKE_Fortran_IGNORE_EXTENSIONS h;H;o;O;obj;OBJ;def;DEF;rc;RC)
set(CMAKE_Fortran_LINKER_PREFERENCE 20)
if(UNIX)
  set(CMAKE_Fortran_OUTPUT_EXTENSION .o)
else()
  set(CMAKE_Fortran_OUTPUT_EXTENSION .obj)
endif()

# Save compiler ABI information.
set(CMAKE_Fortran_SIZEOF_DATA_PTR "8")
set(CMAKE_Fortran_COMPILER_ABI "")
set(CMAKE_Fortran_LIBRARY_ARCHITECTURE "")

if(CMAKE_Fortran_SIZEOF_DATA_PTR AND NOT CMAKE_SIZEOF_VOID_P)
  set(CMAKE_SIZEOF_VOID_P "${CMAKE_Fortran_SIZEOF_DATA_PTR}")
endif()

if(CMAKE_Fortran_COMPILER_ABI)
  set(CMAKE_INTERNAL_PLATFORM_ABI "${CMAKE_Fortran_COMPILER_ABI}")
endif()

if(CMAKE_Fortran_LIBRARY_ARCHITECTURE)
  set(CMAKE_LIBRARY_ARCHITECTURE "")
endif()





set(CMAKE_Fortran_IMPLICIT_LINK_LIBRARIES "AtpSigHandler;AtpSigHCommData;pthread;sci_gnu_49_mpi;gfortran;m;sci_gnu_49;gfortran;m;mpichf90_gnu_49;rt;ugni;pthread;pmi;gfortran;m;mpich_gnu_49;rt;ugni;pthread;pmi;gfortran;m;mpl;pmi;pthread;alpslli;pthread;wlm_detect;ugni;pthread;alpsutil;pthread;rca;xpmem;udreg;gfortran;quadmath;pthread;gfortran;m;quadmath;m;c")
set(CMAKE_Fortran_IMPLICIT_LINK_DIRECTORIES "/opt/cray/libsci/13.0.1/GNU/49/haswell/lib;/opt/cray/dmapp/default/lib64;/opt/cray/mpt/7.0.4/gni/mpich2-gnu/49/lib;/opt/cray/rca/1.0.0-2.0502.57212.2.56.ari/lib64;/opt/cray/pmi/5.0.5-1.0000.10300.134.8.ari/lib64;/opt/cray/xpmem/0.1-2.0502.57015.1.15.ari/lib64;/opt/cray/ugni/6.0-1.0502.10245.9.9.ari/lib64;/opt/cray/udreg/2.3.2-1.0502.9889.2.20.ari/lib64;/opt/cray/alps/5.2.3-2.0502.9295.14.14.ari/lib64;/opt/cray/atp/1.7.5/lib;/opt/cray/wlm_detect/1.0-1.0502.57063.1.1.ari/lib64;/opt/gcc/4.9.1/snos/lib/gcc/x86_64-suse-linux/4.9.1;/opt/gcc/4.9.1/snos/lib64;/lib64;/usr/lib64;/opt/gcc/4.9.1/snos/lib")
set(CMAKE_Fortran_IMPLICIT_LINK_FRAMEWORK_DIRECTORIES "")
