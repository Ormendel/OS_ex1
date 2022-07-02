#include <stdio.h>
#include <cross-stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>


#define PORT 5550
#define IP "127.0.0.1" 
#define BUFFER_SIZE 1024
#define NUM_CONNECTIONS 10
#define SIZE 248

//Reference: https://idiotdeveloper.com/tcp-client-server-implementation-in-c/
int main(int argc, char** argv)
{
    int server_socket , client_socket , read_size;
    struct sockaddr_in server={0} , client={0};
    char server_message[2000];
    
    //Create the server socket
    server_socket = socket(AF_INET , SOCK_STREAM , 0);// 0 for defualt TCP PROTOCOL
    if (server_socket == -1)
    {
      printf("Could not create socket");
    }
    puts("Socket created");
    
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(IP);
    server.sin_port = htons( PORT );
    
    //Bind the socket to our specified IP and port
    if( bind(server_socket,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
      //print the error message
      perror("bind failed. Error");
      return 1;
    }
    puts("bind done");
    
    //Listen
    listen(server_socket , 3); //3 possible waiting connections
    
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    socklen_t client_socket_len = sizeof(client_socket);
    
    //accept connection from an incoming client
    client_socket = accept(server_socket, (struct sockaddr *)&client, &client_socket_len);
    if (client_socket < 0)
    {
      perror("accept failed");
      return 1;
    }
    puts("Connection accepted");
    
    //Receive a message from client
    while((read_size = recv(client_socket ,server_message, sizeof(server_message) , 0) > 0) )
    {
      //Send the message back to client
      //VERY IMPORTANT !
      write(client_socket , server_message , strlen(server_message));
      printf("---------------------------------------------\n");
      printf("Server got command: %s\n",server_message);  

      //Using system(command)
      setenv("message", server_message,1);  
      int ret = system("$message");
      if (WIFEXITED(ret) && !WEXITSTATUS(ret))
      {
        printf("Completed successfully\n"); ///successful
        printf("---------------------------------------------\n");
      }
      memset(server_message, 0, strlen(server_message));//reset message - now this string is empty
    }
    if(read_size == 0)
    {
      puts("Client disconnected");
    }
    else if(read_size == -1)
    {
      perror("recv failed");
    }

  
  return 0;
}