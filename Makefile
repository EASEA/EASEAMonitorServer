CFLAGS = -Wall -Wextra -g -O0 
#OBJECTS = $(patsubst src/%.c, %.o, $(wildcard src/*.c))
#OBJECTS_DIR = $(patsubst src/%.c, objets/%.o, $(wildcard src/*.c))
CC = g++
LIB=-lm -lpthread 
LIB_EASEA=$(EZ_PATH)libeasea/include
INC_PATH = include
SRC_PATH = src
OBJ_PATH = obj
BIN_PATH = bin
VPATH = src:include:obj:bin
TARGET=EASEAMonitor

ALL:$(TARGET) 

EASEAMonitor:mainFrame.o EASEAMonitorServer.o EASEAClientData.o
	$(CC) $(CFLAGS) -o $@ $(OBJ_PATH)/mainFrame.o $(OBJ_PATH)/EASEAMonitorServer.o $(OBJ_PATH)/EASEAClientData.o $(EZ_PATH)libeasea/libeasea.a  $(LIB)
	mv $@ bin/.

%.o: %.cpp %.hpp
	$(CC) $(CFLAGS)  -c $< -I $(INC_PATH) -o $(OBJ_PATH)/$@  -I$(LIB_EASEA) $(LIB)

clean : 
	rm -r doc/ $(OBJ_PATH)/*.o $(BIN_PATH)/$(TARGET) EASEAMonitor.tar.gz

tar:
	tar czvf EASEAMonitor.tar.gz $(SRC_PATH) $(INC_PATH) $(LIB_PATH) makefile

doc:
	doxygen Doxyfile
