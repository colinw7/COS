#ifndef STD_OS_H
#define STD_OS_H

#if   defined(__linux__)
# define OS_UNIX  1
# define OS_LINUX 1
#elif defined(_WIN32)
# define OS_WIN   1
#elif defined(mips)
# define OS_UNIX  1
# define OS_SGI   1
#elif defined(sun)
# define OS_UNIX  1
# define OS_SUN   1
#elif defined(hp700)
# define OS_UNIX  1
# define OS_HP    1
# define OS_HP700 1
#elif defined(__APPLE__)
# define OS_UNIX  1
# define OS_OSX   1
#else
# define OS_UNIX  1
#endif

#endif
