#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
#define rep(i, n) for (int i = 0; i < (n); ++i)
using ll = long long;
using P = pair<int, int>;
using T = tuple<int, int, int>;

int main(){
    string s1, s2, s3; cin >> s1 >> s2 >> s3;
    vector<vector<int>> id1(26), id2(26), id3(26);
    rep(i, s1.size()){
        id1[s1[i]-'a'].push_back(i);
    }
    rep(i, s2.size()){
        id2[s2[i]-'a'].push_back(i);
    }
    rep(i, s3.size()){
        id3[s3[i]-'a'].push_back(i);
    }

    int m = 0;
    set<char> st_tmp;
    rep(i, 26){
        if (id1[i].size() || id2[i].size() || id3[i].size()){
            st_tmp.insert(i+'a');
        } 
    }
    m = st_tmp.size();
    if (m > 10){
        cout << "UNSOLVABLE" << endl;
        return 0;
    }
    vector<char> ch_kind;
    for(auto c: st_tmp) ch_kind.push_back(c);
    vector<vector<int>> kind_all;
    vector<int> vec_tmp;
    auto f = [&](auto f, int now, int d) -> void{
        if (d == m){
            kind_all.push_back(vec_tmp);
            return;
        }
        for (int i = now+1; i < 10; i++){
            vec_tmp.push_back(i);
            f(f, i, d+1);
            vec_tmp.pop_back();
        }
    };
    f(f, -1, 0);
    for(auto v: kind_all){
        do{
        vector<int> nv1(s1.size()), nv2(s2.size()), nv3(s3.size());
        rep(i, m){
            rep(j, id1[ch_kind[i]-'a'].size()){
                nv1[id1[ch_kind[i]-'a'][j]] = v[i];
            }
            rep(j, id2[ch_kind[i]-'a'].size()){
                nv2[id2[ch_kind[i]-'a'][j]] = v[i];
            }
            rep(j, id3[ch_kind[i]-'a'].size()){
                nv3[id3[ch_kind[i]-'a'][j]] = v[i];
            }
        }
        if (nv1[0] == 0 || nv2[0] == 0 || nv3[0] == 0) continue;
        ll ten = 1, n1 = 0, n2 = 0, n3 = 0;
        rep(i, s1.size()){
            n1 += ten*nv1[s1.size()-1-i];
            ten *= 10;
        }
        ten = 1;
        rep(i, s2.size()){
            n2 += ten*nv2[s2.size()-1-i];
            ten *= 10;
        }
        ten = 1;
        rep(i, s3.size()){
            n3 += ten*nv3[s3.size()-1-i];
            ten *= 10;
        }
        if (n1+n2 == n3){
            cout << n1 << "\n" << n2 << "\n" << n3 << endl;
            return 0;
        }
        } while (next_permutation(v.begin(), v.end()));
    }
    cout << "UNSOLVABLE" << endl;
    return 0;
}
