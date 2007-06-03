#http://websvn.kde.org/trunk/KDE/kdeaddons/ConfigureChecks.cmake?rev=520160&view=log

include(CheckIncludeFile)
include(CheckIncludeFiles)
#include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckLibraryExists)
#include(CheckPrototypeExists)
include(CheckTypeSize)


#check for libz using the cmake supplied FindZLIB.cmake
if(ZLIB_FOUND)
   set(HAVE_LIBZ 1)
else(ZLIB_FOUND)
   set(HAVE_LIBZ 0)
endif(ZLIB_FOUND)

if(JPEG_FOUND)
   set(HAVE_LIBJPEG 1)
else(JPEG_FOUND)
   set(HAVE_LIBJPEG 0)
endif(JPEG_FOUND)

#check for png
if(PNG_FOUND)
   set(HAVE_LIBPNG 1)
else(PNG_FOUND)
   set(HAVE_LIBPNG 0)
endif(PNG_FOUND)

if(BZIP2_FOUND)
   set(HAVE_BZIP2_SUPPORT 1)
   if(BZIP2_NEED_PREFIX)
      set(NEED_BZ2_PREFIX 1)
   endif(BZIP2_NEED_PREFIX)
else(BZIP2_FOUND)
   set(HAVE_BZIP2_SUPPORT 0)
endif(BZIP2_FOUND)

if(CAIRO_FOUND)
   set(HAVE_LIBART 0)
   set(HAVE_CAIRO 1)
else(CAIRO_FOUND)
   set(HAVE_LIBART 1)
   set(HAVE_CAIRO 0)
endif(CAIRO_FOUND)

if(CARBON_FOUND)
        set(HAVE_CARBON 1)
else(CARBON_FOUND)
        set(HAVE_CARBON 0)
endif(CARBON_FOUND)

if(OPENSSL_FOUND)
   set(HAVE_SSL 1)
else(OPENSSL_FOUND)
   set(HAVE_SSL 0)
endif(OPENSSL_FOUND)


check_include_files(endian.h HAVE_ENDIAN_H)
check_include_files(fcntl.h HAVE_FCNTL_H)
check_include_files(dlfcn.h HAVE_DLFCN_H)
#check_include_files(crt_externs.h HAVE_CRT_EXTERNS_H)
#check_include_files(inttypes.h HAVE_INTTYPES_H)
#check_include_files(memory.h HAVE_MEMORY_H)
#check_include_files(stdint.h HAVE_STDINT_H)
#check_include_files(stdlib.h HAVE_STDLIB_H)
#check_include_files(strings.h HAVE_STRINGS_H)
#check_include_files(string.h HAVE_STRING_H)
#check_include_files(sys/bitypes.h HAVE_SYS_BITYPES_H)
#check_include_files(sys/stat.h HAVE_SYS_STAT_H)
#check_include_files(sys/types.h HAVE_SYS_TYPES_H)
check_include_files(unistd.h HAVE_UNISTD_H)

#check_function_exists(_NSGetEnviron   HAVE_NSGETENVIRON)
#check_function_exists(initgroups HAVE_INITGROUPS)
#check_function_exists(setenv     HAVE_SETENV)
#check_function_exists(strlcat    HAVE_STRLCAT)
#check_function_exists(strlcpy    HAVE_STRLCPY)

#check_symbol_exists(res_init        "sys/types.h;netinet/in.h;arpa/nameser.h;resolv.h" HAVE_RES_INIT)
#check_symbol_exists(snprintf        "stdio.h"                  HAVE_SNPRINTF)
#check_symbol_exists(vsnprintf       "stdio.h"                  HAVE_VSNPRINTF)


#check_prototype_exists(res_init "sys/types.h;netinet/in.h;arpa/nameser.h;resolv.h" HAVE_RES_INIT_PROTO)
#check_prototype_exists(initgroups "unistd.h;sys/types.h;unistd.h;grp.h" HAVE_INITGROUPS_PROTO)
#check_prototype_exists(setenv stdlib.h HAVE_SETENV_PROTO)
#check_prototype_exists(strlcat string.h HAVE_STRLCAT_PROTO)
#check_prototype_exists(strlcpy string.h HAVE_STRLCPY_PROTO)


#check_type_size("int" SIZEOF_INT)
#check_type_size("char *"  SIZEOF_CHAR_P)
#check_type_size("long" SIZEOF_LONG)
#check_type_size("short" SIZEOF_SHORT)
#check_type_size("size_t" SIZEOF_SIZE_T)
#check_type_size("unsigned long" SIZEOF_UNSIGNED_LONG)

