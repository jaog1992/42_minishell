# Coder Alias
USER_NAME   = jde-orma

# Compiler and flags
CC          = gcc
CCFLAGS     = -Wall -Wextra -Werror

#Colors:

DEF_COLOR	= \033[0;39m
BLUE		= \033[0;94m
GREEN		= \033[0;92m

# Directories
SRC_DIR     = source/
INC_DIR     = source/
LIBFT_DIR   = libft/
OBJ_DIR     = source/
BIN_DIR     = ./

# Source files
SRC_FILE    = minishell_main.c minishell_signals.c minishell_exit.c minishell_var_expansion.c minishell_tokens.c
SRC         = $(addprefix $(SRC_DIR), $(SRC_FILE))
OBJ_FILE    = $(SRC_FILE:.c=.o)
OBJ         = $(addprefix $(OBJ_DIR), $(OBJ_FILE))

# Output executable
NAME        = minishell

# Binary name for your program
CODE_BIN    = $(BIN_DIR)$(NAME)

# Static library
LIBFT       = $(LIBFT_DIR)libft.a

# Variable to control if the library has been built
LIBFT_BUILT = no

all: $(CODE_BIN)

$(CODE_BIN): $(LIBFT) $(OBJ)
	@$(CC) $(CCFLAGS) -I$(INC_DIR) $(OBJ) -lreadline -L /Users/$(USER)/.brew/opt/readline/lib -I /Users/$(USER)/.brew/opt/readline/include -L$(LIBFT_DIR) -lft -o $@
	@echo "$(GREEN)✔ $(BLUE)$(USER_NAME)'s $(NAME) compilation$(DEF_COLOR)"

$(LIBFT):
	@if [ "$(LIBFT_BUILT)" = "no" ]; then \
		make -C $(LIBFT_DIR); \
		LIBFT_BUILT=yes; \
	fi

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@$(CC) $(CCFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ)
	@echo "$(GREEN)✔ $(BLUE)$(USER_NAME)'s $(NAME) .o files removal$(DEF_COLOR)"
	@make -C $(LIBFT_DIR) fclean

fclean: clean
	@$(RM) $(CODE_BIN)
	@echo "$(GREEN)✔ $(BLUE)$(USER_NAME)'s $(NAME) executable file removal$(DEF_COLOR)"
	@make -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re