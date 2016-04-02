#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

static int displayDir(char *);
int dispalyFileInfo(struct stat, char *);
void displayFilePerm(int, char *);
int displayLink(char *);
void stripSpaces(char *);
