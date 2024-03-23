#ifndef AUTO_INCLUDES_H
#define AUTO_INCLUDES_H


/* The size of a `int', as computed by sizeof. */
#define SIZEOF_INT 4

/* The size of a `long', as computed by sizeof. */
#define SIZEOF_LONG 4

/* The size of a `long long', as computed by sizeof. */
#define SIZEOF_LONG_LONG 8

/* The size of a `off_t', as computed by sizeof. */
#define SIZEOF_OFF_T 8

/* Version number of package */
#define VERSION "0.1"

/* Number of bits in a file offset, on hosts where this is settable. */
#define _FILE_OFFSET_BITS 64

/* Define to 1 to make fseeko visible on some hosts (e.g. glibc 2.2). */
#define _LARGEFILE_SOURCE 1

#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <memory.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <dirent.h>
#include <fnmatch.h>
#include <errno.h>
#include <fcntl.h>
#include <getopt.h>

#endif

