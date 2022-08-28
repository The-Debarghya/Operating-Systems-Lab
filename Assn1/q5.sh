#!/bin/bash
read -p "Enter Date 1 :- " date1
read -p "Enter Date 2 :- " date2
day=${date1%%/*}
year=${date1##*/}
month=${date1#*/}
month=${month%/*}
_dow1=$(date --date="$month/$day/$year" '+%A')
day=${date2%%/*}
year=${date2##*/}
month=${date2#*/}
month=${month%/*}
_dow2=$(date --date="$month/$day/$year" '+%A')
echo $_dow1
echo $_dow2
if [[ $_dow1 == $_dow2 ]]; then
	echo "Days are equal"
else
	echo "Days are not equal"
fi
