// #pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
using mint = modint998244353;
// using mint = modint1000000007;
// using namespace boost::multiprecision;
using uint = unsigned int;
using ll = long long;
using ull = unsigned long long;
using ld = long double;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
using T3 = tuple<int, int, int>;
using G = vector<vector<int>>;
#define rep(i, n) for (ll i = 0; i < (n); ++i)
#define rep2(i, a, b) for (ll i = a; i < (b); ++i)
#define rrep2(i, a, b) for (ll i = a-1; i >= (b); --i)
#define rep3(i, a, b, c) for (ll i = a; i < (b); i+=c)
#define rng(a) a.begin(),a.end()
#define rrng(a) a.rbegin(),a.rend()
#define popcount __builtin_popcount
#define popcount_ll __builtin_popcountll
#define fi first
#define se second
#define UNIQUE(v) sort(rng(v)), v.erase(unique(rng(v)), v.end())
#define MIN(v) *min_element(rng(v))
#define MAX(v) *max_element(rng(v))
#define SUM(v) accumulate(rng(v),0LL)
#define IN(v, x) (find(rng(v),x) != v.end())
template<class T> bool chmin(T &a,T b){if(a>b){a=b;return 1;}else return 0;}
template<class T> bool chmax(T &a,T b){if(a<b){a=b;return 1;}else return 0;}
template<class T> void printv(vector<T> &v){rep(i,v.size())cout<<v[i]<<" \n"[i==v.size()-1];}
template<class T> void printvv(vector<vector<T>> &v){rep(i,v.size())rep(j,v[i].size())cout<<v[i][j]<<" \n"[j==v[i].size()-1];cout<<endl;}
const ll dx[] = {-1, 0, 1, 0};
const ll dy[] = {0, 1, 0, -1};
const ll dxx[] = {-1, -1, 0, 1, 1, 1, 0, -1};
const ll dyy[] = {0, 1, 1, 1, 0, -1, -1, -1};
const ll LINF = 3001002003004005006ll;
const int INF = 1001001001;
int rand(){static random_device rd; static mt19937 mt(rd()); static uniform_int_distribution<int> dist(0, INF); return dist(mt);}

vector<double> computeSteadyState(
    int N,
    const vector<int> &A,
    const vector<int> &B,
    int maxIter = 300,
    double eps = 1e-14
)
{
    // 1. 確率遷移行列 M を作る (サイズ N×N, 各行に2つだけ 1/2)
    //    M[i][j] = 1/2 if j==A[i] or j==B[i], otherwise 0
    //      ただし i==A[i]==B[i] のような場合、 M[i][A[i]] = 1.0 になる。
    //    → 以下では2つに加算する形で実装
    vector<vector<double>> M(N, vector<double>(N, 0.0));
    for(int i = 0; i < N; i++){
        M[i][A[i]] += 0.5;
        M[i][B[i]] += 0.5;
    }

    // 2. パワーイテレーション初期値 p(0) は、一様分布などでOK (ここでは一様に)
    vector<double> p(N, 1.0 / N);

    // 3. 反復 (p <- p*M) を maxIter 回以内で収束させる
    for(int iter = 0; iter < maxIter; iter++){
        vector<double> newp(N, 0.0);

        // p * M の計算
        for(int i = 0; i < N; i++){
            double pi = p[i];
            // 行 i から列 j への確率 M[i][j]
            // 全jについて newp[j] += p[i]*M[i][j]
            for(int j = 0; j < N; j++){
                newp[j] += pi * M[i][j];
            }
        }

        // 収束判定 (L1ノルムでの変化量など)
        double diff = 0.0;
        for(int j = 0; j < N; j++){
            diff += fabs(newp[j] - p[j]);
        }

        p = newp;
        if(diff < eps) { // 十分小さくなったら終了
            break;
        }
    }

    return p;
}

G make_ans(vector<bool> &iszero, vector<int>&freq, int L){
    int n = freq.size();
    vector<double> p(n);
    rep(i, n) p[i] = (double)freq[i]/L;
    vector<int> que;
    rep(i, n) if (iszero[i] == false) que.emplace_back(i);
    int D = n*2;
    rep(i, n) p[i] *= D;
    random_device rd; 
    mt19937 mt(rd());
    vector<int> cnt(n);
    vector<vector<int>> ans(2, vector<int>(n));
    bool init = false;
    rep(i, n){
        rep(j, 2){
            if (que.empty()){
                rep(v, n) if (iszero[v] == false) que.emplace_back(v);
            }
            uniform_int_distribution<int> dist(0, que.size()-1);
            int id = dist(mt);
            int to = que[id];
            cnt[to]++;
            ans[j][i] = to;
            if (p[to] <= cnt[to]){
                que.erase(find(que.begin(), que.end(), to));
            }
        }
    }
    return ans;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    auto start = std::chrono::high_resolution_clock::now();
    int n, L; 
    cin >> n >> L;
    vector<bool> iszero(n);
    vector<int> freq(n);
    rep(i, n){
        cin >> freq[i];
        if (freq[i] == 0) iszero[i] = true;
    }
    G final_ans;
    double min_score = LINF;
    int times = 0,  update = 0;
    while(1){
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        if (elapsed.count() > 1900) break;
        times++;
        auto tmp_ans = make_ans(iszero, freq, L);
        auto p = computeSteadyState(n, tmp_ans[0], tmp_ans[1]);
        double score = 0;
        rep(i, n) score += abs(p[i] - (double)freq[i]/L);
        if (min_score > score){
            update++;
            min_score = score;
            final_ans = tmp_ans;
        }
    }    
    // cerr << 1000000-min_score << endl;
    cerr << times << " " << update << endl;
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        cerr << elapsed.count() << endl;
    }
    rep(i, n){
        cout << final_ans[0][i] << " " << final_ans[1][i] << endl;
    }
    return 0;
}