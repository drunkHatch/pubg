all: server client

server:
	gcc -pthread -std=c99 -o server server.c -w -lncurses

client:
	gcc -pthread -std=c99 -o client client.c -w -lncurses

clean:
	rm -f *.o server client

rs:
	./server 16 127.0.0.1 8989 1547543314

rc:
	./client 127.0.0.1 8989

