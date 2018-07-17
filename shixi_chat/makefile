.PHONY:all
all:chat_ser chat_cli

chat_ser:chat_ser.o
	gcc -o $@ $^ -lpthread 
chat_cli:chat_cli.o
	gcc -o $@ $^
%.o:%.c
	gcc -c $< -o $@

.PHONY:clean
clean:
	rm -f chat_ser chat_cli *.o
