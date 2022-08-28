#!/bin/bash

number_of_files=`find . -type f 2>/dev/null | wc -l`
echo "Number of files in current directory:$number_of_files"
