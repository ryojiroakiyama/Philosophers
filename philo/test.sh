#!/bin/bash

RED="\033[31m"
BLUE="\033[34m"
CYAN="\033[36m"
BLACK="\033[30m"
GREEN="\033[32m"
WHITE="\033[37m"
YEELOW="\033[33m"
MAGENTA="\033[35m"
RESET="\033[0m"

do_test () {
	echo -e ${CYAN}${1}${RESET}
	eval $1
	echo ""
}

if [ "$1" = "err" ]; then
do_test './philo 10 400 100'
do_test './philo 10 400 100 100 3 3'
do_test './philo 10 400 "" 100'
do_test './philo 10 400 -100 100'
do_test './philo 10 400 100 100 -1'
do_test './philo 10a 400 100 100'
do_test './philo 0 400 100 100'
do_test './philo 10 400 2147483648 100'
do_test './philo 10 4294967300 2147483647 100 100'
do_test './philo 2147483648 400 100 100'
do_test './philo 214748364 400 100 4222222222222222222'
elif [ "$1" = "e1" ]; then
do_test './philo 10 200 100 100'
elif [ "$1" = "e2" ]; then
do_test './philo 10 210 100 100'
elif [ "$1" = "e3" ]; then
do_test './philo 10 120 60 60'
elif [ "$1" = "e4" ]; then
do_test './philo 10 130 60 60'
elif [ "$1" = "e5" ]; then
do_test './philo 4 203 100 100'
elif [ "$1" = "e6" ]; then
do_test './philo 1 203 100 100'
elif [ "$1" = "e7" ]; then
do_test './philo 2 203 100 100'
elif [ "$1" = "o1" ]; then
do_test './philo 11 300 100 100'
elif [ "$1" = "o2" ]; then
do_test './philo 11 350 100 100'
elif [ "$1" = "o3" ]; then
do_test './philo 11 180 60 60'
elif [ "$1" = "o4" ]; then
do_test './philo 11 190 60 60'
elif [ "$1" = "op1" ]; then
do_test './philo 11 350 100 100 0'
elif [ "$1" = "op2" ]; then
do_test './philo 11 350 100 100 1'
elif [ "$1" = "op3" ]; then
do_test './philo 10 50000 1 1 42'
elif [ "$1" = "op4" ]; then
do_test './philo 11 350 100 100 2 | grep "2 is eating" | wc -l'
elif [ "$1" = "op5" ]; then
do_test './philo 11 350 100 100 42 | grep "2 is eating" | wc -l'
elif [ "$1" = "op6" ]; then
do_test './philo 10 5000 1 1 42 | grep "3 is eat" | wc -l'
fi
