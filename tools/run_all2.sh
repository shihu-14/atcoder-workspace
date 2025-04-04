#!/bin/bash

mkdir -p ../out  # 出力ディレクトリがなければ作る

for i in $(seq 0 49); do
    filename=$(printf "%04d" $i)
    echo "Running test case $filename..."
    cargo run -r --bin tester ../src/a.out < in/${filename}.txt > ../out/${filename}.txt
done
# cargo run -r --bin tester ../src/a.out < in/0000.txt > ../out/0000.txt