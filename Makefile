ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME =  libft_malloc_$(HOSTTYPE).so

NAME_SIMLINK = libft_malloc.so

S_IMPL	 =  new_heap.c  debug.c
D_IMPL	 =  impl/
SRC_IMPL =  $(addprefix $(D_IMPL), $(S_IMPL))

SRC	=  $(SRC_IMPL)  free.c  malloc.c  realloc.c  show_alloc_mem.c  dump_heap.c  dump_large_heap.c
D_SRC	=  src/

D_BUILD = .build/
OBJ =  $(addprefix $(D_BUILD), $(SRC:.c=.o))

S_LPRINTF = ftprintf
D_LPRINTF = .printf/
L_PRINTF = $(D_LPRINTF)lib$(S_LPRINTF).a

SRC_TEST  =  main.c  realloc.c
D_TEST	  =  srcTest/
S_TEST	  =  $(addprefix $(D_TEST), $(SRC_TEST))
TEST_NAME =  test

CC =  cc 
FLAGS = -Wall -Wextra -Werror -fno-builtin -MMD -fPIC -g -march=native

D_INC := inc/ . $(D_LPRINTF)
IFLAGS = $(addprefix -I, $(D_INC))

RM =  rm -rf
LINKER = $(CC) $(FLAGS) -shared

MAKE += --no-print-directory

all:	$(NAME)

$(NAME):	$(OBJ) $(L_PRINTF)
	$(LINKER) -o$@ $^ -L$(D_LPRINTF) -l$(S_LPRINTF)
	ln -sf $@ $(NAME_SIMLINK) 

$(OBJ): $(D_BUILD)%.o:	$(D_SRC)%.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) $(IFLAGS) -c $< -o $@ 

$(L_PRINTF):
	@$(MAKE) $(D_LPRINTF)

clean:
	$(RM) $(D_BUILD)

fclean: clean
	$(RM) $(NAME) $(NAME_SIMLINK) $(TEST_NAME)

re: fclean
	@$(MAKE) all

$(TEST_NAME):	$(NAME) $(S_TEST)
	cc -I. $(IFLAGS) -o$@  -L. -Wl,-rpath,$(shell pwd) -lft_malloc_$(HOSTTYPE) -L$(D_LPRINTF) -l$(S_LPRINTF) $(S_TEST) -o$@

DEPS = $(addprefix $(D_BUILD), $(SRC:.c=.d))
-include $(DEPS)

.PHONY: re fclean clean all

