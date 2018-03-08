all: server client

server:
	gcc -lncurses -pthread -std=c99 -o server server.c -w

client:
	gcc -lncurses -pthread -std=c99 -o client client.c -w

clean:
	rm -f *.o server client

