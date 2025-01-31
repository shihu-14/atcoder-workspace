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
using ull = unsigned long long;
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
    vector<int> a(n), b(m), c(l);
    rep(i, n) cin >> a[i];
    rep(i, m) cin >> b[i];
    rep(i, l) cin >> c[i];

    multiset<int> handt(a.begin(), a.end()), handa(b.begin(), b.end()), 
        table(c.begin(), c.end());

    map<tuple<multiset<int>, multiset<int>, multiset<int>, int>, bool> memo;

    auto dfs = [&](auto dfs, multiset<int>& handt, multiset<int>& handa, multiset<int>& table, int turn) -> bool {
        auto state = make_tuple(handt, handa, table, turn);
        if (memo.count(state)) return memo[state];

        multiset<int>& now_hand = (turn == 0) ? handt : handa;
        if (now_hand.empty()) return memo[state] = false;

        vector<int> hand_cards(now_hand.begin(), now_hand.end());
        for (int card : hand_cards) {
            now_hand.erase(now_hand.find(card));
            table.insert(card);
            vector<int> candidates;
            for (int t_card : table) {
                if (t_card < card) candidates.push_back(t_card);
            }
            
            bool opponent_win = dfs(dfs, handt, handa, table, 1 - turn);
            if (!opponent_win) {
                now_hand.insert(card);
                table.erase(table.find(card));
                return memo[state] = true;
            }

            for (int take_card : candidates) {
                table.erase(table.find(take_card));
                if (turn == 0) handt.insert(take_card);
                else handa.insert(take_card);

                opponent_win = dfs(dfs, handt, handa, table, 1 - turn);

                if (turn == 0) handt.erase(handt.find(take_card));
                else handa.erase(handa.find(take_card));
                table.insert(take_card);

                if (!opponent_win) {
                    now_hand.insert(card);
                    table.erase(table.find(card));
                    return memo[state] = true;
                }
            }

            now_hand.insert(card);
            table.erase(table.find(card));
        }
        return memo[state] = false;
    };

    bool ans = dfs(dfs, handt, handa, table, 0);
    cout << (ans ? "Takahashi" : "Aoki") << endl;
    return 0;
}
