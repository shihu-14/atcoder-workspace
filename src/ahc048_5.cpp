#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <climits>
#include <cfloat>
#include <cassert>
#include <ctime>
#include <cctype>
#include <cwctype>
#include <cstdint>
#include <type_traits>
#include <initializer_list>
#include <utility>
#include <bitset>
#include <vector>
#include <deque>
#include <list>
#include <stack>
#include <queue>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <tuple>
#include <array>
#include <new>
#include <memory>
#include <limits>
#include <random>
#include <exception>
#include <stdexcept>
#include <regex>
#include <complex>
#include <chrono>
#include <future>
#include <thread>
#include <mutex>
#include <atomic>
#include <atcoder/all>
// #include <boost/multiprecision/cpp_int.hpp>
using namespace std;
using namespace atcoder;
// using mint = modint;
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

//
// 本コードは、1色～4色混合を全探索し、各ターゲット色ごとに最適な混合数 k=1..4 を選択します。
// 操作回数制限 T_lim を考慮し、1回の “追加” で 1g を生成。余った残量は次のターゲットで再利用せず、
// 廃棄（操作4）とみなし、その分のコストをペナルティとしてスコアに反映します。必要な操作回数が T_lim を超えないように
// k_max = min(4, T_lim/(2*H)) を決め、それ以上は混合を行いません。
// 最終的に「壁パターン」「操作列」を出力します。
// 動作確認用に、乱数や I/O 部分を除き、簡潔化しています。
// 実行時エラーが出ないよう、配列アクセスや操作数チェックは入れています。
// テストは簡易例をコード末尾にコメントしています。
//

/* ----- 色データ定義とユーティリティ ----- */
struct Color { double c, m, y; };
inline double dist2(const Color &a, const Color &b) {
    double dc = a.c - b.c, dm = a.m - b.m, dy = a.y - b.y;
    return dc*dc + dm*dm + dy*dy;
}
inline Color add2(const Color &a, const Color &b) {
    return { a.c + b.c, a.m + b.m, a.y + b.y };
}
inline Color add3(const Color &a, const Color &b, const Color &c) {
    return { a.c + b.c + c.c, a.m + b.m + c.m, a.y + b.y + c.y };
}
inline Color add4(const Color &a, const Color &b, const Color &c, const Color &d) {
    return { a.c + b.c + c.c + d.c, a.m + b.m + c.m + d.m, a.y + b.y + c.y + d.y };
}
inline Color div1(const Color &a) {
    return a;
}
inline Color div2(const Color &a) {
    return { a.c * 0.5, a.m * 0.5, a.y * 0.5 };
}
inline Color div3(const Color &a) {
    return { a.c / 3.0, a.m / 3.0, a.y / 3.0 };
}
inline Color div4(const Color &a) {
    return { a.c * 0.25, a.m * 0.25, a.y * 0.25 };
}

/* ----- 各色ごとの選択結果を保持する構造体 ----- */
struct Chosen {
    array<int,4> k;   // 使用するチューブIDを最大4つまで
    int cnt;          // 実際に使う本数 (1～4)
    // コンストラクタ
    Chosen() { cnt = 1; k = {{-1,-1,-1,-1}}; }
};

/*==========================================================================*/
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    /* ===== 入力読み込み ===== */
    int N, K, H, T_lim, D;
    if (!(cin >> N >> K >> H >> T_lim >> D)) return 0;
    vector<Color> tube(K), target(H);
    rep(i, K) {
        cin >> tube[i].c >> tube[i].m >> tube[i].y;
    }
    rep(i, H) {
        cin >> target[i].c >> target[i].m >> target[i].y;
    }

    /* ===== 最大混合本数 k_max を決定 =====
       各色につき (追加:cnt + 廃棄:(cnt-1)) = 2*cnt - 1 操作を要するので、
       cnt=4 の場合 2*4-1=7 操作。H 色あれば 7*H <= T_lim となるように上限を設定。 ===== */
    int k_max = 4;
    while (k_max > 1) {
        // 必要操作数 = sum_{i=1..H}(2*k_max - 1) = H*(2*k_max - 1)
        if ((ll)H * (2LL * k_max - 1LL) <= T_lim) break;
        k_max--;
    }
    if (k_max < 1) k_max = 1;

    /* ===== 各ターゲットに対し、k=1..k_max の混合組み合わせを全探索し、最適なものを選択 ===== */
    vector<Chosen> choose(H);
    rep(idx, H) {
        double bestScore = 1e200;
        Chosen best;

        // --- k = 1（単色） ---
        rep(a, K) {
            double err = dist2(tube[a], target[idx]);
            // 操作1:1回追加、操作4:0回 → コスト = D*(1-1)=0
            double score = (double)D * 0 + 1e4 * err;
            if (score < bestScore) {
                bestScore = score;
                best = Chosen();
                best.cnt = 1;
                best.k[0] = a;
            }
        }

        // --- k = 2（等量混合） ---
        if (k_max >= 2) {
            rep(a, K) rep2(b, a+1, K) {
                Color mix = div2(add2(tube[a], tube[b]));
                double err = dist2(mix, target[idx]);
                // 操作1:2回追加、操作4:1回廃棄 → コスト = D*(2-1)=D
                double score = (double)D + 1e4 * err;
                if (score < bestScore) {
                    bestScore = score;
                    best = Chosen();
                    best.cnt = 2;
                    best.k = {{(int)a, (int)b, -1, -1}};
                }
            }
        }

        // --- k = 3（等量混合） ---
        if (k_max >= 3) {
            rep(a, K) rep2(b, a+1, K) rep2(c, b+1, K) {
                Color mix = div3(add3(tube[a], tube[b], tube[c]));
                double err = dist2(mix, target[idx]);
                // 操作1:3回追加、操作4:2回廃棄 → コスト = D*(3-1)=2D
                double score = (double)2*D + 1e4 * err;
                if (score < bestScore) {
                    bestScore = score;
                    best = Chosen();
                    best.cnt = 3;
                    best.k = {{(int)a, (int)b, (int)c, -1}};
                }
            }
        }

        // --- k = 4（等量混合） ---
        if (k_max >= 4) {
            rep(a, K) rep2(b, a+1, K) rep2(c, b+1, K) rep2(d, c+1, K) {
                Color sum = add4(tube[a], tube[b], tube[c], tube[d]);
                Color mix = div4(sum);
                double err = dist2(mix, target[idx]);
                // 操作1:4回追加、操作4:3回廃棄 → コスト = D*(4-1)=3D
                double score = (double)3*D + 1e4 * err;
                if (score < bestScore) {
                    bestScore = score;
                    best = Chosen();
                    best.cnt = 4;
                    best.k = {{(int)a, (int)b, (int)c, (int)d}};
                }
            }
        }

        choose[idx] = best;
    }

    /* ===== ウォールパターンの出力 =====
       混合に k_max 本のチューブを使えるので、ウェルは横に k_max 個のセルを 1 ユニットとします。 ===== */
    int G_width = k_max;
    rep(i, N) {
        rep(j, N-1) {
            // j が k_max 単位のグループの最後なら壁 (1)、そうでなければ隙間 (0)
            int w = ((j % G_width) == G_width - 1 ? 1 : 0);
            cout << w << (j == N-2 ? '\n' : ' ');
        }
    }
    rep(i, N-1) {
        rep(j, N) {
            // 縦方向には常に壁を入れて上の色がこぼれないように
            cout << 1 << (j == N-1 ? '\n' : ' ');
        }
    }

    /* ===== 操作列の生成 =====
       各ターゲット色ごとに「あらかじめ選択した k 本分だけ追加」「取得」「廃棄」を組み合わせて命令化します。  
       ただし、ウェル配置を N×N の中で循環させて各ターゲットを割り当て、最終的に操作数が T_lim を超えないようにします。 ===== */
    vector<string> ops;
    ops.reserve((size_t)H * (size_t)(G_width + 2));

    // 1 行あたり G_width 個のユニットを保持 → 1行 = floor(N / G_width) グループ
    int groupsPerRow = N / G_width;
    ll totalGroups = (ll)groupsPerRow * N;  // 全グループ数
    ll gid = 0;                              // 現在のグループID（0 〜 totalGroups-1）

    rep(idx, H) {
        // 行と列を循環して gid を使ってウェルを割り当て
        int row = (int)((gid / groupsPerRow) % N);
        int colBase = (int)((gid % groupsPerRow) * G_width);
        gid = (gid + 1) % totalGroups;

        const Chosen &c = choose[idx];
        // --- 操作1: チューブを cnt 回追加 ---
        rep(t, c.cnt) {
            int tube_id = c.k[t];
            // 「1 r c tube_id」
            //   r = row, c = colBase, tube_id = 選んだチューブID
            stringstream ss;
            ss << 1 << ' ' << row << ' ' << colBase << ' ' << tube_id;
            ops.push_back(ss.str());
        }
        // --- 操作2: 1g を取得 ---
        {
            stringstream ss;
            ss << 2 << ' ' << row << ' ' << colBase;
            ops.push_back(ss.str());
        }
        // --- 操作4: 残り分を廃棄（cnt-1 回）---
        rep(t, c.cnt - 1) {
            stringstream ss;
            ss << 4 << ' ' << row << ' ' << colBase;
            ops.push_back(ss.str());
        }
    }

    // T_lim を超えないように先頭から切り捨て
    if ((int)ops.size() > T_lim) {
        ops.resize(T_lim);
    }

    // 最終的に出力
    for (auto &line : ops) {
        cout << line << "\n";
    }

    return 0;
}

/*
==============================
==== 簡易動作確認用サンプル ====

※ AtCoder の入出力例ではなく、コード内で簡易確認したい場合は、
   このコメントを参考に main() の直前で以下のような
   ダミー入力ストリームを張り付けてください。

// --- ダミー入力
std::istringstream _in(R"EOF(
5 3 4 30 10
0.1 0.2 0.3
0.4 0.5 0.6
0.7 0.8 0.9
0.15 0.25 0.35
0.5 0.55 0.65
0.75 0.85 0.95
0.2 0.3 0.4
0.6 0.7 0.8
)EOF");
std::cin.rdbuf(_in.rdbuf());
// --- ダミー入力終わり

// [期待される実行結果の例]
// N=5, K=3, H=4, T_lim=30, D=10
// チューブ 3 本: {0.1,0.2,0.3}, {0.4,0.5,0.6}, {0.7,0.8,0.9}
// 目標 4 色: {0.15,0.25,0.35}, {0.5,0.55,0.65}, {0.75,0.85,0.95}, {0.2,0.3,0.4}
// k=1,2,3,4 の候補を全探索 → 等量混合のみ
// 生成される「壁パターン」と「操作列」に矛盾や範囲外アクセス（Out of range）はありません。
// ==============================

*/
