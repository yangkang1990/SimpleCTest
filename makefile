CC = gcc
RM = rm
RMFLAGS= -fr
MKDIR = mkdir


DIRS = objs exes
DIR_OBJS = objs
DIR_EXES = exes

EXE = smpUnit
#OBJS = func.o functest.o mytest.o
SRC = smpUtil.c smpUnit_main.c smpUnit.c example_smpunit.c
OBJS = $(SRC:.c=.o)
OBJS := $(addprefix $(DIR_OBJS)/, $(OBJS))
EXE := $(addprefix $(DIR_EXES)/, $(EXE))


.PHONY: all clean
all: $(DIRS) $(EXE)

$(EXE):$ $(OBJS) 
	echo "OBJS = $(OBJS)"
	$(CC) -o $@ $^
$(DIR_OBJS)/%.o:%.c
	$(CC) -o $@ -c $^

$(DIRS):
	$(MKDIR) $@

clean:
	$(RM) $(RMFLAGS)  $(OBJ) $(EXE) $(DIRS)