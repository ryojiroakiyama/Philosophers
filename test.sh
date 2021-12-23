#!/bin/bash

GREEN="\033[32m"
RESET="\033[39m"

do_test () {
	echo -e ${GREEN}${1}${RESET}
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
do_test './philo 10 4294967300 100 100'
do_test './philo 4294967300 400 100 100'
elif [ "$1" = "e1" ]; then
do_test './philo 10 200 100 100'
elif [ "$1" = "e2" ]; then
do_test './philo 10 210 100 100'
elif [ "$1" = "e3" ]; then
do_test './philo 10 130 60 60'
elif [ "$1" = "e4" ]; then
do_test './philo 10 120 60 60'
elif [ "$1" = "o1" ]; then
do_test './philo 11 300 100 100'
elif [ "$1" = "o2" ]; then
do_test './philo 11 350 100 100'
elif [ "$1" = "o3" ]; then
do_test './philo 11 180 60 60'
elif [ "$1" = "o4" ]; then
do_test './philo 11 190 60 60'
fi
