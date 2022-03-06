#ifndef ECO_BASE_CONFIG_H
#define ECO_BASE_CONFIG_H

#ifdef __linux__
#  define ECO_CONFIG_TARGET_OS LINUX
#  define ECO_CONFIG_UNIXLIKE
#else
# ifdef __FreeBSD__
#   define ECO_CONFIG_TARGET_OS FREEBSD
#   define ECO_CONFIG_UNIXLIKE
# else
#  ifdef __APPLE__
#    define ECO_CONFIG_TARGET_OS APPLE
#    define ECO_CONFIG_UNIXLIKE
#  else
#    ifdef _WIN32
#      define ECO_CONFIG_TARGET_OS WINDOWS
#      define ECO_CONFIG_WINDOWS
#    else
#      warning "Target OS could not be determined!"
#      define ECO_CONFIG_TARGET_OS UNKNOWN
#    endif
#  endif
# endif
#endif

#define ECO_CONFIG_USE_ZLIB_COMPRESSION

#endif
