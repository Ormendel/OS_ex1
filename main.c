/*
Dear checker,
the answers for the theoritcal questions are at the bottom of the files in comments =)
*/


//INCLUDING LIBRARIES
#include "main.h" /*our declarations for implemented functions*/
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <errno.h>
#include <fcntl.h> // For file descriptors
#include <stdbool.h>

//DEFINES
#define EXITCMD "EXIT"
#define ECHOCMD "ECHO"
#define CDCMD "CD"
#define DELETECMD "DELETE"
#define COPYCMD "COPY"
#define echo_size 4
#define delete_size 6
#define SIZE 248
#define PORT 5550
#define IP "127.0.0.1"


//The port we define is communicating to the server
/*********************************************ourShell.c is representing CLIENT SIDE*********************************************/
/*********************************************when typing TCP PORT*********************************************/

//This function returns the full current directory path using getcwd function
char *get_current_dir(char s[], int size)
{
    return getcwd(NULL, 0);
}
/*This function prints to the user using ECHO command:
 Input: ECHO X
 Output: X 
 
 Input: ECHO "X"
 Output: X
 
 Input: ECHO "X
 Output: "X
 
 Input: ECHO X"
 Output: X"
 
 Otherwise - no print
 */
void echo_out(const char *command, int firstSpaceIndex)
{
    char echo_output[SIZE-echo_size];
    int j=0;
    for(int i=firstSpaceIndex+1;command[i]!='\0';++i,++j)
    {
        echo_output[j]=command[i];
    }
    echo_output[j]='\0';
    //When we reached here, there are 2 options: echo "output" or echo output
    if(echo_output[0]=='"'&&echo_output[j-1]=='"')// for double qouates""
    {
        for(int i=1;i<=j-2;++i)
        {
            printf("%c",echo_output[i]);
        }
        printf("\n");
    }
    else printf("%s\n",echo_output); 
}

//Bio: https://www.youtube.com/watch?v=jFlyXUskBSI&ab_channel=PortfolioCourses (How to delete files in C, remove works similar to unlink)
void delete_file(char command[], int firstSpaceIndex)
{
    char delete_file_name[SIZE-delete_size];
    int j=0;    
    for(int i=firstSpaceIndex+1;command[i]!='\0';++i,++j)//Starting from index 7 - after the space
    {
        delete_file_name[j]=command[i];
    }
    delete_file_name[j]='\0';
    //When we reached here, we need to delete the file. 0 if success, -1 otherwise
    if(unlink(delete_file_name)!=0)
    {
        fprintf(stderr, "Error: %d\n",errno);
        perror("Error msg");
    }
    else printf("%s was deleted successfully\n",delete_file_name);
}

//bio: https://stackoverflow.com/questions/14150431/copying-a-file-in-c-with-fwrite
void copyFile(const char *src, const char *dest)
{
    // Variable definition
    char cTemp;
    FILE *fsrc = fopen(src, "rb");
    if(fsrc==NULL)
    {
        fprintf(stderr, "Error: %d\n",errno);
        perror("Error msg");
    }
    else
    {
        FILE *fdest = fopen(dest, "wb");//If exists or not, it's not a problem
        // Code section

        // Read from the source file - "Copy" char by char
        while(fread(&cTemp, 1, 1, fsrc) == 1)
        {
            // Write to the target file - "Paste" char by char
            fwrite(&cTemp, 1, 1, fdest);
        }

        printf("The content inside %s was copied successfully to %s\n",src,dest);
        // Close The files 
        fclose(fsrc);
        fclose(fdest);
    }
} 


//Bio: https://www.youtube.com/watch?v=j9yL30R6npk&ab_channel=CodeVault
void displayFiles(const char* dirname) 
{
    //Recursively
    DIR* dir = opendir(dirname);
    if (dir == NULL) 
    {
        return;
    }
    printf("Reading files in: %s\n", dirname);
    struct dirent* entity;
    entity = readdir(dir);
    while (entity != NULL) 
    {
        printf("%s\n",entity->d_name);
        if (entity->d_type == DT_DIR && strcmp(entity->d_name, ".") != 0 && strcmp(entity->d_name, "..") != 0) 
        {
            char path[100] = { 0 };
            strcat(path, dirname);
            strcat(path, "/");
            strcat(path, entity->d_name);
            displayFiles(path);
        }
        entity = readdir(dir);
    }

    closedir(dir);

    //bio: https://www.sanfoundry.com/c-program-list-files-directory/
    // DIR *d;
    // struct dirent *dir;
    // d = opendir(dirname);
    // if (d)
    // {
    //     while ((dir = readdir(d)) != NULL)
    //     {
    //         printf("%s\n", dir->d_name);
    //     }
    //     closedir(d);
    // }
    return;
}
void change_directory(const char* dirname, char* prev_dir)
{
    char *prev2_dir = getcwd(NULL, 0);
    bool all_empty = true;
    for(int i=0;i<strlen(dirname);++i)
    {
        if(dirname[i]!=' ')
        {
            all_empty = false;
            break;
        }
    }
    if(all_empty || strcmp(dirname,"~")==0 || strlen(dirname)==0) // CD with spaces (or not at all) or CD ~
    {
        chdir(getenv("HOME"));
        printf("Directory was changed to home directory successfully!\n");
        // printf("prev dir: %s\n",prev2_dir);
        printf("You're now at dir: %s\n",getcwd(NULL,0));
    }
    else if(strcmp(dirname,"/")==0 ) //move to root
    {
        chdir(dirname);
        printf("Directory was changed to / successfully!\n");
        // printf("prev dir: %s\n",prev2_dir);
        printf("You're now at dir: %s\n",getcwd(NULL,0));
    }
    else if(strcmp(dirname,"-")==0 ) //move to last directory you were in
    {
              
        chdir(prev_dir);
        printf("Directory was changed to %s successfully!\n", getcwd(NULL,0));
        // printf("prev dir: %s\n",prev_dir);
        printf("You're now at dir: %s\n",getcwd(NULL,0));
    }
    else // Check for nesting directory 
    { 
        DIR *d = opendir(dirname);
        if (d)
        {
            chdir(dirname);
            printf("Directory was changed to %s successfully!\n",getcwd(NULL,0));
            // printf("prev dir: %s\n",prev2_dir);
            printf("You're now at dir: %s\n",getcwd(NULL,0));
            closedir(d);//Very important to close after open! 
        }
        else 
        {
            printf("Directory %s doesn't exists\n",dirname);
        }
    }
    if(strcmp(prev2_dir,prev_dir)!=0)
    {
        strcpy(prev_dir,prev2_dir);
    }
    
}
//This function relates to the first time we see a space character- ' '
int findFirstSpace(const char* cmd)
{
    for(int i=0;i<strlen(cmd);++i)
    {
        if(cmd[i]==' ')
            return i;
    }
    return -1;
}
//This function returns a copy of our first word in our command (if there is any)
const char *findCMD(const char* cmd,int firstSpaceIndex)
{
    /*
    We defined "ans" as static. 
    Otherwise when the function ends, the variable (and the pointer you are returning) gets out of scope. 
    Since that memory is allocated on the stack, and it will get corrupted.
    */
    static char ans[SIZE];
    int i=0;
    while(firstSpaceIndex!=-1&&i!=firstSpaceIndex)
    {
        ans[i]=cmd[i];
        ++i;
    }
    ans[i]='\0';
    return ans;
}
int main(int argc, char** argv)
{
    char command[SIZE];
    char cd_name_dir[SIZE];
    char *prev_dir = getcwd(NULL,0);
    do
    {
        printf("yes master? > ");
        scanf("%[^\n]s",command);//The format specifier "%[^\n]" tells to the compiler that read the characters until "\n" is not found.
        int firstSpaceIndex=findFirstSpace(command);
        const char *cut_command=findCMD(command,firstSpaceIndex);       
        if(strcmp(command,"getcwd")==0)//Get current library we are at
        {
            printf("%s\n",get_current_dir(command,SIZE));
        }
        else if(strcmp(command,"DIR")==0)
        {
            displayFiles(".");
        }
        else if (strcmp(command, "TCP PORT") == 0)
        {
            /*
            References: 
            1. https://idiotdeveloper.com/tcp-client-server-implementation-in-c/
            2. https://www.binarytides.com/server-client-example-c-sockets-linux/
            */
           /************CLIENT SIDE************/
			
            int sock;
	        struct sockaddr_in server;
	        char server_reply[2000];
	
	        //Create socket
            sock = socket(AF_INET , SOCK_STREAM , 0);// 0 for defualt TCP PROTOCOL
            if (sock == -1)
            {
                printf("Could not create socket");
            }
            puts("Socket created");
            
            //Specify an address for the socket
	        server.sin_addr.s_addr = inet_addr(IP);
	        server.sin_family = AF_INET;
	        server.sin_port = htons( PORT );

	        //Connect to remote server, check for error with the connection
	        if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
            {
                perror("connect failed. Error");
                return 1;
            }
	
	        puts("Connected\n");
            // char str[SIZE];
            // scanf("%s",str);
            // printf("my check is %s\n", str);
            
            int saved_stdout = dup(STDOUT_FILENO);
            // here the newfd is the file descriptor of stdout (i.e. 1)
            dup2(sock, STDOUT_FILENO);

            // char command2[SIZE];
            char * temp[SIZE];
            // memset(command, 0, strlen(command));
            scanf("%s",temp);
            strcpy(command,temp);
            
            //keep communicating with server
            while ( strcmp(command, "LOCAL") != 0) 
            {
                
                /*
                As long as the given command (or line) is not LOCAL
                we should direct it towards system function
                */               
		
		        //Send some data
		        if( send(sock , command , strlen(command) , 0) < 0)
		        {
			        puts("Send failed");
			        return 1;
		        }
		
		        //Receive a reply from the server
                if( recv(sock , server_reply, sizeof(server_reply),  0) < 0)
                {
                    puts("recv failed");
                    break;
                }
                memset(temp, 0, sizeof(temp));
                scanf("%s",temp);
                strcpy(command,temp);
            }
            //If we got LOCAL command - close socket
            close(sock);
            /*
            Also we need to return to original stdout
            Reference: https://stackoverflow.com/questions/11042218/c-restore-stdout-to-terminal
            */
            dup2(saved_stdout,STDOUT_FILENO);
            close(saved_stdout);

            printf("You are now disconnected from the server.\n"); 

		}
        //ECHO HELLO WORLD -> HELLO WORLD
        //ECHO "HELLO WORLD" -> HELLO WORLD
        else if(strcmp(cut_command,ECHOCMD)==0)
        {
            echo_out(command,firstSpaceIndex);
        }
        else if(strcmp(cut_command,DELETECMD)==0)
        {
            delete_file(command,firstSpaceIndex);
        }
        //Currently CD command works only for getting inside an inner folder
        else if (strcmp(cut_command,CDCMD) == 0) 
        {
            int j = 0;
            for (int i = firstSpaceIndex+1; firstSpaceIndex!=-1 && i<strlen(command);++i,++j)
            {
                cd_name_dir[j] = command[i];
            }
            cd_name_dir[j]='\0';
            change_directory(cd_name_dir, &prev_dir); //Change the directory using chdir
            
            
            /*OPTIONS OF CD (in short)*/
            /*
            0. CD -> Move to home directory (like case 4) - DONE
            1. CD . -> Remain in current directory - DONE
            2. CD .. -> Move to parent directory - DONE
            3. CD ~ -> Move to home directory - DONE
            3.1 CD -> If nothing was pressed after, it's the same as CD ~ DONE
            4. CD - -> Move to the last directory which the user has been in  - DONE
            5. CD / -> Move to root directory - DONE
            
            */

           memset(cd_name_dir, 0, strlen(cd_name_dir));//RESET DIRECTORY NAME TO BE EMPTY!
            
        }
        else if(strcmp(cut_command,COPYCMD)==0)
        {
            char *token = strtok(command," ");
            char *src = strtok(NULL, " ");
            char *dest = strtok(NULL, " ");

            //We now have our source file (in our current directory), and our destination.
            //Call to function.
            copyFile(src,dest);

        }
        else if(strcmp(command, EXITCMD) != 0)//If we reached here, it's a system call
        {
            // system(command);

            
            // First we need to do dupliate the proccess with fork()
            // printf("My process is is %d\n ",getpid());
            char * args[SIZE];
            char *tok_command = strtok(command," ");
            args[0]=tok_command;
            int i=1;
            char *params = strtok(NULL, " ");
            while(params!=NULL)//In case there are more paramters to be added to args
            {
                args[i++]=params;
                params = strtok(NULL, " ");
            }
            args[i]=NULL;//Finally mark the final value as NULL 

            /*We are now using fork() command.
            Here is a great explanation:
            https://www.section.io/engineering-education/fork-in-c-programming-language/
            */
            pid_t fork_id = fork();
            if (fork_id < 0) /* error occurred */ 
            {
                fprintf(stderr, "Fork Failed");
            }
            
            else if (fork_id == 0) /* we are in child process */
            {
                // char * args[] = {cut_command,params,NULL};
                int status_code = execvp(args[0],args);
                if (status_code == -1 )
                {
                    printf("Error execvp\n");
                    // fprintf(stderr, "Error: %d\n",errno);
                    // perror("Error msg");
                }
                else 
                {
                    printf("Successfully executed\n");
                }
            }
            else /* parent process */
            {
                /* parent will wait for the child to complete */
                wait(NULL);
                /* When the child is ended, then the parent will continue to execute its code */
                printf("Child completed!\nNow back to parent (main)\n");             
            }     
        }
        
        // flushes the standard input
        // clears the input buffer - we must do it everytime
        while ((getchar()) != '\n');
    } while (strcmp(command, EXITCMD) != 0);
    return 0;
}


/*
Answers to THEORTICAL QUESTIONS:
1. The chdir command is a system function (system call) which is used to change the current working directory.
2. system command is using 'sys' library, and it's called system call.
3. unlink is a system call and a command line utility to delete files (this function is saved in unistd.h)
4. fopen, fread and fwrite are library functions
*/