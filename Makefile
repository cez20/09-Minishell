NAME = Minishell

SRC = main.c

CC = @gcc 
CFLAGS = -Werror -Wall -Wextra -g

OBJS = $(SRC:.c=.o)

GREEN = \033[1;32m
READLINE = -lreadline -I /usr/local/opt/readline/include -L ~/.brew/opt/readline/lib -I ~/.brew/opt/readline/include
BRANCH ?= $(shell bash -c 'read -p "Branch: " branch; echo $$branch')
COMMIT ?= $(shell bash -c 'read -p "Commit: " commit; echo $$commit')
PATH_SUBMODULE ?= $(shell bash -c 'read -p "Adress of the submodule: " path_submodule; echo $$path_submodule')
NAME_SUBMODULE ?= $(shell bash -c 'read -p "Name of the submodule: " name_submodule; echo $$name_submodule')

all: $(NAME)

$(NAME): $(OBJS)
	@$(MAKE) -C ./Libft
	@$(CC) $(CFLAGS) $(OBJS) $(READLINE) ./Libft/libft.a -o $(NAME)
	@echo "$(GREEN)Everything is top shape tiguidou 🐥"

clean:
	@echo "Cleaning 🌪 🔥🌊"
	@$(MAKE) -C ./Libft fclean
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all

add:
	@git add *.c *.h Makefile TODO
	@git status

b:
	@git branch


update:
	@git fetch
	@git pull origin $(BRANCH)

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

go:
	@git checkout $(BRANCH)