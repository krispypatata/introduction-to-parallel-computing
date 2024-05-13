lab04:
	gcc -o lab04 lab04.c
	./lab04 10 8080 0

clean:
	rm -f *.o a.out
	ls | grep -v "\." | grep -v makefile | xargs rm

4:
	gcc -o gabinete_exer04_t5l gabinete_exer04_t5l.c -lpthread -lm
	./gabinete_exer04_t5l

3:
	gcc -o gabinete_exer03_t5l gabinete_exer03_t5l.c -lpthread -lm
	./gabinete_exer03_t5l

2:
	gcc -o gabinete_exer02_t5l gabinete_exer02_t5l.c -lpthread -lm 
	./gabinete_exer02_t5l

server:
	gcc server.c -o server 
	./server

client:
	gcc client.c -o client 
	./client

mat:
	gcc -o generateMatrix generateMatrix.c
	./generateMatrix

multi:
	gcc -o multi multi.c
	./multi

t:
	gcc -o try try.c

ta:
	./try 10 5000 0

tb:
	./try 10 5000 1

z:
	gcc -o z z.c
	./z

zc:
	gcc -o zc zc.c

gc:
	gcc -o gc gc.c
	./gc

ts:
	gcc -o tcpServer tcpServer.c
	./tcpServer
tc:
	gcc -o tcpClient tcpClient.c