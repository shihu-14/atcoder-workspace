string decimalToBinary(ll num) {
    if (num == 0) return "0";  // 特殊ケース: 0の場合

    string binary;
    while (num > 0) {
        binary.push_back((num % 2) + '0');  // 余りを文字として追加
        num /= 2;
    }
    
    reverse(binary.begin(), binary.end());  // 逆順にして完成
    return binary;
}
