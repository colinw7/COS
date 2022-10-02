CC = g++
AR = ar
RM = rm

CDEBUG = -g

INC_DIR = ../include
OBJ_DIR = ../obj
LIB_DIR = ../lib

all: dirs $(LIB_DIR)/libCOS.a

dirs:
	@if [ ! -e ../obj ]; then mkdir ../obj; fi
	@if [ ! -e ../lib ]; then mkdir ../lib; fi
	@if [ ! -e ../bin ]; then mkdir ../bin; fi

SRC = \
COSErr.cpp \
COSExec.cpp \
COSFile.cpp \
COSLimit.cpp \
COSNaN.cpp \
COSProcess.cpp \
COSPty.cpp \
COSRand.cpp \
COSRead.cpp \
COSSignal.cpp \
COSStrSignal.cpp \
COSSystem.cpp \
COSTerm.cpp \
COSTime.cpp \
COSTimer.cpp \
COSUser.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = \
-std=c++17 \
-I$(INC_DIR) \
-I.

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(LIB_DIR)/libCOS.a

$(OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

.SUFFIXES: .cpp

$(LIB_DIR)/libCOS.a: $(OBJS)
	$(AR) crv $(LIB_DIR)/libCOS.a $(OBJS)
