int solution(vector<int> &A) {
    // Implement your solution here
    int n = A.size();
    sort(A.begin(), A.end());
    long long ans = 0;
    for (int i = 0; i < n; i++)
    {
        ans += abs(A[i]-i-1);
    }
    return ans;
}