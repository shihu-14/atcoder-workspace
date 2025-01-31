void output_int128(__int128 value){
    if (value == 0){ cout << "0" << endl; return; } 
    __int128 num = value;
    if (num < 0){ cout << "-"; num = -num; } 
    string result;
    while (num > 0) {
        result += '0'+(num%10);
        num /= 10;
    }
    reverse(result.begin(), result.end());
    cout << result << endl;
}
__int128 input_int128(const string& s){
    __int128 result = 0;
    bool is_negative = false;
    int i = 0;
    if (s[0] == '-') { is_negative = true; i = 1;}
    for (;i<s.size(); i++){ result = result*10 + (s[i]-'0'); } 
    if (is_negative){ result = -result;}
    return result;
}

