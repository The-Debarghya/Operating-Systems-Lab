#!/bin/bash
echo "Enter the path to the file"
read file_path
if [[ -f "$file_path" ]]; then
  if [[ -r "$file_path" ]]; then
    number_of_lines=`wc --lines < $file_path`
    echo "Number of lines in the file :- $number_of_lines"
  else
    echo "No Read Permissions"
  fi
else
  echo "File specified doesn't exist!" > $file_path
  echo "New File created with filename:$file_path"
fi
