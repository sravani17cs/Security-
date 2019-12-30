#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#define _PROGRAM_NAME "whoami"
#include <stdlib.h>
#include <pwd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
static uid_t rUID; //the executer
static uid_t eUID;//owned by source
char* concat(const char *s1, const char *s2)
{
	const size_t len1 = strlen(s1);
	const size_t len2 = strlen(s2);
	char *result = malloc(len1 + len2 + 1); // +1 for the null-terminator
    	memcpy(result, s1, len1);
	memcpy(result + len1, s2, len2 + 1); // +1 to copy the null-terminator
//	printf("%s",result);
	return result;
}
void do_setuid (uid_t newUID)
{
	int status;

#ifdef _POSIX_SAVED_IDS
	status = seteuid ((newUID == -1)? eUID:newUID);//set rUID,EUID to the argument
#else
	status = setreuid (rUID, eUID); //if condition fails, change it back
#endif
	if (status < 0) {
	fprintf (stderr, "1:Couldn't set uid.\n");
	exit (status);
	}
}

void undo_setuid (void)
{
	  int status;

#ifdef _POSIX_SAVED_IDS
	status = seteuid (rUID);
#else
	status = setreuid (eUID, rUID);
#endif
	if (status < 0) {
	    fprintf (stderr, "Couldn't set uid.\n");
	       exit (status);
	}
}
int main(int argc, char **argv)
{
	 char name[40];		                                                                  
	 char line [256];
	char perm[256];  
	char *buffer;
	rUID = getuid();
	eUID = geteuid();
	FILE *access;

	if(argc == 1)
	 {
     		 printf("Missing commands");
  	 }
  	if(argc != 3) 
	{ /*3 because even the executables name string is on argc*/
 		 printf("Unexpected number of arguments\n");
 		 return -1;
  	}
	do_setuid(-1);//change uid to view
//	printf("%o\n", getuid());
	int i;
 
char *accFile = concat(argv[1],".acl");
	do_setuid(-1);//change uid to eUID to view file
	int fd = open(accFile, O_RDONLY);
	undo_setuid(); //undo it before there any error
	free(accFile);

	if (fd  < 0 )
	{
		close(fd); //if the file dont exist, break
		return -1;
	}
	
	access = fdopen(fd, "r");//get a FILE* to read
	if (access == NULL) //can't open it
	{
		printf("Cant open");
		return -1; //if there is no access file
	}  
  while (fgets(line, sizeof(line), access))
	{ //get each line in the config file
  	sscanf(line, "%s %s", name, perm); //get each thing in the config file, name == user, perm == permissions
  //	printf("%s %s",name, perm); check
  	buffer=(char *)malloc(10*sizeof(char));
  	buffer=getlogin(); //get the true ID to check if they are in the config file
  	char r[] = "r";
  	char b[] = "b";
        char w[] = "w";
    //check if user is in the config file with correct permissions
  	if ((strcmp(buffer, name) == 0) && ((strcmp(&r[0], perm) == 0) || (strcmp(&b[0], perm) == 0) || (strcmp(&w[0], perm) == 0) )) {
  	seteuid(-1);  // make bobs ID to be alice
//      printf( "%s %s", name, perm);	check	
	if ((strcmp(buffer, name) == 0) && ((strcmp(&r[0], perm) == 0) || (strcmp(&b[0], perm) == 0) || (strcmp(&w[0], perm) == 0) )) {
	seteuid(-1);  // make bobs ID to be alice
  		FILE *sourceFile;
  		FILE *destFile;
  		char ch;
  		sourceFile  = fopen(argv[1], "r"); //open basename.exl
  		destFile    = fopen(argv[2],   "w+"); //open the file that you will copy into
//		printf("I'm here");
  		// fopen() return NULL if unable to open file in given mode. 
  		if (sourceFile == NULL || destFile == NULL) {
  		// Unable to open file hence exit 
  			printf("\nUnable to open file.\n");
  			printf("Please check if file exists and you have read/write privilege.\n");
  			exit(EXIT_FAILURE);
  		} 
  		//Copy file contents character by character.
  		ch = fgetc(sourceFile);
  		while (ch != EOF){
  		// Write to destination file 
  			fputc(ch, destFile);
  			// Read next character from source file 
  			ch = fgetc(sourceFile);
  		}
  		printf("\nFile has been copied successfully.\n");
  		}
		undo_setuid(); //changed from Alice to Bob
  }	
  }	
 }




