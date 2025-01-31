#include "bits/stdc++.h"
using namespace std;
#define MAX 1000000

long long dp[MAX];
long long dp2[MAX];

int main() {
	long long mod = (long long)1e9 + 9;
	int T;
	cin >> T;

	dp[0] = 1;
	for (int i = 1; i <= 9; i++)
	{
		for (int j = 0; j < MAX - i; j++)
		{
			dp[i + j] += dp[j];
			if (dp[i + j] >= mod) dp[i + j] -= mod;
		}
	}
	dp2[0] = 1;
	for (int i = 1; i < MAX; i++)
	{
		dp2[i] = dp2[i - 1] + dp[i];
		if (dp2[i] >= mod) dp2[i] -= mod;
	}

	for (int i = 0; i < T; i++)
	{
		long long M;
		cin >> M;
		M /= 111111;
		cout << dp2[(int)M] << endl;
	}
    for(int i = 0; i < 20; i++) cout << dp[i] << " ";
    cout << endl;
}