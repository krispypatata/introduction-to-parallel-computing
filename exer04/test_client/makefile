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

s:
	python3 lab04.py 25000 5000 0



s1:
	python3 lab04.py 25000 5001 1

s2: 
	python3 lab04.py 25000 5002 1

c1:
	python3 lab04.py 25000 5001 1

c2: 
	python3 lab04.py 25000 5002 1

c3:
	python3 lab04.py 25000 5003 1

c4:
	python3 lab04.py 25000 5004 1

c5:
	python3 lab04.py 25000 5005 1

c6:
	python3 lab04.py 25000 5006 1

c7:
	python3 lab04.py 25000 5007 1

c8:
	python3 lab04.py 25000 5008 1

c9:
	python3 lab04.py 25000 5009 1

c10:
	python3 lab04.py 25000 5010 1

c11:
	python3 lab04.py 25000 5011 1

c12:
	python3 lab04.py 25000 5012 1

c13:
	python3 lab04.py 25000 5013 1

c14:
	python3 lab04.py 25000 5014 1

c15:
	python3 lab04.py 25000 5015 1

c16:
	python3 lab04.py 25000 5016 1

perms:
	chmod +x s2_clients.sh
	chmod +x s4_clients.sh
	chmod +x s8_clients.sh
	chmod +x s16_clients.sh
	chmod +x p2_clients.sh
	chmod +x p4_clients.sh
	chmod +x p8_clients.sh
	chmod +x p16_clients.sh

s4:
	python3 lab04_serial.py 30000 5000 0

p4:
	python3 lab04_parallel.py 20000 5000 0

s5:
	python3 lab05_serial.py 30000 5000 0

p5:
	python3 lab05_parallel.py 30000 5000 0

kill:
	lsof -i :5000
