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
    enscript --line-numbers -p - --color=1 $file | pstopdf -i -o $DIRECTORY/${file}.pdf
done

# Generate pdfs for h
for file in *.h
do
    echo $file
    enscript --line-numbers -p - --color=1 $file | pstopdf -i -o $DIRECTORY/${file}.pdf
done

# Generate pdfs for txt
for file in *.txt
do
    echo $file
    enscript --line-numbers -p - --color=1 $file | pstopdf -i -o $DIRECTORY/${file}.pdf
done