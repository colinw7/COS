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
COSErr.cpp \
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
-std=c++11 \
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
