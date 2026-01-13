ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME =  libft_malloc_$(HOSTTYPE).so
NAME_SIMLINK = libft_malloc.so

S_IMPL	 =  init.c newpage.c
D_IMPL	 =  impl/
SRC_IMPL =  $(addprefix $(D_IMPL), $(S_IMPL))

SRC	=  $(SRC_IMPL)  free.c  malloc.c  realloc.c
D_SRC	=  src/

D_INC := inc/ . $(D_LPRINTF)

D_BUILD = .build/
OBJ =  $(addprefix $(D_BUILD), $(SRC:.c=.o))


S_LPRINTF = ftprintf
D_LPRINTF = .printf/
L_PRINTF = $(D_LPRINTF)lib$(S_LPRINTF).a

CC =  cc 
FLAGS = -Wall -Wextra -Werror -fno-builtin -MMD -fPIC -g
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
	$(RM) $(NAME) $(NAME_SIMLINK)

re: fclean
	make all

test:	$(NAME)
	cc -I. -o$@ main.c -L. -Wl,-rpath,$(shell pwd) -lft_malloc_$(HOSTTYPE)
#	cc -L. -lft_malloc -o$@ main.c


DEPS = $(addprefix $(D_BUILD), $(SRC:.c=.d))
-include $(DEPS)

.PHONY: re fclean clean all test

