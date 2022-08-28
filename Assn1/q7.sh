#!/bin/bash
echo "Filename to search in:"
read filename
echo "Word to search:"
read word1
echo "Word to replace:"
read word2

if grep -Fqw $word1 $filename 
then
  sed -i "s/$word1/$word2/g" $filename
  echo "Replaced all occurrences of ($word1) with ($word2)"
else
  echo "The entered string($word1) was not found in $filename"
fi
