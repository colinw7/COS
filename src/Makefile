CC = g++
AR = ar
RM = rm

CDEBUG = -g
LDEBUG = -g

INC_DIR = ../include
OBJ_DIR = ../obj
LIB_DIR = ../lib

all: $(LIB_DIR)/libCOS.a

SRC = \
COSUser.cpp \
COSFile.cpp \
COSProcess.cpp \
COSSignal.cpp \
COSStrSignal.cpp \
COSRand.cpp \
COSRead.cpp \
COSTimer.cpp \
COSTime.cpp \
COSNaN.cpp \

OBJS = $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRC))

CPPFLAGS = -I$(INC_DIR) -I.

clean:
	$(RM) -f $(OBJ_DIR)/*.o
	$(RM) -f $(LIB_DIR)/libCOS.a

$(OBJS): $(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $< -o $(OBJ_DIR)/$*.o $(CPPFLAGS)

.SUFFIXES: .cpp

$(LIB_DIR)/libCOS.a: $(OBJS)
	$(AR) crv $(LIB_DIR)/libCOS.a $(OBJS)
