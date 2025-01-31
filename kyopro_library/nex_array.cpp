// i文字目以降で(1-index)文字cが初めて登場する0-indexを返す。(存在しない場合は-1を返す)
vector<vector<int>> nex(string s){
    int n = s.size();
    vector<vector<int>> res(n+1, vector<int>(26, -1));
    for(int i = n-1; i >= 0; i--){
        res[i] = res[i+1];
        res[i][s[i]-'a'] = i; // 適宜1-indexに変更可能i->i+1
    }
    return res;
}
