// next_array[i][c] = i文字目以降で(1-index)文字cが初めて登場する1-indexを返す。(存在しない場合はn+1を返す)
vector<vector<int>> next_array(string s){
    int n = s.size();
    vector<vector<int>> res(n+1, vector<int>(26, n+1));
    for(int i = n-1; i >= 0; i--){
        res[i] = res[i+1];
        res[i][s[i]-'a'] = i+1; // 適宜0-indexに変更
    }
    return res;
}
