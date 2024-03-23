#include "utils.h"

extern struct ftpc_options o;

/*******************************/
/* ERROR, WARNING, ... STRINGS */
/*******************************/

char* msg_err[] =
{
	0,
	"bad server response",

	"Usage: ftpc [OPTION]... HOSTNAME\n"

	"Try '"
#if HAVE_GETOPT_LONG
	"ftpc --help"
#else
	"ftpc -h"
#endif
	"' for more information.",

	"connection failed",
	"giving up on login",
	"unexpected error",
	"heap allocation failed",
	"lost working directory",
	"double SIGINT, aborting",
	"no usable tmpdir"
};

char* msg_war[] = {
	"bye",
	"cannot open:",
	"error writing to:",
	"error reading from:",
	"transfer not confirmed",
	"file not found:",
	"cannot open temp file:",
	"unknown command:",
	"missing argument",
	"ignoring argument",
	"login failed",
	"operation failed or incomplete",
	"missing environment variable:",
	"resuming connection lost during",
	"interrupted during",
	"could not resume",
	"invalid source:",
	"invalid target:",
	"skipping:",
	"(local)",
	"(remote)"
};

#if HAVE_GETOPT_LONG

char* msg_usage = "ftpc [OPTION]... HOSTNAME\n\n"
"options: \n"
" -h, --help       show this help and quit\n"
" -v, --version    show version and quit\n"
" -p, --port N     specify the remote TCP port of the server (def:21)\n"
" -b, --buffer N   specify the size in bytes of the transfer buffer (def:8192)\n"
" -a, --attempts N number of manual login attempts\n"
" -q, --quiet      be (very) quiet\n"
" -n, --no-auto    disable autologin (~/.netrc)\n"
" -m, --no-manual  disable manual login\n"
" -g, --no-paging  disable output paging\n"
" -d, --dotfiles   do not ignore dot-files in transfers\n"
" -P, --no-path    disable path in prompt\n";

#else
char* msg_usage = "ftpc [OPTION]... HOSTNAME\n\n"
"options: \n"
" -h             show this help and quit\n"
" -v             show version and quit\n"
" -p N           specify the remote TCP port of the server (def:21)\n"
" -b N           specify the size in bytes of the transfer buffer (def:8192)\n"
" -a N           number of manual login attempts\n"
" -q             be (very) quiet\n"
" -n             disable autologin (~/.netrc)\n"
" -m             disable manual login\n"
" -g             disable output paging\n"
" -d             do not ignore dot-files in transfers\n"
" -P             disable path in prompt\n";
#endif

char* msg_version = " Copyright (c) 2003-2006";

char* msg_intro = 
"ftpc comes with ABSOLUTELY NO WARRANTY; this is free software.\n";

char* msg_help[] = {
	"h", "display this help",
	"l", "switch to local mode, following commands refer to local",
	"r", "switch to remote mode, following commands refer to remote",
	"pwd", "prompt working directory",
	"cd  PATH", "change working directory to PATH",
	"md  PATH", "make new directory PATH",
	"rd  PATH", "remove empty directory PATH",
	"rm  MASK", "delete regular files matching MASK, skip directories",
	"ls", "list current directory contents",
	"ls  MASK", "list files/dirs matching MASK",
	"dir", "pretty list of current directory",
	"cp  SRC TRG", "copy SRC to TRG. Behaves like /bin/cp -r",
	"mv  SRC TRG", "move SRC to TRG. Behaves like /bin/mv",
	"u   MASK DIR", "upload files/dirs matching MASK into remote DIR (recurs)",
	"d   MASK DIR", "download files/dirs matching MASK into local DIR (recurs)",
	"dr  MASK DIR", "same as above. If local file already exists, resume",
	"ren MASK FR TO", "replace FR to TO once in all filenames matching MASK",
	"p   FILE", "print contents of the FILE on the terminal",
	"e   FILE", "edit FILE",
	"q", "quit client",
	"quit|exit|bye", "aliases for q command",
	"<TAB>", "tab-completion"
};
int size_msg_help = sizeof( msg_help ) / sizeof( char * );

/************************************/
/* ERROR, WARNING, OUTPUT FUNCTIONS */
/************************************/


void ftpc_err(int code, char* msg)
  { fprintf(stderr, "ftpc: %s %s\n", msg_err[code], msg); exit(code); }

void ftpc_war(int code, char* msg)
  { fprintf(stderr, "ftpc: %s %s\n", msg_war[code], msg); }

void usage(void) { printf("\n%s\n", msg_usage); }
void version(void) { printf("%s\n", msg_version); }

void intro(void)
{
	int i;

	for (i = 0; i < 79; i++) fputc('*', stdout);
		fputc('\n', stdout);
	version();
	printf("\n%s\n", msg_intro);
	for (i = 0; i < 79; i++) fputc('*', stdout);
		fputc('\n', stdout);
}

void print_progress(char* op, char* fn, size_t fn_len, off_t cur_pos, off_t total_size)
{

	int abbrev;
	char* units[] = { "", "K", "M", "G", "T" };
	int u_idx;
	char line[WIDE_LINE_LEN];
	size_t line_len;
	double progress = 
	total_size > 0 ? (cur_pos * 100.0 / total_size) : 100.0;

	u_idx = 0;

	while ((total_size >> 10) >= 10 && u_idx < 4)
	{ 
		total_size >>= 10; cur_pos >>= 10; u_idx++;
	}

	snprintf(line, 75, "%s %lld%s/%lld%s (%4.*f%%) ",
		op, cur_pos, units[u_idx], total_size, units[u_idx],
		progress < 1.0 ? 2 : 1, progress);

	line[74] = '\0';		/* just to be sure that*/
	line_len = strlen(line);	/* line_len <= 74 */

	if (line_len + fn_len < 79)
	{
		strcpy(line + line_len, fn);
	}
	else
	{
		sprintf(line + line_len, "[...]%s", (fn + fn_len - (74 - line_len)));
	}

	printf("%s\r", line);
	fflush(stdout);
}


int local_file(char* name)
{
	struct stat buf;

	if (stat(name, &buf) != 0)
		return 0;

	return (S_ISREG(buf.st_mode) ? 1 : 
		S_ISDIR(buf.st_mode) ? 2 :
		3);
}

off_t local_size(FILE* f)
{
	off_t rv;
	fseeko(f, 0, SEEK_END); rv = ftello(f); fseeko(f, 0, SEEK_SET);
	return rv;
}

void* my_malloc(size_t s)
{
  void* rv;
  if (!(rv = malloc(s))) ftpc_err(6, "");
  return rv;
}

void* my_realloc(void* ptr, size_t s)
{
  void* rv;
  if (!(rv = realloc(ptr, s))) ftpc_err(6, "");
  return rv;
}

char* my_strdup(char* s)
{
  char* rv;
  if (!(rv = strdup(s))) ftpc_err(6, "");
  return rv;
}


/***************************/
/* OTHER UTILITY FUNCTIONS */
/***************************/

void cleanexit(void)
{
  exit(fclose(stdout) == 0 ? 0 : -1);
}

