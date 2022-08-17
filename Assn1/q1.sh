#!/bin/bash
echo "Enter the value of variable 1"
read uv1
echo "Enter the value of variable 2"
read uv2
echo "$uv1, $uv2"
echo "$uv2 and $uv1"

echo "Reverse of 1st number:"
echo $uv1 | rev 
echo "Reverse pf 2nd number:"
echo $uv2 | rev
