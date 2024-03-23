#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define WIDE_LINE_LEN 160

void ftpc_err(int code, char* msg);

void ftpc_war(int code, char* msg);

void usage(void);
void version(void);

void intro(void);

void print_progress(char* op, char* fn, size_t fn_len, off_t cur_pos, off_t total_size);

int local_file(char* name);

off_t local_size(FILE* f);

void* my_malloc(size_t s);

void* my_realloc(void* ptr, size_t s);

char* my_strdup(char* s);

void cleanexit(void);

