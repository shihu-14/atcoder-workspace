#include<bits/stdc++.h>
using namespace std;

#define REP(i,a,b) for(i=a;i<b;i++)
#define rep(i,n) REP(i,0,n)

#define ll long long
#define ull unsigned ll

#define INF 1000000000

int Q;
int seed, N, K, B;
int X[100000];
int Y[100000];

int p[20], pn[20], ps;

int main(){
  int i, j, k;
  int res;
  
  scanf("%d",&Q);
  while(Q--){
    scanf("%d%d%d%d",&seed,&N,&K,&B);
    N++;
    
    X[0] = seed;
    REP(i,1,N) X[i] = 1 + (((ll)X[i-1]*X[i-1] + (ll)X[i-1]*12345) % 100000009);
    if (Q == 3) rep(i, N+1) cout << X[i] << " ";
    cout << endl;
    ps = 0;
    for(i=2;i<=B;i++){
      if(B%i==0){
        p[ps] = i;
        pn[ps] = 0;
        while(B%i==0) pn[ps]++, B/=i;
        ps++;
      }
    }

    res = INF;
    rep(k,ps){
      rep(i,N){
        Y[i] = 0;
        j = X[i];
        while(j%p[k]==0) j/=p[k], Y[i]++;
      }
      sort(Y, Y+N);
      if (Q == 3) rep(i, N+1) cout << Y[i] << " ";
      cout << endl;
      j = 0;
      rep(i,K) j += Y[i];
      res = min(res, j/pn[k]);
    }

    printf("%d\n",res);
  }

  return 0;
}