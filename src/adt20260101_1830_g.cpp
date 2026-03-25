#include <iostream>
#include <vector>
#include <bitset>
#include <cassert>
#include <algorithm>
#include <random>
#include <numeric>

using namespace std;
#define rep(i, n) for (int i = 0; i < (n); ++i)

const int MAX_N = 1005;

struct bit_Matrix {
    using BS = bitset<MAX_N>;
    int h, w;
    vector<BS> d;
    
    bit_Matrix(int h, int w): h(h), w(w), d(h) {}
    
    BS operator[](int i) const {return d[i];}
    BS& operator[](int i) {return d[i];}

    // 連立方程式を解く関数（ここは変更なし）
    BS solve() {
        int ni = 0;
        rep(j, w-1) {
            for (int i = ni; i < h; i++) {
                if (d[i][j]) {swap(d[i], d[ni]); break;}
            }
            if (!d[ni][j]) continue;
            rep(i, h) if (ni != i && d[i][j]) d[i] ^= d[ni];
            ni++; if (ni == h) break;
        }
        BS nan, res; nan[w-1] = 1;
        rep(i, h) {
            int j = 0;
            while (j < w-1 && !d[i][j]) j++;
            if (j == w-1) {
                if (d[i][w-1]) return nan;
                continue;
            }
            res[j] = d[i][w-1];
        }
        return res;
    }
};

void solve()
{
    int n, k; cin >> n >> k;
    
    // 乱数生成器
    mt19937 mt(1333); 
    vector<int> p(n);
    iota(p.begin(), p.end(), 0);

    // 最終的に解くための行列
    bit_Matrix mat(n, n + 1);
    
    // 質問内容を保存しておくリスト
    vector<vector<int>> queries(n);

    // 線形独立性のチェック用（基底ベクトル）
    // basis[j] には、「一番左の立っているビットが j であるベクトル」を格納
    vector<bitset<MAX_N>> basis(n);
    vector<bool> used_basis(n, false);

    rep(i, n) {
        while (true) {
            // 1. ランダムな質問候補を作る
            shuffle(p.begin(), p.end(), mt);
            bitset<MAX_N> candidate;
            vector<int> current_indices;
            
            rep(j, k) {
                candidate[p[j]] = 1;
                current_indices.push_back(p[j]);
            }

            // 2. 既存の基底を使って掃き出しを行う（独立性チェック）
            bitset<MAX_N> temp = candidate;
            
            // ビットが立っている場所を探してXORしていく
            // ※ N=1000程度ならこのループで十分高速です
            for (int j = 0; j < n; ++j) {
                if (temp[j]) { // ビット j が立っている
                    if (used_basis[j]) {
                        // 既に基底があるなら消す
                        temp ^= basis[j];
                    } else {
                        // 新しい基底を発見！ -> この候補は採用できる
                        basis[j] = temp;
                        used_basis[j] = true;
                        
                        // 行列とクエリリストに登録
                        mat[i] = candidate; // ここには掃き出し前の元のベクトルを入れる
                        queries[i] = current_indices;
                        goto next_row; // 次の行（i+1）の作成へ
                    }
                }
            }

            // ここに来た = tempが全て0になった = 線形従属だった
            // -> whileループの先頭に戻って作り直し
        }
        next_row:;
    }

    // 全ての質問が決まったら、実際に質問を投げる
    rep(i, n) {
        cout << "?";
        for (int idx : queries[i]) {
            cout << " " << idx + 1;
        }
        cout << endl;

        int T; cin >> T;
        if (T == -1) return;
        mat[i][n] = T; // 答えを行列の右端にセット
    }

    // 解く
    auto ans_bits = mat.solve();

    cout << "!";
    rep(i, n) {
        cout << " " << ans_bits[i];
    }
    cout << endl;
}

int main()
{
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    solve();
    return 0;
}