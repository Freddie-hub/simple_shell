#ifndef SHELL_H
#define SHELL_H

// Standard C libraries
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

// Read/Write buffer sizes
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

// Command chaining types
#define CMD_NORM    0
#define CMD_OR      1
#define CMD_AND     2
#define CMD_CHAIN   3

// Convert number types
#define CONVERT_LOWERCASE   1
#define CONVERT_UNSIGNED    2

// 1 if using system getline()
#define USE_GETLINE 0
#define USE_STRTOK 0

// History file and maximum size
#define HIST_FILE   ".simple_shell_history"
#define HIST_MAX    4096

extern char **environ;

// Structure for singly linked list
typedef struct liststr {
    int num;
    char *str;
    struct liststr *next;
} list_t;

// Structure for passing information to functions
typedef struct passinfo {
    char *arg;
    char **argv;
    char *path;
    int argc;
    unsigned int line_count;
    int err_num;
    int linecount_flag;
    char *fname;
    list_t *env;
    list_t *history;
    list_t *alias;
    char **environ;
    int env_changed;
    int status;
    char **cmd_buf; // Pointer to command chaining buffer
    int cmd_buf_type; // Command chaining type
    int readfd; // File descriptor for reading input
    int histcount; // History line number count
} info_t;

// Function prototypes
int hsh(info_t *, char **);
int find_builtin(info_t *);
void find_cmd(info_t *);
void fork_cmd(info_t *);
// ... Other function prototypes ...

#endif

