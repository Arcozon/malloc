NAME =  libftprintf.a

D_SRC_FORMAT = format/
S_SRC_FORMAT = conversion.c  flags.c  precision.c  width.c
SRC_FORMAT = $(addprefix $(D_SRC_FORMAT), $(S_SRC_FORMAT))

D_SRC_CONV = conversion/
S_SRC_CONV =  fill_format.c  cpy_uint_b10_to_buff.c  cpy_ulong_b16_to_buff.c  s.c  u.c  x.c  p.c  percent.c  c.c  di.c
SRC_CONV = $(addprefix $(D_SRC_CONV), $(S_SRC_CONV))

S_SRC =  my_printf.c  write_buff.c  $(SRC_FORMAT)  $(SRC_CONV)
D_SRC =  src/

SRC =  $(addprefix $(D_SRC), $(S_SRC))

D_BUILD = .build/
OBJ =  $(addprefix $(D_BUILD), $(patsubst %.c,%.o,$(S_SRC)))

D_INC =  ./inc/

CC =  cc
FLAGS =  -Wall -Wextra -Werror -MMD -fPIC

AR =  ar -rcs

RM =  rm -rf

all:	$(NAME)

echosrc:
	@echo $(SRC)

bonus:	$(NAME)

$(NAME):	$(OBJ)
	$(AR)  $@ $^

$(OBJ): $(D_BUILD)%.o:	$(D_SRC)%.c
	@mkdir -p $(@D)
	$(CC) $(FLAGS) -c $< -o$@ -I$(D_INC)

clean:
	$(RM) $(D_BUILD)

fclean: clean
	$(RM)  $(NAME) 

re: fclean
	make all

-include DEPS = $(OBJ:.o=.d) 

.PHONY: re fclean clean all bonus $(CC) $(FLAGS) $(RM) $(AR)
