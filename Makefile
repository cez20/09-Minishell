NAME 			= minishell

SRCS_DIR   		= srcs/
SRCS_FILES		= main.c args_checker.c builtins.c execution.c free.c \
				  init.c parsing.c parsing_utils.c prepare_exec.c redirection.c signal.c \
				  utils_1.c utils_2.c utils_3.c utils_builtins.c utils_execution.c \
				  utils_print.c utils_redirection.c var_expansion.c utils_expansion.c error.c signal_utils.c
SRCS 			= $(addprefix $(SRCS_DIR), $(SRCS_FILES))

CC 				= @gcc
CFLAGS 			= -Werror -Wall -Wextra -g
READLINE 		=  -lcurses readline/libreadline.a  readline/libhistory.a
# READLINE		= $(LDFLAGS) #$(CPPFLAGS)
OBJS_DIR		= objs/
OBJS 			= $(patsubst $(SRCS_DIR)%.c, $(OBJS_DIR)%.o, $(SRCS))

HEADER_DIR      = include/

GREEN 			= \033[1;32m
RESET_COLOR 	= \033[0;00m
BRANCH 			?= $(shell bash -c 'read -p "Branch: " branch; echo $$branch')
COMMIT 			?= $(shell bash -c 'read -p "Commit: " commit; echo $$commit')
PATH_SUBMODULE 	?= $(shell bash -c 'read -p "Adress of the submodule: " path_submodule; echo $$path_submodule')
NAME_SUBMODULE 	?= $(shell bash -c 'read -p "Name of the submodule: " name_submodule; echo $$name_submodule')

$(OBJS_DIR)%.o: $(SRCS_DIR)%.c 
				$(CC) $(CFLAGS) -I$(HEADER_DIR) -c $< -o $@

all: 			objs $(NAME)

$(NAME): 		$(OBJS)
				@$(MAKE) -C ./Libft
				@$(MAKE) -C ./readline everything
				@$(CC) $(CFLAGS) $(OBJS) $(READLINE)   ./Libft/libft.a -o $(NAME)
				@echo "$(GREEN)Everything is top shape tiguidou 🐥$(RESET_COLOR)"

clean:
				@echo "Cleaning 🌪 🔥🌊"
				@$(MAKE) -C ./Libft fclean
				@$(MAKE) -C ./readline clean
				@rm -rf $(OBJS)
				@rm -rf $(OBJS_DIR)

fclean: 		clean
				@rm -rf $(NAME)

re: 			fclean all

add: 			fclean
				@git add *.c *.h Makefile TODO
				@git status

b:
	@git branch


update:
	@git fetch
	@git pull origin $(BRANCH)

cb:
	@./clone_branch.sh
sub:
	@git submodule update --init --recursive

add_sub:
	@git submodule add $(PATH_SUBMODULE)

del_sub:
	@vim .gitmodules
	@git add .gitmodules
	@vim .git/config
	@git rm --cached $(NAME_SUBMODULE)
	@rm -rf .git/modules/$(NAME_SUBMODULE)
	@git commit -m "Removed submodule"
	@rm -rf $(NAME_SUBMODULE)

# init:
	# @git remote add origin https://github.com/Steven1659139/Minishell.git
	# @git remote add intra git@vogsphere.42quebec.com:vogsphere/intra-uuid-76d7a526-3984-47ce-bf43-5082004a1aca-4391329-slavoie

p: add
	git commit -m $(COMMIT)
	@git push origin $(BRANCH)

merge:
	@git checkout master
	@git fetch
	@git pull origin $(BRANCH)
	@git merge $(BRANCH)


objs:
	@mkdir -p objs