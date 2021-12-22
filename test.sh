#!/bin/bash

GREEN="\033[32m"
RESET="\033[39m"

do_test () {
	echo -e ${GREEN}${1}${RESET}
	eval $1
	echo ""
}

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
