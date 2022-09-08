NAME = Minishell

# SRC = 

CC = @gcc
CFLAGS = -Werror -Wall -Wextra -g

OBJS = $(SRC:.c=.o)

GREEN = \033[1;32m

BRANCH ?= $(shell bash -c 'read -p "Branch: " branch; echo $$branch')
COMMIT ?= $(shell bash -c 'read -p "Commit: " commit; echo $$commit')
PATH_SUBMODULE ?= $(shell bash -c 'read -p "Adress of the submodule: " path_submodule; echo $$path_submodule')
NAME_SUBMODULE ?= $(shell bash -c 'read -p "Name of the submodule: " name_submodule; echo $$name_submodule')

all: $(NAME)

$(NAME): $(OBJS)
	@$(MAKE) -rmC ./Libft
	@$(CC) $(CFLAGS) $(OBJS) ./Libft/libft.a -o $(NAME)
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

init:
	@git remote add origin https://github.com/Steven1659139/Minishell.git
	# git remote intra 

p: add
	git commit -m $(COMMIT)
	@git push origin $(BRANCH)

merge:
	@git checkout master
	@git fetch
	@git pull
	@git merge $(BRANCH)

go:
	@git checkout $(BRANCH)