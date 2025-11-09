// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h> 
#include <unistd.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h> 

#define PORT 8080 
#define IP "10.145.80.191"

int main(int argc, char const *argv[]) 
{
	int sock = 0, valread; 
	struct sockaddr_in serv_addr; 
	char *hello = "Hello from client"; 
	char buffer[1024] = {0}; 
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
	{ 
		printf("\n Socket creation error \n"); 
		return -1; 
	} 

	memset(&serv_addr, '0', sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);  

	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, IP , &serv_addr.sin_addr)<=0)  
	{ 
		printf("\nInvalid address/ Address not supported \n"); 
		return -1; 
	} 

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
	{ 
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