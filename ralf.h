#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int displayDir(char *);
int dispalyFileInfo(char *);
void displayFilePerm(mode_t);
int displayLink(char *, off_t);
