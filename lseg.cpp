// SEGTREE WORKS NOW! Verified by CSES

#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2")

#include <bits/stdc++.h>
using namespace std;
typedef long long int ll;

#define NN 200010
#define K first
#define V second

namespace lseg{
    typedef ll T;
    typedef ll D;

    T idT = -1e18, t[2*NN];
    D idD = 0, d[NN];
    ll n = (fill_n(d, NN, idD), NN);

    T f(T a, T b) {
        return max(a,b);
    }
    T g(T a, D b) { 
        return a+b;
    }
    D h(D a, D b) { 
        return a+b;
    }

    void apply(ll p, D v){
        t[p] = g(t[p], v);
        if (p < n) d[p] = h(d[p], v);
    }  

    void push(ll p){
        for (ll s=26; --s;) {
            ll i = p>>s;
            apply(2*i, d[i]);
            apply(2*i+1, d[i]);
            d[i]=idD;
        }
    }

    void modify(ll p, T v = idT){
        if (p < n) { push(p += n), t[p] = v;}
        while (p/=2) t[p] = g(f(t[2*p], t[2*p+1]), d[p]);
    }

    void modify(ll l, ll r, D v){
        ll l0 = (l+=n), r0 = (r+=n);
        push(l0), push(r0-1);
        for (; l < r; l/=2, r/=2) {
            if (l&1) apply(l++, v);
            if (r&1) apply(--r, v);
        }
        modify(l0), modify(r0 - 1);
    }

    T query(ll l, ll r){
        l += n, r += n;
        push(l), push(r-1);
        T resL = idT, resR = idT;
        for (; l < r; l/=2, r/=2) {
            if (l&1) resL = f(resL, t[l++]);
            if (r&1) resR = f(t[--r], resR);
        }
        return f(resL, resR);
    }
}

int main(){
    
}