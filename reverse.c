//gcc Server.c –o server
//Client Side Code
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#define PORT 4000
main() {
   struct sockaddr_in address;
   int my_socket = 0, valread;
   struct sockaddr_in server_address;
   char str[100];
   int l;
   printf("Enter a String:");
   fgets(str, 100, stdin); //read string until new line character is pressed
   char buffer[1024] = { 0 }; //create a buffer and fill with 0
   // Creating socket file descriptor
   if ((my_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      printf("\nUnable to create Socket \n");
      return -1;
   }
   memset(&server_address, '0', sizeof(server_address));
   server_address.sin_family = AF_INET;
   server_address.sin_port = htons(PORT);
   if (inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr) <= 0) {
      printf("\nThe address is not supported \n");
      return -1;
   }
   // connect the socket
   if (connect(my_socket, (struct sockaddr*)&server_address, sizeof(server_address)) < 0) {
      printf("\nUnable to connect to the server \n");
      return -1;
   }
   l = strlen(str);
   send(my_socket, str, sizeof(str), 0); // send message to server side
   valread = read(my_socket, str, l); // read message sent by server
   printf("%s\n", str);
}

//Server Side Code
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#define PORT 4000
char *strrev(char *str){
   char *p1, *p2;
   if (! str || ! *str)
      return str;
   for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2){
      *p1 ^= *p2;
      *p2 ^= *p1;
      *p1 ^= *p2;
   }
   return str;
}
main() {
   int server_fd, new_socket, valread;
   struct sockaddr_in address;
   char str[100];
   int addrlen = sizeof(address);
   char buffer[1024] = { 0 };
   int i, j, temp;
   int l;
   char* message = "Welcome to the server"; //initial message
   // Creating socket file descriptor
   if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
      perror("Socket connection failed");
      exit(EXIT_FAILURE);
   }
   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(PORT);
   if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) { //attack with socket 4000
      perror("Unable to bind with the socket 4000");
      exit(EXIT_FAILURE);
   }
   if (listen(server_fd, 3) < 0) {
      perror("Listning.....");
      exit(EXIT_FAILURE);
   }
   if ((new_socket = accept(server_fd, (struct sockaddr*)&address,(socklen_t*)&addrlen)) < 0) {
      perror("Accept");
      exit(EXIT_FAILURE);
   }
   // read string send by client
   valread = read(new_socket, str, sizeof(str));
   l = strlen(str);
   printf("\nString sent by client:%s\n", str);
   strrev(str); //reverse the string
   send(new_socket, str, sizeof(str), 0);
   printf("\nReversed Strng is sent\n");
}