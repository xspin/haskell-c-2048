DIR_SRC = ./src
DIR_INC = ./include
DIR_OBJ = ./obj
DIR_BIN = ./bin

TARGET = main
BIN_TARGET = ${DIR_BIN}/${TARGET}

MAIN = Main
################################################
CPP = head ai run
C_SRC = $(patsubst %,${DIR_SRC}/%.cpp,$(CPP)) 
C_OBJ = $(patsubst %,${DIR_OBJ}/%.o,$(CPP)) 

HS = Ffi $(MAIN)
HS_SRC = $(patsubst %,${DIR_SRC}/%.hs,$(HS)) 
HS_OBJ = $(patsubst %,${DIR_OBJ}/%.o,$(HS)) 
################################################
CC = ghc --make -Wall
CFLAGS = -I${DIR_INC}
HFLAGS = -main-is $(MAIN) 
LFLAGS = -package wx

#ALL:
	#@echo $(C_SRC)
	#@echo $(C_OBJ)
	#@echo ${C}

${BIN_TARGET}: ${C_OBJ} ${HS_OBJ}
	$(CC) $(LFLAGS) $^ -o $@

${HS_OBJ}: ${HS_SRC}
	$(CC) $(HFLAGS) -c $^ -hidir $(DIR_OBJ) -odir $(DIR_OBJ)

${DIR_OBJ}/%.o: ${DIR_SRC}/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

CMAIN_CPP = main ai head run
cmain: $(patsubst %,${DIR_OBJ}/%.o,$(CMAIN_CPP)) 
	g++ $^ -o $(DIR_BIN)/cmain

.PHONY:clean
clean:
	rm -rf ${DIR_OBJ}/*
