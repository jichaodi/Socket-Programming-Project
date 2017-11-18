
all: a b c d e
a: aws.c
	gcc -o runaws aws.c -lsocket -lnsl -lresolv
b: client.c
	gcc -o client client.c -lsocket -lnsl -lresolv
c:serverA.c
	gcc -o runserverA serverA.c -lsocket -lnsl -lresolv
d:serverB.c
	gcc -o runserverB serverB.c -lsocket -lnsl -lresolv
e:serverC.c
	gcc -o runserverC serverC.c -lsocket -lnsl -lresolv
serverA: runserverA
	./runserverA
serverB: runserverB
	./runserverB
serverC: runserverC
	./runserverC
aws: runaws
	./runaws
