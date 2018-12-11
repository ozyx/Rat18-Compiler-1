#!/bin/bash

# Generates PDFs for all *.cpp, *.h, and *.txt files in the current working directory

# This is where we want to save the pdfs
DIRECTORY="./docs/AS3"

# If directory does not exist, create it.
if [ ! -d $DIRECTORY ]; then
    mkdir $DIRECTORY
fi

# Generate pdfs for cpp
for file in *.cpp
do
    echo $file
    enscript --font=Courier7 --line-numbers -p - --color=1 $file | pstopdf -i -o $DIRECTORY/${file}.pdf
done

# Generate pdfs for h
for file in *.h
do
    echo $file
    enscript --font=Courier7 --line-numbers -p - --color=1 $file | pstopdf -i -o $DIRECTORY/${file}.pdf
done

# Generate pdfs for tests
for file in test*.txt
do
    echo $file
    enscript --font=Courier7 --line-numbers -p - --color=1 $file | pstopdf -i -o $DIRECTORY/${file}.pdf
done

enscript --font=Courier7 -p - --color=1 output.txt | pstopdf -i -o $DIRECTORY/output.txt.pdf