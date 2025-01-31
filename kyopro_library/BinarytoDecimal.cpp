string addStrings(const string& num1, const string& num2) {
    string result;
    int carry = 0, i = num1.size() - 1, j = num2.size() - 1;
    while (i >= 0 || j >= 0 || carry) {
        int sum = carry;
        if (i >= 0) sum += num1[i--] - '0';
        if (j >= 0) sum += num2[j--] - '0';
        result.push_back(sum % 10 + '0');
        carry = sum / 10;
    }
    reverse(result.begin(), result.end());
    return result;
}

string multiplyByTwo(const string& num) {
    string result;
    int carry = 0;
    for (int i = num.size() - 1; i >= 0; --i) {
        int prod = 2 * (num[i] - '0') + carry;
        result.push_back(prod % 10 + '0');
        carry = prod / 10;
    }
    if (carry) {
        result.push_back(carry + '0');
    }
    reverse(result.begin(), result.end());
    return result;
}

string binaryToDecimal(const string& binary) {
    string decimal = "0";
    for (char bit : binary) {
        decimal = multiplyByTwo(decimal);
        if (bit == '1') {
            decimal = addStrings(decimal, "1");
        }
    }
    return decimal;
}
