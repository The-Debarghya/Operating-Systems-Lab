#!/bin/bash
help(){
    echo "Usage: ./q4.sh [ -c | --clear ]
               [ -h | --help  ]
      ./q4.sh [ filename ]"
    exit 2
}
clear(){
  echo "Do you really want to clear the Trash?[Y/n]"
  read resp
  if [[ $resp -eq 'Y'  ||  $resp -eq 'y' ]] ; then
    rm -rf my-deleted-files/ -v
  else
    echo "Keeping the deleted files."
    exit 2
  fi
}
if [ "$#" -eq 0 ] || [[ ( ! $1 =~ ^(--help|-h|-help)$) && ( ! -f "$1") && 
                       ( ! $1 =~ ^(--clear|-c|-clear)$) ]] ; then
  echo "./q4.sh: Missing operand
Try ./q4.sh --help for more information."
  exit 2
fi
for i in $@; do
  if [[ $1 =~ ^(--help|-h|-help)$ ]]; then
    help
  fi
  if [[ $1 =~ ^(--clear|-c|-clear)$ ]]; then
    clear
  fi
  if [[ -f $i ]]; then
    if [[ -d "./my-deleted-files" ]]; then
      filename="./my-deleted-files/$i"
      if [[ -f $filename ]]; then
        mv $filename "${filename}_0"
        mv $i my-deleted-files/$i\_1
      elif compgen -G "${filename}_*" > /dev/null ; then
        filenum=`ls ./my-deleted-files | sort -g -r | head -n 1 | cut -d "_" -f2`
        filenum=$(($filenum+1))
        mv $i "./my-deleted-files/${i}_${filenum}"
      else
        mv $i my-deleted-files/
      fi
    else
      mkdir my-deleted-files/
      mv $i my-deleted-files/
    fi
  fi
done
