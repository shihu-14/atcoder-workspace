#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
using namespace std;
const double INF = 1001001001;
int main(){
    // グラフを定義 (行き先の頂点, 辺の重み)
    map<int, vector<pair<int, double>>> g;
    g[0] = {{2, 1}, {5, 4}, {7, 2}};
    g[2] = {{0, 1}, {3, 7}, {5, 1}};
    g[3] = {{2, 7}, {5, 1.5}, {7, 6}};
    g[5] = {{0, 4}, {2, 1}, {3, 1.5}, {7, 5}};
    g[7] = {{0, 2}, {3, 6}, {5, 5}};

    // 最短距離を管理するためのデータ構造(始点は0で、それ以外はINFで初期化する)
    map<int, double> dist;
    for (auto [v, _]: g) dist[v] = INF; dist[0] = 0;
    // Dijkstra法で最短距離を求めるために優先度付きキューを利用する
    priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq; pq.emplace(0, 0);
    // 最短路を求めるために親を記録する
    map<int, int> par; par[0] = -1;
    // Dijkstra法
    while(!pq.empty()){
        auto [d, now] = pq.top(); pq.pop();
        if (dist[now] < d) continue;
        for (auto [to, w]: g[now]){
            if (dist[to] > dist[now]+w){
                dist[to] = dist[now]+w;
                pq.emplace(dist[to], to);
                par[to] = now;
            }
        }
    }
    // 最短路/最短距離の出力
    for (auto [v, _]: g) if (v != 0){
        vector<int> root; root.emplace_back(v);
        int u = v;
        // 親ノードを辿って最短路を復元する。
        while(par[u] != -1) u = par[u], root.emplace_back(u);
        reverse(root.begin(), root.end());
        cout << "0" << " -> " << v << endl;
        cout << "最短路: ";
        for (int i = 0; i < root.size()-1; i++){
            cout << root[i] << " -> "; 
        }
        cout << root.back() << endl;
        cout << "最短距離: " << dist[v] << endl << endl;;
    }
    return 0;
}