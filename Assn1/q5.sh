#!/bin/bash

read -p "Enter Date 1 :- " date1
read -p "Enter Date 2 :- " date2

#evaluating the first day
day=${date1%%/*}
year=${date1##*/}
month=${date1#*/}
month=${month%/*}

_dow1=$(date --date="$month/$day/$year" '+%A')

#evaluating the second day
day=${date2%%/*}
year=${date2##*/}
month=${date2#*/}
month=${month%/*}

_dow2=$(date --date="$month/$day/$year" '+%A')

if [ $_dow1 -eq $_dow2 ]; then
	echo "Days are equal"
else
	echo "Days are not equal"
fi
