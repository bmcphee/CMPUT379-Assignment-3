#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

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
	int errorCheck = 0
	
	// Try to open the directory, print error if NULL
	pDir = opendir(dir);
	if(pDir == NULL){
		printf("Cannot open directory: %s\n", dir);
		return 1;
	}
	
	// Read the directory then print the info about it
	while((pDirent = readdir(pDir)) != NULL){
		errorCheck &= errorCheckdisplayFileInfo(pDirent.d_name);
	}
	
	// Rewind the directory and search for subfolders
	// If a subfolder is found recursively run displayDir
	rewinddir(pDir);
	while((pDirent = readdir(pDir) != NULL){
		lstat(pDirent.d_name, &subCheck);
		if(S_ISDIR(subCheck.st_mode)){
			errorCheck &= displayDir(pDirent.d_name);
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
	struct group *group;
	struct tm *timeFormat;
	char timeBuffer[20];
	int errorCheck = 0
	
	// Print the file permissions and the number of hard links
	errorCheck &= displayFilePerm(filename);
	stat(filename, &info);
	printf("%d ", info.st_nlink);
	
	// Get the user name and group name and print them
	getpwuid(info.st_uid, &pw);
	getgrgid(info.st_gid, &group);
	printf("%s ", pw.pw_name);
	printf("%s ", group.gr_name);
	
	// Print the size
	printf("%d ", info.st_size);
	
	// Convert the time modified to the tm struct, then format and print
	timeFormat = localtime(&(info.st_mtime));
	strftime(timeBuffer, 20, "%b %d %H:%M", timeFormat);
	printf("%s ", timeBuffer);
	
	// Check if the file is a symbolic link deal with accordingly
	if(S_ISLINK(info.st_mode){
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
 */
 int displayFilePerm(char *filename){
	 
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
	 char *linkname = malloc[size + 1];
	 
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
 
