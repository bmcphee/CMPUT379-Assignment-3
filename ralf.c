#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Error: please provide an argument);
		exit(1);
	}
	
}

/** 
 * @brief Recursively displays info about the directory given
 * 				using the displayFileInfo function
 * 
 * @param dir The directory to recursively print info about
 * 
 * @return Returns 0 if successful and 1 otherwise
 * 
 */
int displayDir(char *dir({
	DIR *pDir;
	struct dirent *pDirent;
	struct stat subCheck;
	
	// Try to open the directory, print error if NULL
	pDir = opendir(dir);
	if(pDir == NULL){
		printf("Cannot open directory: %s\n", dir);
		exit(1);
	}
	
	// Read the directory then print the info about it
	while((pDirent = readdir(pDir)) != NULL){
		displayFileInfo(pDirent.d_name);
	}
	
	// Rewind the directory and search for subfolders
	// If a subfolder is found recursively run displayDir
	rewinddir(pDir);
	while((pDirent = readdir(pDir) != NULL){
		lstat(pDirent.d_name, &subCheck);
		if(S_ISDIR(subCheck.st_mode)){
			displayDir(pDirent.d_name);
		}
	}
	
	// Close the directory to prevent memory leaks
	closedir(pDir);
}


/** 
 * @brief Displays the permissions, number of hard-links, owner,
 * 				group, size, modification date/time, filename of the
 * 				given file
 * 
 * @param filename The file to print the info about
 * 
 * @return Returns 0 if successful and 1 otherwise
 * ls -Ralf
 */
int displayFileInfo(char *filename){
	
}

/**
 * @brief Prints the file permission to the terminal
 * 
 * @param filename The filename to print the permissions for
 * 
 * @return Returns nothing
 */
 void displayFilePerm(char *filename){
	 
 }

