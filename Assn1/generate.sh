#!/usr/bin/env bash

report="Assignment1.md"
touch $report
command -v pandoc >/dev/null 2>&1 || { echo >&2 "I require pandoc but it's not installed.  Aborting."; rm "Assignment1.md"; exit 1; }
IFS=''

echo "# Assignment 1" > $report
echo "" >> $report
echo "" >> $report
question_file="BCSE-OS-assignments-I-2022.txt"
for (( i = 1; i < 8; i++ )); do
  echo "### Question $i" >> $report
  echo "" >> $report
  question=`sed "${i}q;d" $question_file`
  echo "> $question" >> $report
  echo "" >> $report

  echo "### Answer $i" >> $report
  echo "" >> $report
  answer_file="q${i}.sh"
  echo "> \`\`\`bash" >> $report
  while read line; do
    echo "> $line" >> $report
  done < $answer_file
  echo "> \`\`\`" >> $report
  echo "" >> $report
  if [[ $i -eq 6 ]]; then
    echo "![Question${i}](./${i}.png){ width=50% }" >> $report
  elif [[ $i -ge 4 ]]; then
    echo "![Question${i}](./${i}.png){ width=75% }" >> $report
  else
    echo "![Question${i}](./${i}.png)" >> $report
  fi
  echo "" >> $report
  echo "" >> $report
  echo "" >> $report
done

pandoc -s --verbose --wrap=preserve -V geometry:paperheight=16in -V geometry:margin=.25in -V headersize=18pt --from markdown $report --output "${report}.pdf" | while IFS= read -r line; do printf '[%s] %s\n' "$(date '+%Y-%m-%d %H:%M:%S')" "$line"; done

