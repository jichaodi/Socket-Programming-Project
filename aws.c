
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <ctype.h>
#include <string.h>

int main()
{
        int sock1, sock2, connected, bytes_recieved , true = 1; 
        int addr_len, bytes_read; 
        char send_data[1024] , recv_data[1024];
        char recv_data2[1024];  
        char recv_results[3][1024]; 

        struct sockaddr_in server_addr,client_addr;    
        int sin_size;

        struct hostent *host;
        host= (struct hostent *) gethostbyname((char *)"127.0.0.1");
        

        char* getdata[1000][1024];
        int data[1024];
        int i=0;
        int j;
        int result;
  
        if ((sock1 = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            perror("Failed to create socket");
            exit(1);
        }
        
        server_addr.sin_family = AF_INET;         
        server_addr.sin_port = htons(25539);     
        server_addr.sin_addr.s_addr = INADDR_ANY; 
        bzero(&(server_addr.sin_zero),8); 

        if (bind(sock1, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
            perror("Failed to bind");
            exit(1);
        }

        if (listen(sock1, 5) == -1) {
            perror("Failed to listen");
            exit(1);
        }
		
	printf("The AWS is up and running.\n");
        fflush(stdout);



//receive data and function number from client
       while(1)
        {  

            sin_size = sizeof(struct sockaddr_in);

            connected = accept(sock1, (struct sockaddr *)&client_addr,&sin_size);

              bytes_recieved = recv(connected,recv_data,1024,0);
              recv_data[bytes_recieved] = '\0';
 
              
              do{
              bytes_recieved = recv(connected,getdata[i],1024,0);
              recv_data[bytes_recieved] = '\0';
              i++;
              }while(strcmp(getdata[i-1],"end"));

              printf("The AWS has received %d numbers from the client using TCP over port from the client: %d\n",i-1,ntohs(client_addr.sin_port));
              fflush(stdout);

              break;

        }       






      //send data and function name to A/BC
	if ((sock2 = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
	{
	perror("Failed to create socket.");
	exit(1);
	}
//to A
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(21539);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);

        sendto(sock2, recv_data, strlen(recv_data), 0,
                       (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 

        while (1)
       {
          for(j=0;j<(i-1)/3;j++){
          	sendto(sock2, getdata[j], strlen(getdata[j]), 0,
                       (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 
          }
          sendto(sock2, "end", strlen("end"), 0,
                       (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 
          printf("The AWS sent %d numbers to Backend­Server A",j);
          fflush(stdout);
          break;
       }
//to B    
        server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(22539);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);

        sendto(sock2, recv_data, strlen(recv_data), 0,
                       (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 

        while (1)
       {
          for(j=(i-1)/3;j<2*(i-1)/3;j++){
          	sendto(sock2, getdata[j], strlen(getdata[j]), 0,
                       (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 
          }
          sendto(sock2, "end", strlen("end"), 0,
                       (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 
          printf("The AWS sent %d numbers to Backend­Server B",j/2);
          fflush(stdout);
          break;
       }
//to C
        server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(23539);
	server_addr.sin_addr = *((struct in_addr *)host->h_addr);
	bzero(&(server_addr.sin_zero),8);

        sendto(sock2, recv_data, strlen(recv_data), 0,
                       (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 

        while (1)
       {
          for(j=2*(i-1)/3;j<(i-1);j++){
          	sendto(sock2, getdata[j], strlen(getdata[j]), 0,
                       (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 
          }
          sendto(sock2, "end", strlen("end"), 0,
                       (struct sockaddr *)&server_addr, sizeof(struct sockaddr)); 
          printf("The AWS sent %d numbers to Backend­Server C",j/3);
          fflush(stdout);
          break;
       }

      close(sock2); 





        


//receive results from A/B/C
       if ((sock2 = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            perror("Failed to creat Socket");
            exit(1);
        }

        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(24539);
        server_addr.sin_addr.s_addr = INADDR_ANY;
        bzero(&(server_addr.sin_zero),8);


        if (bind(sock2,(struct sockaddr *)&server_addr,
            sizeof(struct sockaddr)) == -1)
        {
            perror("Failed to Bind");
            exit(1);
        }

        addr_len = sizeof(struct sockaddr);

        j=0;
        while(j<3){
        strcpy(recv_data2,"");
        bytes_read = recvfrom(sock2,recv_data2,1024,0,
	                    (struct sockaddr *)&client_addr, &addr_len);
	recv_data2[bytes_read] = '\0';

        switch(recv_data2[0]) {
           case 'A':
           {
            j++;
            for (i=1;recv_data2[i]!='\0';i++){
                recv_results[0][i-1]=recv_data2[i];
            }
            recv_results[0][i]='\0';
           printf("The AWS received reduction result of %s from Backend­Server A using UDP over port A and it is %s",recv_data, recv_results[0]);
           fflush(stdout);
           break;}
           case 'B':
	   {
           j++;
	   for (i=1;recv_data2[i]!='\0';i++){
                recv_results[1][i-1]=recv_data2[i];
            }
            recv_results[1][i]='\0';
           printf("The AWS received reduction result of %s from Backend­Server B using UDP over port B and it is %s", recv_data, recv_results[1]);
           fflush(stdout);
           break;}
           case 'C':
	   {
           j++;
	   for (i=1;recv_data2[i]!='\0';i++){
                recv_results[2][i-1]=recv_data2[i];
            }
            recv_results[2][i]='\0';
           printf("The AWS received reduction result of %s from Backend­Server C using UDP over port C and it is %s", recv_data, recv_results[2]);
           fflush(stdout);
           break;}
          }
        }

        close(sock2); 



//calculate final result
        for(i=0;i<3;i++)
           data[i]=atoi(recv_results[i]);
     
        if (strcmp(recv_data,"sum")==0)
            {result=0;
             for(j=0;j<3;j++)
                result=result+data[j];
             printf("The AWS has successfully finished the reduction %s: %d",recv_data,result);       
             fflush(stdout);}

        if (strcmp(recv_data,"min")==0)
            {result=data[0];
	     for(j=1;j<3;j++){
	        if(data[j]<result)
                  result=data[j];
             }
             printf("The AWS has successfully finished the reduction %s: %d",recv_data,result);       
             fflush(stdout);}

        if (strcmp(recv_data,"max")==0)
            {result=data[0];
	     for(j=1;j<3;j++){
	        if(data[j]>result)
                  result=data[j];
             }
             printf("The AWS has successfully finished the reduction %s: %d",recv_data,result);       
             fflush(stdout);}

        if (strcmp(recv_data,"sos")==0)
            {result=0;
             for(j=0;j<3;j++)
                result=result+data[j]*data[j];
             printf("The AWS has successfully finished the reduction %s: %d",recv_data,result);       
             fflush(stdout);}
        






  
//send final result to client

        sprintf(send_data,"%d",result);
        send(connected, send_data,strlen(send_data), 0);

        printf("The AWS has successfully finished sending the reduction value to client.");
        fflush(stdout);

        close(sock1);
      return 0;  
} 
