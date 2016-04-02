#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <grp.h>
#include "ralf.h"

/** 
 * @brief Recursively displays info about the directory given
 * 				using the displayFileInfo function
 * 
 * @param dir The directory to recursively print info about
 * 
 * @return Returns 0 if successful and 1 otherwise
 * 
 */
static int displayDir(char *dir){
	DIR *pDir;
	struct dirent *pDirent;
	struct stat info;
	int errorCheck = 0;
	char directory[512];
	printf(dir);
	// Try to open the directory, print error if NULL
	printf("Trying to open a new directory\n");
	pDir = opendir(dir);
	if(pDir == NULL){
		printf("Cannot open directory: %s\n", dir);
		return 1;
	}
	// Read the directory then print the info about it
	while((pDirent = readdir(pDir)) != NULL){
		strcpy(directory, "./");
		snprintf(directory, sizeof directory, "%s%s", directory, pDirent->d_name);
		lstat(directory, &info);
		errorCheck &= displayFileInfo(info, directory);
	}
	
	/*
	// Rewind the directory and search for subfolders
	// If a subfolder is found recursively run displayDir
	rewinddir(pDir);
	while(1){
		char *name;
		pDirent = readdir(pDir);
		if(!pDirent){
			break;
		}
		if(pDirent->d_type & DT_DIR){
			name = pDirent->d_name;
			stripSpaces(name);
			if ((strcmp(name, ".") != 0)  && (strcmp(name, "..") != 0)){
				printf("%s\n", pDirent->d_name);
				snprintf(directory, sizeof directory, "%s", name);
				displayDir(directory);
			}
		}
	}
	*/
	// Close the directory to prevent memory leaks
	closedir(pDir);
	return errorCheck;
}


/** 
 * @brief Displays the permissions, number of hard-links, owner,
 * 				group, size, modification date/time, filename of the
 * 				given file
 * 
 * @param filename The file to print the info about
 * 
 * @return Returns 0 if successful and 1 otherwise
 * 
 */
int displayFileInfo(struct stat info, char *filename){
	struct passwd *pw;
	struct group *g;
	struct tm *timeFormat;
	char timeBuffer[20];
	char permBuffer[11];
	int errorCheck = 0;
	int mode = 0;
	
	// Print the file permissions and the number of hard links
	mode = info.st_mode;
	displayFilePerm(mode, permBuffer);
	printf(permBuffer);
	printf("\t%d\t", (int) info.st_nlink);
	
	// Get the user name and group name and print them
	pw = getpwuid(info.st_uid);
	g = getgrgid(info.st_gid);
	printf("%s\t", pw->pw_name);
	printf("%s\t", g->gr_name);
	
	// Print the size
	printf("%d \t", (int) info.st_size);
	
	// Convert the time modified to the tm bufferuct, then format and print
	timeFormat = localtime(&(info.st_mtime));
	strftime(timeBuffer, 20, "%b %d %H:%M", timeFormat);
	printf("%s\t", timeBuffer);
	
	// If the file is a link use the displayLink function otherwise print the filename
	if(S_ISLNK(info.st_mode)){
		printf("!!!!!!!");
		errorCheck &= displayLink(filename);
	}
	else{
		printf("%s\n", filename);
	}
	
	return errorCheck;
}

/**
 * @brief Writes the file permissions to the provided buffer
 * 
 * @param mode The mode to print the permissions for
 * 
 * @param buffer The char buffer to write the permissions to
 * 
 * @return Returns nothing
 * 
 * @ref  johnloomis.org/ece537/notes/Files/Examples/ls2.html
 * 
 */
 void displayFilePerm(int mode, char *buffer){
	 // Set the default string
	 strcpy(buffer, "-----------");
	
	// Copy the relevant permissions to the buffer
	// File type
	if(S_ISDIR(mode)){
		buffer[0] = 'd';
	}
	if(S_ISLNK(mode)){
		buffer[0] = 'l';
	}
	// User permissions
	if(mode & S_IRUSR){
		buffer[1] = 'r';
	}
	if(mode & S_IWUSR){
		buffer[2] = 'w';
	}
	if(mode & S_IXUSR){
		buffer[3] = 'x';
	}
	//Group permissions
	if(mode & S_IRGRP){
		buffer[4] = 'r';
	}
	if(mode & S_IWGRP){
		buffer[5] = 'w';
	}
	if(mode & S_IXGRP){
		buffer[6] = 'x';
	}
	// Other permissions
	if(mode & S_IROTH){
		buffer[7] = 'r';
	}
	if(mode & S_IWOTH){
		buffer[8] = 'w';
	}
	if(mode & S_IXOTH){
		buffer[9] = 'x';
	}
 }
 
 /**
  * @brief Finds what the file given is linking to
  * 
  * @param filename The name of the file to find the link of
  * 
  * @param size The size of the file given
  * 
  * @return Returns 0 if successful and 0 otherwise
  * 
  */
 int displayLink(char *filename){
	ssize_t link;
	struct stat linkStat;
	lstat(filename, &linkStat);
	char *linkname = malloc(linkStat.st_size + 1);
	printf("There is a link here\n");
	// If there is insufficent memory return 1
	if(linkname == NULL){
		return 1;
	}
	
	link = readlink(filename, linkname, linkStat.st_size +1);
	
	// Check if there was an error with readlink
	if(link < 0){
		return 1;
	}
	
	// Append the link name with a terminator and print
	linkname[linkStat.st_size] = '\0';
	printf("%s -> %s\n", filename, linkname);
	free(linkname);
	return 0;
 }
 
 void stripSpaces(char *name){
	 char *source = name;
	 char *dest = name;
	 do while(*source == ' ') source++; while(*dest++ = *source++);
}
 
int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Error: please provide an argument");
		return 1;
	}
	int error = displayDir(argv[1]);
	return error;
}
