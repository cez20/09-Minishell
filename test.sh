#!/usr/bin/expect -f

#/x04 is an EOF

set timeout -1

spawn ./minishell

expect "Minishell$> "
send -- "< test.txt cat\r"
expect "Minishell$> "
send -- "ca\r"
expect "Minishell$> "
send -- "echo Cesar | wc\r"
expect "Minishell$> "
send -- "sort | uniq -c | sort -r | head -3\r"
expect ""
send -- "pears\rapples\roranges\r\x04"



expect eof