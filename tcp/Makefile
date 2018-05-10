.PHONY:all
all:tcpClient tcpServer

tcpClient:tcpClient.c
	gcc -o $@ $^ 
tcpServer:ttcpserver.c
	gcc -o $@ $^ -lpthread

.PHONY:clean
clean:
	rm -f tcpClient tcpServer
