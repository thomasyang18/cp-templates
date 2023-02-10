#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")

#include<bits/stdc++.h>
#include<math.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;

#define K first
#define V second
#define G(x) ll x; cin >> x;
#define GD(x) ld x; cin >> x;
#define GS(s) string s; cin >> s;
#define EX(x) { cout << x << '\n'; exit(0); }
#define A(a) (a).begin(), (a).end()


// Debugs be like
// This has all containers and pair
template<class T, class S>
ostream& operator<<(ostream &o, pair<T, S> p) {
    return o<<'('<<p.first<<", "<<p.second<<')';
}

template<template<class, class...> class T, class... A>
typename enable_if<!is_same<T<A...>, string>(), ostream&>::type
operator<<(ostream &o, T<A...> V) {
	o<<'[';
	for(auto a:V) o<<a<<", ";
	return o<<']';
}

// This is tuples, _p is helper
template<ll i, class... T>
typename enable_if<i==sizeof...(T)>::type _p(ostream& o, tuple<T...> t) {}

template<ll i, class... T>
typename enable_if<i<sizeof...(T)>::type _p(ostream& o, tuple<T...> t) {
    _p<i+1>(o << get<i>(t)<< ", ", t);
}
 
template<class... T>
ostream& operator<<(ostream& o, tuple<T...> t) {
    _p<0>(o<<'(', t);
    return o<<')';
}

#define NN
#define M 1000000007 // 998244353

ld EPS = 1e-9;
vector<int> where;
int solve_system(vector<vector<ld>> &a, vector<ld> &b){
    int n = a.size();
    int m = a[0].size();
    for (int i = 0; i < n; ++i) a[i].push_back(b[i]);

    where.assign(m, -1);
    for (int col = 0, row = 0; col < m && row < n; ++col){
        int sel = row;
        for (int i = row; i < n; ++i) if (abs(a[i][col]) > abs(a[sel][col])) sel = i;
        if (abs(a[sel][col]) < EPS) continue;
        for (int i = col; i <= m; ++i) swap(a[sel][i], a[row][i]);
        where[col] = row;

        for (int i = 0; i < n; ++i) if (i != row){
            ld c = a[i][col] / a[row][col];
            for (int j = 0; j <= m; ++j) a[i][j] -= a[row][j]*c;
        } 

        ++row;
    }

    b.assign(m, 0);
    for (int i = 0; i < m; ++i) if (where[i] != -1) b[i] = a[where[i]][m] / a[where[i]][i];

    for (int i = 0; i < n; ++i) {
        ld sum = 0;
        for (int j = 0; j < m; ++j) sum += b[j] * a[i][j];
        if (abs(sum - a[i][m]) > EPS) return 0;
    }

    for (int i = 0; i < m; ++i) if (where[i] == -1) return 2;

    return 1;
}

int main(){
//    freopen("a.in", "r", stdin);
//    freopen("a.out", "w", stdout);

    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(4);

    while (1){
        G(n)
        if (n == 0) return 0;
        vector<vector<ld>> a;
        vector<ld> b(n);
        for (int i = 0; i < n; i++){
            a.push_back(vector<ld>(n));
            for (auto &x: a[i]) cin >>x;
        }
        for (auto &x: b) cin >> x;
        int ans = solve_system(a, b);
        if (ans >= 2) cout << "multiple\n";
        else if (ans == 0) cout << "inconsistent\n";
        else {
            for (auto v: b) cout << v << " ";
            cout << '\n';
        }
    }
    
}

