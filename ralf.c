#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
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
int displayDir(char *dir){
	DIR *pDir;
	struct dirent *pDirent;
	struct stat subCheck;
	int errorCheck = 0;
	
	// Try to open the directory, print error if NULL
	pDir = opendir(dir);
	if(pDir == NULL){
		printf("Cannot open directory: %s\n", dir);
		return 1;
	}
	
	// Read the directory then print the info about it
	while((pDirent = readdir(pDir)) != NULL){
		errorCheck &= displayFileInfo(pDirent->d_name);
	}
	
	// Rewind the directory and search for subfolders
	// If a subfolder is found recursively run displayDir
	rewinddir(pDir);
	while((pDirent = readdir(pDir)) != NULL){
		lstat(pDirent->d_name, &subCheck);
		if(S_ISDIR(subCheck.st_mode)){
			errorCheck &= displayDir(pDirent->d_name);
		}
	}
	
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
int displayFileInfo(char *filename){
	struct stat info;
	struct passwd *pw;
	struct group *g;
	struct tm *timeFormat;
	char timeBuffer[20];
	int errorCheck = 0;
	
	// Print the file permissions and the number of hard links
	displayFilePerm(info.st_mode);
	stat(filename, &info);
	printf("%d ", (int) info.st_nlink);
	
	// Get the user name and group name and print them
	pw = getpwuid(info.st_uid);
	g = getgrgid(info.st_gid);
	printf("%s ", pw->pw_name);
	printf("%s ", g->gr_name);
	
	// Print the size
	printf("%d ", (int) info.st_size);
	
	// Convert the time modified to the tm struct, then format and print
	timeFormat = localtime(&(info.st_mtime));
	strftime(timeBuffer, 20, "%b %d %H:%M", timeFormat);
	printf("%s ", timeBuffer);
	
	// If the file is a link use the displayLink function otherwise print the filename
	if(S_ISLNK(info.st_mode)){
		errorCheck &= displayLink(filename, info.st_size);
	}
	else{
		printf("%s\n", filename);
	}
	
	return errorCheck;
}

/**
 * @brief Prints the file permission to the terminal with a
 * 				single space and the end of the output
 * 
 * @param filename The filename to print the permissions for
 * 
 * @return Returns 0 if successful and 1 otherwise
 * 
 * @ref  Exact code from the Lab 3 powerpoint
 * 
 */
 void displayFilePerm(mode_t mode){
	 // Compare mode against the references
	printf(S_ISDIR(mode) ? "d" : "-");
	printf((mode & S_IRUSR) ? "r" : "-");
	printf((mode & S_IWUSR) ? "w" : "-");
	printf((mode & S_IXUSR) ? "x" : "-");
	printf((mode & S_IRGRP) ? "r" : "-");
	printf((mode & S_IWGRP) ? "w" : "-");
	printf((mode & S_IXGRP) ? "x" : "-");
	printf((mode & S_IROTH) ? "r" : "-");
	printf((mode & S_IWOTH) ? "w" : "-");
	printf((mode & S_IXOTH) ? "x" : "-");
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
 int displayLink(char *filename, off_t size){
	ssize_t link;
	char *linkname = malloc(size + 1);
	
	// If there is insufficent memory return 1
	if(linkname == NULL){
		return 1;
	}
	
	link = readlink(filename, linkname, size +1);
	
	// Check if there was an error with readlink
	if(link < 0){
		return 1;
	}
	
	// Append the link name with a terminator and print
	linkname[size] = '\0';
	printf("%s -> %s\n", filename, linkname);
	free(linkname);
	return 0;
 }
 
int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Error: please provide an argument");
		return 1;
	}
	int error = displayDir(argv[1]);
	return displayDir(argv[1]);
}
