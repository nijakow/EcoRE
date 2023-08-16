#ifndef ECO_BASE_CONFIG_H
#define ECO_BASE_CONFIG_H

#define ECO_NUMERIC_VERSION_STRING "0.1vx"

#ifdef ECO_LATEST_GIT_COMMIT
#  define ECO_VERSION_STRING ECO_NUMERIC_VERSION_STRING "-" ECO_LATEST_GIT_COMMIT
#else
#  define ECO_VERSION_STRING ECO_NUMERIC_VERSION_STRING
#endif

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

#define Eco_WORD_SIZE (sizeof(void*))
#define Eco_ALLOCATION_ALIGNMENT Eco_WORD_SIZE

#define Eco_CONFIG_ALIGNED_ALLOC 1
// Eco_CONFIG_ALIGNED_ALLOC_STRATEGY Eco_CONFIG_ALIGNED_ALLOC

#endif
