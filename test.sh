#!/usr/bin/expect -f

#/x04 is an EOF
#TEMPLATE
# expect "Minishell$> "
# send -- "\r"
# expect "Minishell$> "
# send -- "\r"
# expect "Minishell$> "
# send -- "\r"
# expect "Minishell$> "
# send -- "exit\r"

set timeout -1

spawn ./minishell


###########################################################
##         ERROR MANAGEMENT			 					###	
###########################################################


# Tests that displays "No such file or directory"

expect "Minishell$> "
send -- "< te.txt\r"
expect "Minishell$> "
send -- "< te.txt cat\r"
expect "Minishell$> "
send -- "<\\>\r"
# expect "Minishell$> "
# send -- "exit\r"

# Tests that displays "command not found"

expect "Minishell$> "
send -- "ca\r"
expect "Minishell$> "
send -- "wtftgif\r"
expect "Minishell$> "
send -- "ehco\r"
# expect "Minishell$> "
# send -- "exit\r"

###########################################################
##         TESTING REDIRECTIONS	 						###	
###########################################################


################  INPUT REDIRECTION (<)####################
expect "Minishell$> "
send -- "< test.txt cat\r"

expect "Minishell$> "
send -- "< test.txt cat | wc\r"

expect "Minishell$> "
send -- "< test.txt wc | cat | wc -l\r"

#expect "Minishell$> "
#send -- "exit\r"


################  HEREDOC (<<) 	##########################

expect "Minishell$> "
send -- "<< Cesar cat\r"
expect ""
send -- "pears\rapples\roranges\rCesar\r"

expect "Minishell$> "
send -- "<< Cesar cat | wc\r"
expect ""
send -- "pears\rapples\roranges\rCesar\r"

# expect "Minishell$> "
# send -- "exit\r"

################  OUTPUT_REDIRECTION (>)####################

expect "Minishell$> "
send -- "echo Cesar > test1.txt\r"
expect "Minishell$> "
send -- "echo Cesar Steven | wc > test2.txt\r"
expect "Minishell$> "
send -- "echo Cesar Steven | cat | wc -w > test3.txt\r"
# expect "Minishell$> "
# send -- "exit\r"

################  APPEND_REDIRECTION (>)####################

expect "Minishell$> "
send -- "echo Alexandra Robert >> test1.txt\r"
expect "Minishell$> "
send -- "echo Alexandra Robert | wc >> test2.txt\r"
expect "Minishell$> "
send -- "echo Cesar Alexandra Robert Steven| cat | wc -w >> test3.txt\r"
expect "Minishell$> "
send -- "exit\r"

###########################################################
##         TESTING REGULAR COMMANDS  					###	
###########################################################

# Test that returns nothing. New prompt "Minishell >" is displayed

# expect "Minishell$> "
# send -- "< test.txt\r"
# expect "Minishell$> "
# send -- "\$A\r"

# Testing echo "command" with single and double quote arguments 

# expect "Minishell$> "
# send -- "echo allo\r"
# expect "Minishell$> "
# send -- "echo \"allo\"\r"
# expect "Minishell$> "
# send -- "echo 'allo'\r"
# expect "Minishell$> "
# send -- "echo \"'allo'\"\r"
# expect "Minishell$> "
# send -- "echo '\"allo\"'\r"
# expect "Minishell$> "
# send -- "echo '\"a\"ll\"o\"'\r"
# expect "Minishell$> "
# send -- "exit\r"

#Tester des commandes ou series de commandes particulieres 

# expect "Minishell$> "
# send -- "cat /dev/urandom | head -3\r"
# expect "Minishell$> "
# send -- "sleep 2 | sleep 2 | sleep 2\r"
# expect "Minishell$> "
# send -- "exit\r"








# expect "Minishell$> "
# send -- "< test.txt cat\r"
# expect "Minishell$> "
# send -- "ca\r"
# expect "Minishell$> "
# send -- "echo Cesar | wc\r"
# expect "Minishell$> "
# send -- "sort | uniq -c | sort -r | head -3\r"
# expect ""
# send -- "pears\rapples\roranges\r\x04"



expect eof