# Minishell 

This project aims to create our own shell using C. This shell must be based on bash and reproduce some of its functionalities. 

## Usage

Here are the instructions in order to compile the program:

| Command | Usage |
| --- | --- |
| `make` | Compiles program and creates executable ./minishell 
| `make clean` | Removes all object files (.o) of project and from libft
| `make fclean` | Removes all objects files (.o), all .a along with executable ./minishell
| `make re` | Removes all objects (.o) files and executable and recompiles them

The program does not take any argument. 

```shell
./minishell 
```	
Once executed, program will initiate a sub-shell in which it possible to execute commands as in bash

## Mandatory elements

The shell must:

- Display a prompt when waiting for a new command.
- Have a working history.
- Implement redirections:
	-  < should redirect input
    - '>' should redirect output.
	- << should be given a delimiter, then read the input until a line containing the
		delimiter is seen. However, it doesn’t have to update the history!
	- '>>' should redirect output in append mode.
- Implement pipes
- Handle environment variables ($ followed by a sequence of characters) which
		should expand to their values.
- Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash
- In interactive mode:
	- ctrl-C displays a new prompt on a new line.
	- ctrl-D exits the shell.
	- ctrl-\ does nothing
- Implement the following builtins:
	- echo with option -n
	- cd with only a relative or absolute path
	- pwd with no options
	- export with no options
	- unset with no options
	- env with no options or arguments
	- exit with no options


## Contribution

This project was realized with [Steven1659139](https://github.com/Steven1659139)
