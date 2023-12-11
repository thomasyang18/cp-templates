// Test d2d on https://codeforces.com/contest/893/problem/F
#pragma GCC target("avx2")
#pragma GCC optimize("O3")
#pragma GCC optimize("unroll-loops")
#include <bits/stdc++.h>
using namespace std;

typedef int ll;
typedef long double ld;
typedef pair<ll, ll> pl;
typedef vector<ll> vl;

#define G(x) ll x; cin >> x;
#define F(i, l, r) for(ll i = l; i < (r); ++i)
#define FD(i, r, l) for(ll i = r; i > (l); --i)
#define A(a) (a).begin(), (a).end()
#define N 100010
ll tin[N], tout[N], dep[N];
vl adj[N];

void dfs(ll i, ll p) {
    dep[i] = dep[p]+1;
    static int time = 0;
    tin[i] = time++;
    for (ll x: adj[i]) if (x-p) dfs(x, i);
    tout[i] = time;
}

namespace d2d {
    using key_t = int;
    using T = int;
    
    T id = INT_MAX;
    T f(T a, T b) {return min(a,b);}
    
    struct seg {
        vector<T> t;
        vector<key_t> rr;
        #define rein(x) (lower_bound(A(rr), x) - rr.begin() + rr.size())
        void modify(key_t pin, T value) {  // set value at position p
            int p = rein(pin);
            // assert(p != 2 * rr.size() and rr[p - rr.size()] == pin);
            for (t[p] = f(t[p], value); p /= 2;) t[p] = f(t[2*p], t[2*p+1]);
        }
    
        T query(key_t lin, key_t rin) { // fold f on interval [l, r)
            T resl=id, resr=id;
            for (int l = rein(lin), r = rein(rin); l < r; l /= 2, r /= 2) {
                if (l&1) resl = f(resl, t[l++]);
                if (r&1) resr = f(t[--r], resr);
            }
            return f(resl, resr);
        }
        #undef rein
    } t[2*N];

// setup functions
    void build() { 
        F(i, 0, N*2) {
            auto &rr = t[i].rr;
            sort(A(rr));
            rr.erase(unique(A(rr)), rr.end());
            t[i].t.assign(2*rr.size(), id);
        }
    }

    void pre_modify(int p, key_t k) {
        for (p+=N, t[p].rr.push_back(k); p /= 2;) t[p].rr.push_back(k);
    }

// actual functions
    void modify(int p, key_t k, int v) { 
        for (p+=N, t[p].modify(k, v); p /= 2;) t[p].modify(k, v);
    }

    T query(int l, int r, key_t l2, key_t r2) { // queries f(..[l2, r2)..) for all segtrees [l, r)
        T res = id;
        for(l += N, r += N; l < r; l /= 2, r /= 2) {
            if(l & 1) res = f(t[l++].query(l2, r2), res);
            if(r & 1) res = f(res, t[--r].query(l2, r2));
        }
        return res;
    }
}

int main(){
//    freopen("a.in", "r", stdin);
//    freopen("a.out", "w", stdout);

    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(20);

    G(n) 
    {
        G(r)
        vl a(n+1);
        F(i, 1, n+1) cin >> a[i];
        F(i, 0, n-1) {
            G(x) G(y) adj[x].push_back(y); adj[y].push_back(x);
        }    
        dfs(r, r);
        F(i, 1, n+1) d2d::pre_modify(tin[i], dep[i]);
        d2d::build();
        F(i, 1, n+1) d2d::modify(tin[i], dep[i], a[i]);
    }
    
    ll last = 0;
    G(m) while(m--) {
        G(p) G(q)
        ll x = (p + last)%n + 1;
        ll k = (q + last)%n;
        last = d2d::query(tin[x], tout[x], dep[x], dep[x] + k + 1);
        cout << last << '\n';
    }
    
}
