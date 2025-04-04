#!/bin/bash

mkdir -p ../out  # 出力ディレクトリがなければ作る

total_score=0

for i in $(seq 0 49); do
    filename=$(printf "%04d" $i)
    echo "Running test case $filename..."
    
    # tester 実行してスコアを取得（stderr→stdout）
    score=$(./target/release/tester ../src/a.out < in/${filename}.txt 2>&1 | tee ../out/${filename}.txt | grep "Score =" | awk '{print $3}')
    
    echo "Score: $score"
    
    # スコアが数字なら加算
    if [[ $score =~ ^[0-9]+$ ]]; then
        total_score=$((total_score + score))
    fi
done

echo "Total Score over 50 cases: $total_score"
