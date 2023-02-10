//Least Common Ancestor
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef long long int ll;
typedef pair<ll, ll> pl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define FD(i, r, l) for(ll i = r; i > (l); --i)
#define N 100010
#define L 20

namespace blift{
    ll dep[N], par[N][L];
    vector<ll> tree[N];

    // Supports associative queries from u to v. Faster than HLD, and our HLD template atm does not support associativity afaik.
    typedef ll T;
    T id = 0;
    T valUp[N][L], valDown[N][L];
    T f_up(T l, T r){
        return l+r;
    }
    T f_down(T l, T r){
        return l+r;
    }

    void dfs(ll i, ll p) {
        dep[i] = dep[p] + 1;
        par[i][0] = p;
        F(l, 1, L) {
            auto half = par[i][l - 1];
            par[i][l] = par[half][l - 1];
            valUp[i][l] = f_up(valUp[i][l - 1], valUp[half][l - 1]);
            valDown[i][l] = f_down(valUp[i][l - 1], valDown[half][l - 1]);
        }
        for(ll j : tree[i]) if(j - p) dfs(j, i);
    }

    ll lca(ll a, ll b) {
        if(dep[a] < dep[b]) swap(a, b);
        FD(l, L - 1, -1) if((dep[a] - dep[b]) >> l) a = par[a][l];
        if(a == b) return a;
        FD(l, L - 1, -1) if(par[a][l] - par[b][l]) a = par[a][l], b = par[b][l];
        return par[a][0];
    }

    // two seperate functions, because if the operation is associative, direction matters (can't just swap with a,b). prolly easy fix tho
    ll query(ll u, ll v){
        ll com = lca(u, v);
        // query up
        T queryUp = id, queryDown = id;
        
        // query down

        // Blifting only works on a half open interval, so manually append the last element
        //T ans = f(queryUp, )
    }
}



int main() {
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    G(n)
    F(i, 0, n - 1) {
        G(u) G(v)
        tree[u].push_back(v);
        tree[v].push_back(u);
    }
    dfs(1, 1);
}