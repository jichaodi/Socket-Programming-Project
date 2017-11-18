
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <ctype.h>

int main()
{
        int sock;
        int addr_len, bytes_read;
        char recv_data[1024];
        char send_data[1024],send_data2[1024];
        struct sockaddr_in server_addr , client_addr;

        struct hostent *host;
        host= (struct hostent *) gethostbyname((char *)"127.0.0.1");

	char* getdata[1000][1024];
        int data[1024];
        int i=0;
        int j;
        int result;

        if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Failed to creat Socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(22539);
        server_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(server_addr.sin_zero),8);


        if (bind(sock,(struct sockaddr *)&server_addr,
            sizeof(struct sockaddr)) == -1)
        {
            perror("Failed to Bind");
            exit(1);
        }

        addr_len = sizeof(struct sockaddr);
		
	printf("\nThe Server B is up and running using UDP on port 22539.");
        fflush(stdout);

        bytes_read = recvfrom(sock,recv_data,1024,0,
	                    (struct sockaddr *)&client_addr, &addr_len);
	recv_data[bytes_read] = '\0';


//receive data from AWS
	while (1)
	{
          do{
              bytes_read = recvfrom(sock,getdata[i],1024,0,
	                    (struct sockaddr *)&client_addr, &addr_len);
	      recv_data[bytes_read] = '\0';
              data[i]=atoi(getdata[i]);
              i++;
              }while(strcmp(getdata[i-1],"end"));

          printf("The Server B has received %d numbers",i-1);
          fflush(stdout);
          break;

        }
 


//calculate result
        if (strcmp(recv_data,"sum")==0)
            {result=0;
             for(j=0;j<i-1;j++)
                result=result+data[j];
             printf("The Server B has successfully finished the reduction %s: %d",recv_data,result);       
             fflush(stdout);}

        if (strcmp(recv_data,"min")==0)
            {result=data[0];
	     for(j=1;j<i-1;j++){
	        if(data[j]<result)
                  result=data[j];
             }
             printf("The Server B has successfully finished the reduction %s: %d",recv_data,result);       
             fflush(stdout);}

        if (strcmp(recv_data,"max")==0)
            {result=data[0];
	     for(j=1;j<i-1;j++){
	        if(data[j]>result)
                  result=data[j];
             }
             printf("The Server B has successfully finished the reduction %s: %d",recv_data,result);       
             fflush(stdout);}

        if (strcmp(recv_data,"sos")==0)
            {result=0;
             for(j=0;j<i-1;j++)
                result=result+data[j]*data[j];
             printf("The Server B has successfully finished the reduction %s: %d",recv_data,result);       
             fflush(stdout);}






	
//send result back to AWS

        host= (struct hostent *) gethostbyname((char *)"127.0.0.1");

	if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
	  perror("Failed to create socket.");
	  exit(1);
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(24539);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);
        
        sprintf(send_data,"%d",result);
        
        send_data2[0]='B';
        strcat(send_data2,send_data);

        sendto(sock,send_data2, strlen(send_data2), 0,
                       (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 
        printf("The Server B has successfully finished sending the reduction value to AWS server.");
        fflush(stdout);
        close(sock); 

        return 0;
}
