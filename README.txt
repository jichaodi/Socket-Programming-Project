a. Jichao Ding

b. 9672214539

c. In this project I implemented a model of computationa offloading where a single client offloads some computation to a server which in turn distributes the load over 3 backend servers. The server facing the client then collects the results from the backend and communicates the same to the client in the required format. The server communicating with the client is called AWS and the three   backend servers are named Back­Server A, Back­Server B and Back­Server C.  
The client and the AWS communicates over a TCP connection while the communication between AWS and the Back­Servers A, B and C is over a UDP connection.

d. There are 5 code files.
<client.c>
1. The client sends function name and data to AWS
2. At lase, AWS sends the final result back to client
<aws.c>
1. AWS reseives function name and data from client
2. AWS divides data to 3 segments and sends them respectly to serverA, serverB and serverC
3. After serverA, serverB and serverC deduct the result, AWS receives the results from A, B and C
4. Finally, it deducts the final result and sends it to client
<serverA.c>
1. ServerA receives function name and the first thirdth part of data transmitted from A
2. It deducts the result and sends it to AWS
<serverB.c>
1. ServerB receives function name and the first thirdth part of data transmitted from B
2. It deducts the result and sends it to AWS
<serverC.c>
1. ServerC receives function name and the first thirdth part of data transmitted from C
2. It deducts the result and sends it to AWS

e. There is nothing special other than following the set direction.

f. string

g. NA

h. NA








