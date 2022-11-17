#!/usr/bin/expect -f

#/x04 is an EOF

set timeout -1

spawn ./minishell

# Test that returns nothing. New prompt "Minishell >" is displayed

expect "Minishell$> "
send -- "< test.txt\r"
expect "Minishell$> "
send -- "\$A\r"

# Tests that displays "No such file or directory"  OR "Command not found"

expect "Minishell$> "
send -- "< te.txt\r"
expect "Minishell$> "
send -- "< te.txt cat\r"
expect "Minishell$> "
send -- "ca\r"
expect "Minishell$> "
send -- "$\r"
expect "Minishell$> "
send -- "exit\r"

# Tests thats


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