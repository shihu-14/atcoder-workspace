// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
using ll = long long;
using ull = unsigned ll;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcountll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {0, 1, 0, -1};
const ll dy[] = {1, 0, -1, 0};
const ll dxx[] = {0, 1, 0, -1, 1, -1, 1, -1};
const ll dyy[] = {1, 0, -1, 0, 1, 1, -1, -1};
const ll LINF = 1001002003004005006ll;
const int INF = 1001001001;

int main() {
    int n, m, l;
    cin >> n >> m >> l;
    int tot = n + m + l;
    vector<int> card_numbers(tot);
    vector<int> card_owner(tot);
    rep(i, n){
        cin >> card_numbers[i];
        card_owner[i] = 0;
    }
    for (int i = n; i < n + m; i++) 
        cin >> card_numbers[i];
        card_owner[i] = 1;
    }
    for (int i = n + m; i < tot; i++) {
        cin >> card_numbers[i];
        card_owner[i] = 2;
    }
    vector<ll> three(tot, 1);
    rep(i, tot-1) {
        three[i+1] = three[i]*3;
    }
    ll init_state = 0;
    for (int i = 0; i < tot; i++) {
        init_state += three[i] * card_owner[i];
    }
    unordered_map<ll, int> memo[2];
    auto dfs = [&](auto dfs, ll state, int turn) -> bool{
        if (memo[turn].count(state)) return memo[turn][state];
        bool has_card = false;
        for (int i = 0; i < tot; i++) {
            int pos = (state / three[i]) % 3;
            if (pos == turn) {
                has_card = true;
                break;
            }
        }
        if (!has_card) return memo[turn][state] = 0;
        for (int i = 0; i < tot; i++){
            int pos_i = (state/three[i])%3;
            if (pos_i != turn) continue;
            ll next_state = state-three[i]*turn + three[i] * 2;
            vector<int> table_cards;
            for (int j = 0; j < tot; j++){
                int pos_j = (next_state / three[j])%3;
                if (pos_j == 2 && card_numbers[j] < card_numbers[i]){
                    table_cards.push_back(j);
                }
            }
            for (int k = -1; k < (int)table_cards.size(); k++){
                ll new_state = next_state;
                if (k != -1) {
                    int take_card = table_cards[k];
                    new_state = new_state - three[take_card] * 2 + three[take_card] * turn;
                }
                if (dfs(dfs, new_state, 1-turn) == false){
                    return memo[turn][state] = 1;
                }
            }
        }
        return memo[turn][state] = 0;
    };
    bool ans = dfs(dfs, init_state, 0);
    cout << (ans ? "Takahashi" : "Aoki") << endl;
    return 0;
}
