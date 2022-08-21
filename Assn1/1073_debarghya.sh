#!/bin/bash

greet() {
  echo $USER

  h=`date +%H`
  if [ $h -lt 12 ]; then
    echo "Good Morning"
  elif [ $h -lt 18 ]; then
    echo "Good Afternoon"
  else
    echo "Good Evening"
  fi
}

list_large_files(){
  bytes=$(($1-1))
  find . -size +${bytes}c -type f -exec ls -lh {} \+ 2>/dev/null
}

view_logfile() {
  if [[ -f "./logfile.txt" ]]; then
    cat logfile.txt
  else
    echo "No logs Yet!"
  fi
}

if [[ ! -f "./logfile.txt" ]]; then
  touch logfile.txt
fi

while [[ true ]]; do
  printf "Give a choice to execute the following commands:\n[1] Display greetings\n[2] List large files\n[3] Disk usage\n[4] View Log File\n[5] Exit\n"
  read -p "Your choice > " resp
  case $resp in
    1 )
      greet
      d=`date`
      echo "$USER%Display Greetings%$d" >> "./logfile.txt"
      ;;
    2 )
      read -p "Enter lower limit of filesize(in bytes):" bytes
      echo "=========================================================="
      echo "PERMS    LINKS   OWNER   GROUP    SIZE    DATE_MOD    NAME"
      echo "=========================================================="
      list_large_files "$bytes"
      d=`date`
      echo "$USER%List large files%%$d" >> "./logfile.txt"
      ;;
    3 )
      read -p "Give a foldername: " folder
      echo "========================================"
      echo "USAGE     NAME"
      echo "========================================"
      if [[ -d $folder ]]; then
        du -ah $folder 2>/dev/null
      else
        echo "Folder not found/not accessible"
      fi
      d=`date`
      echo "$USER%Disk Usage%%%%%%%%$d" >> "./logfile.txt"
      ;;
    4 )
      echo "=================================================================="
      echo "USER      COMMAND             DATE"
      echo "=================================================================="
      view_logfile
      d=`date`
      echo "$USER%View Logfile%%%%%%$d" >> "./logfile.txt"
      ;;
    5 )
      echo "Exiting the program..."
      d=`date`
      echo "$USER%Exit%%%%%%%%%%%%%%$d" >> "./logfile.txt"
      exit 2
      ;;
    * )
      echo "Invalid Option"
      ;;
  esac
done
