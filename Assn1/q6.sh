#!/bin/bash
echo "Filename to search in:"
read filename
echo "Word to search:"
read word
if grep -Fqw $word $filename
then
  echo "The number of occurrences of ($word):"
  grep -Eow $word $filename | wc -l
  echo "Number of occurrences of ($word) in each line:"
  grep -won $word $filename | uniq -c | cut -d ":" -f1 | cut -d " " -f7,8 | while read -ra line ; do echo "Line No.:${line[1]} Count:${line[0]}" ; done
else
  echo "The entered string($word) was not found in ($filename)"
fi
