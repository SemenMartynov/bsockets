CCFLAGS =	-O2 -g -Wall -fmessage-length=0

LIBS =

CLIENT =	client

SERVER =	server

$(CLIENT):	$(CLIENT).o
	$(CC) -o $(CLIENT) $(CLIENT).o $(LIBS)

$(SERVER):	$(SERVER).o
	$(CC) -o $(SERVER) $(SERVER).o $(LIBS)

all:	$(CLIENT) $(SERVER)

clean:
	rm -f $(CLIENT) $(SERVER) $(CLIENT).o $(SERVER).o
