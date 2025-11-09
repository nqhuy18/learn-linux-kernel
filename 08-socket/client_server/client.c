// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <sys/un.h> 
#include <string.h> 
#define PORT 8080 
#define SOCKET_PATH "/tmp/demo_socket"

int main(int argc, char const *argv[]) 
{
	struct sockaddr_un serv_addraddress; 
	int sock = 0, valread; 
	struct sockaddr_un serv_addr; 
	char *hello = "Hello from client"; 
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sun_family = AF_UNIX; 
	strncpy(serv_addr.sun_path, SOCKET_PATH, sizeof(serv_addr.sun_path)-1);   	   
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
	    printf("\nConnection Failed \n"); 
	    return -1; 
	}     
    char send_buff[1000];
    while(1) {
        memset(buffer, 0 , sizeof(buffer));
        if (fgets(send_buff, sizeof(send_buff), stdin) ==  NULL) {
			perror("fget");
		    break;
		}
		send_buff[strcspn(send_buff, "\n")] = 0;
        send(sock , send_buff , strlen(send_buff) , 0 );  
        valread = read( sock , buffer, 1024); 
        printf("%s\n",buffer ); 
    }
	return 0; 
}