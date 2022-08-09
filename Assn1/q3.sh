#!/bin/bash

number_of_files=`find . -type f 2>/dev/null | wc -l`
echo $number_of_files
