all: server client

server:
	gcc -lncurses -pthread -std=c99 -o server server.c -w

client:
	gcc -lncurses -pthread -std=c99 -o client client.c -w

clean:
	rm -f *.o server client

rs:
	./server 127.0.0.1 8989 1547543314

rc:
	./client

