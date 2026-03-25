// n以下の素因数分解/約数列挙
struct Eratosthenes
{
    int N;
    vector<int> min_pf, prime; // min_pf[i]: iの最小の素因数, prime: 素数リスト
    // コンストラクタにおいて、ふるいを回す。-> min_pfを埋め、N以下の素数をprimeに列挙。O(Nlog(logN))
    Eratosthenes(int n) : N(n), min_pf(n+1, -1)
    {
        for(int i = 2; i <= n; i++)
        {
            if(min_pf[i] != -1) continue;
            prime.emplace_back(i);
            for(int j = i; j <= n; j+=i)
                if (min_pf[j] == -1) min_pf[j] = i;
        }
    }
    // N以下の自然数xの素因数分解を返す関数。p^a -> {p, a}, O(logx)
    vector<pair<int, int>> factorize(int x)
    {
        assert(x <= N);
        vector<pair<int,int>> res;
        while (x > 1)
        {
            int p = min_pf[x];
            int exp = 0;
            while (min_pf[x] == p) x/=p, exp++;
            res.emplace_back(p, exp);
        }
        return res;
    }  
    bool isPrime(int x){return min_pf[x] == x;}
    // N以下のxの約数を列挙する。O(xの約数の個数)
    vector<int> divisor(int x)
    {
        assert(x <= N);
        vector<int> res({1});
        auto pf = factorize(x); // O(logx)
        for (auto p : pf)
        {
            int s = (int)res.size();
            for (int i = 0; i < s; ++i)
            {
                int v = 1;
                for (int j = 0; j < p.second; ++j)
                {
                    v *= p.first;
                    res.push_back(res[i] * v);
                }
            }
        }
        sort(res.begin(), res.end());
        return res;
    }
};
// 任意の整数xの素因数分解を返す関数。p^a -> {p, a}, O(sqrt(x))
vector<pair<ll, int>> factorize(ll x)
{
    vector<pair<ll, int>> res;
    for (ll i = 2; i*i <= x; i++)
    {
        if (x%i) continue;
        int exp = 0;
        while (x%i == 0) x/=i, exp++;
        res.emplace_back(i, exp);
    }
    if (x != 1) res.emplace_back(x, 1);
    return res;
}
// 任意の整数xの約数を列挙する。O(sqrt(x))
vector<ll> divisor(ll x)
{
    vector<ll> lower, upper;
    for (ll i = 1; i*i <= x; i++)
    {
        if (x%i) continue;
        lower.emplace_back(i);
        if (i*i != x) upper.emplace_back(x/i);
    }
    lower.insert(lower.end(), upper.rbegin(), upper.rend());
    return lower;
}
