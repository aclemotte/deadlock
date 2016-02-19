INC_DIR=/home/arembedded/deadlock

deadlock: deadlock.c $(INC_DIR)/util/util.c
	gcc deadlock.c $(INC_DIR)/util/util.c -I$(INC_DIR) -lpthread -o app.o

all: deadlock

clean:
	rm -rf *.o	
