root=$(shell pwd)
INCLUDE=-I$(root)/../../lib/include
LIB=-L$(root)/../../lib/lib
LIB_NAME=-lmysqlclient

.PHONY:all
all:insert_data delete_data select_data net_cal

test:test.c
	gcc -o $@ $^ $(INCLUDE) $(LIB) $(LIB_NAME)

insert_data:insert_data.c
	gcc -o $@ $^ $(INCLUDE) $(LIB) $(LIB_NAME)
delete_data:delete_data.c
	gcc -o $@ $^ $(INCLUDE) $(LIB) $(LIB_NAME)
select_data:select_data.c
	gcc -o $@ $^ $(INCLUDE) $(LIB) $(LIB_NAME)
net_cal:net_cal.c
	gcc -o $@ $^ 


.PHONY:clean
clean:
	rm -rf insert_data delete_data select_data net_cal
