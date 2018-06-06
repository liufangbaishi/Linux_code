.PHONY:all
all:server client

server:select.c
	gcc -o $@ $^
client:client.c
	gcc -o $@ $^

.PHONY:
clean:
	rm -f server client
