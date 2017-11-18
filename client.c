
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <ctype.h>


int main(int argc, char* argv[])

{

        int sock, bytes_recieved;  
        char send_data[1024],recv_data[1024];
        struct hostent *host;
        struct sockaddr_in server_addr;  

        char result[1024];

        host = gethostbyname("127.0.0.1");

        if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Failed to create socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;     
        server_addr.sin_port = htons(25539);   
        server_addr.sin_addr = *((struct in_addr *)host->h_addr);
        bzero(&(server_addr.sin_zero),8); 

        printf(The client is up and running.");
        fflush(stdout);

        if (connect(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) 
        {
            perror("Failed to connect");
            exit(1);
        }




//send data and function nane to AWS
        while(1)
        {
          send(sock,argv[1],strlen(argv[1]), 0); 
          printf("\nThe client has sent the reduction type %s to AWS.\n", argv[1]);
          fflush(stdout); 

          FILE* fstream;
          fstream=fopen("nums.csv","r");

          char getdata[1000][1024];
          int i=0;
          do{
          fgets(getdata[i],1024,fstream);
          i=i+1;
          } while(!feof(fstream));


          int j;
          for(j=0;j<i-1;j++){
          	send(sock,getdata[j],strlen(getdata[j]), 0); 
          }
          send(sock,"end",strlen("end"), 0); 

          printf("The client has sent %d numbers to AWS\n",j);
          fflush(stdout);

          break;
        }   




       
//Receive the final result from AWS
          strcpy(result,"");

          while(1){
          bytes_recieved = recv(sock,result,1024,0);
          result[bytes_recieved] = '\0';
          if(strcmp(result,""))
             break;
          }         
          printf("The client has received reduction %s: %s\n",argv[1],result);
          fflush(stdout);
          
          close(sock);

return 0;
}
