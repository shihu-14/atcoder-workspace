// https://codeforces.com/blog/entry/62393 から引用
struct custom_hash {
    // splitmix64 (64ビット乱数生成)
    static uint64_t splitmix64(uint64_t x) {
        x += 0x9e3779b97f4a7c15ULL;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
        x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
        return x ^ (x >> 31);
    }
    // ハッシュ本体
    size_t operator()(uint64_t x) const {
        static const uint64_t FIXED_RANDOM = chrono::steady_clock::now().time_since_epoch().count();
        return splitmix64(x + FIXED_RANDOM);
    }
} xor_rng64;

// 値を64bitの乱数に変換する
struct Random {
    unordered_map<uint64_t, uint64_t> memo;
    std::mt19937_64 engine;
    Random() : engine(std::chrono::steady_clock::now().time_since_epoch().count()) {}
    uint64_t operator()(uint64_t x) {
        if (memo.count(x)) return memo[x];
        std::uniform_int_distribution<uint64_t> dist(0, (1ULL<<63)-1);
        return memo[x] = dist(engine);
    }
} rng64;

/**
 * SetHash 構造体
 * - 集合に対してハッシュを構築
 * - 区間[l, r)のハッシュ値を O(1) で取得できる（排他的論理和(^)ベース）
 */
struct SetHash {
    int n;
    vector<uint64_t> hashSum;
    map<uint64_t, bool> mp;
    SetHash(const vector<uint64_t> &a): n(a.size()), hashSum(n+1) {
        for (int i = 0; i < n; i++){
            uint64_t h = xor_rng64(a[i]);
            if (mp.count(h)) h = 0;
            mp[h] = true;
            hashSum[i+1] = hashSum[i]^h;
        }
    }
    uint64_t getHash(int l, int r) const { // [l, r)のハッシュ値
        if(l < 0 || r > n || l > r) return 0;
        return hashSum[r]^hashSum[l];
    }
};

/**
 * MultiSetHash 構造体
 * - 多重集合に対してハッシュを構築
 * - 区間[l, r)のハッシュ値を O(1) で取得できる（加算(+)ベース）
 */
struct MultiSetHash {
    static const uint64_t P = (1LL<<61)-1; // big prime
    int n;
    vector<uint64_t> hashSum;
    MultiSetHash(const vector<uint64_t>& a): n(a.size()), hashSum(n+1) {
        for (int i = 0; i < n; i++) {
            uint64_t h = rng64(a[i]);
            hashSum[i+1] = (hashSum[i]+h)%P;
        }
    }
    uint64_t getHash(int l, int r) const { // [l, r)のハッシュ値
        if(l < 0 || r > n || l > r) return 0;
        return (hashSum[r]-hashSum[l]+P)%P;
    }
};
