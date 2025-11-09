// Server side C/C++ program to demonstrate Socket programming 
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <sys/un.h> 
#include <string.h> 

#define SOCKET_PATH "/tmp/demo_socket"
int main(int argc, char const *argv[]) 
{ 
	int server_fd, new_socket, valread; 
	struct sockaddr_un address; 
	int opt = 1; 
	int addrlen = sizeof(address); 
	char buffer[1024] = {0}; 
	char *hello = "Hello from server"; 
	   
	// Creating socket file descriptor 
	if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == 0) 
	{ 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 

	address.sun_family = AF_UNIX; 
	strncpy(address.sun_path, SOCKET_PATH, sizeof(address.sun_path)-1);   
	unlink(SOCKET_PATH);
	// Forcefully attaching socket to the port 8080 
	if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) 
	{ 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(server_fd, 3) < 0) 
	{ 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	} 
	printf("Server is listening...\n");	
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) 
	{ 
			perror("accept"); 
			exit(EXIT_FAILURE); 
	} 
	char send_buff[1000];
	while (1) {
		valread = read( new_socket , buffer, 1024);
        printf("%s\n", buffer);

		memset(buffer, 0 , sizeof(buffer));
		if (fgets(send_buff, sizeof(send_buff), stdin) ==  NULL) {
			perror("fget");
			break;
		}
		send_buff[strcspn(send_buff, "\n")] = 0;
       	send(new_socket, send_buff, strlen(send_buff), 0);		
	}
	return 0;
}