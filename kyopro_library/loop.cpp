
auto next_func = [&](ll s)
{
    // Edit here: 状態sから次の状態を返す処理を書く
    return 
};

// Edit here: vectorなどのデータ構造の変更や適宜初期化を行う.
int cur = 0; // 初期状態
ll exp = -1; // 繰り返し回数
vector<ll> counts;      // ID->出現回数 
vector<int> history_S;         // ID->状態
vector<int> first_seen;      // 状態->ID

int start_idx = -1, cycle_len = -1;
// main loop
for (ll k = 0; k <= exp; ++k)
{
    if (first_seen[cur] != -1)
    {
        start_idx = first_seen[cur];
        cycle_len = (int)history_S.size() - start_idx;

        ll rem = (exp + 1) - k; // 残りの個数
        ll q = rem / cycle_len; // 周回数
        ll r = rem % cycle_len; // 余り

        // ループ部分のカウントを一括更新
        for (int i = start_idx; i < (int)history_S.size(); i++)
        {
            counts[i] += q;
            if (i - start_idx < r) counts[i]++;
        }
        break;
    }

    // Edit here: 状態curの処理を書く
    first_seen[cur] = (int)history_S.size();
    counts[history_S.size()]++;
    history_S.emplace_back(cur);

    cur = next_func(cur);
}

auto get_state = [&](ll i)
{
    if (i < (int)history_S.size()) return history_S[i];
    // 周期がある場合
    ll offset = (i - start_idx) % cycle_len;
    return history_S[start_idx + offset];
};
