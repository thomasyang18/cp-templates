#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")

#include<bits/stdc++.h>
#include<math.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;
typedef vector<ll> vl;
#define FD(i, r, l) for(ll i = r; i > (l); --i)

#define K first
#define V second
#define G(x) ll x; cin >> x;
#define GD(x) ld x; cin >> x;
#define GS(s) string s; cin >> s;
#define EX(x) { cout << x << '\n'; exit(0); }
#define A(a) (a).begin(), (a).end()
#define F(i, l, r) for (ll i = l; i < (r); ++i)

#define NN
#define M 1000000007 // 998244353

// TODO: write a rollback_dsu implementation that actually works
class rollback_dsu {
    vector<pl> ops;
    ll components;
    vl p, sz;
public:
    rollback_dsu(ll n): components(2*n) {
        p.resize(n); 
        iota(A(p), 0ll);
        sz.assign(n, 1);
    } 
    ll find(ll i) {
        ops.emplace_back(~i, p[i]);
        while (p[i] != p[p[i]]) p[i] = p[p[i]];
        return p[i];
    }
    
    void merge(ll u, ll v) {
        u = find(u), v = find(v);
        if (u-v) {
            components-=2;
            ops.emplace_back(v, sz[v]); 
            ops.emplace_back(u, sz[u]);
            if (sz[u] < sz[v]) swap(u, v);
            p[v] = u;
            sz[u] += sz[v];
        }
    }

    ll get_rollback_time() const {
        return ops.size();
    }

    ll get_components() const {
        assert(components%2==0);
        return components/2;
    }

    void rollback(ll time) {
        assert(ops.size() >= time);
        while (ops.size() > time) {
            auto [idx, size] = ops.back(); ops.pop_back();
            if (idx < 0) p[~idx] = size; 
            else p[idx] = idx, sz[idx] = size, components++; 
        }
    }
};

struct queryseg {
    struct query {
        int v, u;
        query(int _v, int _u) : v(_v), u(_u) {}
    };

    vector<vector<query>> t;
    rollback_dsu dsu;
    int n;

    queryseg(int _T, int dsu_n) : dsu(dsu_n), n(_T) { 
        t.resize(2*n);
    }
    
    // Adds query to [l, r)
    void add_query(ll l, ll r, query q) {
        for (l += n, r += n; l < r; l >>= 1, r >>= 1) {
            if (l&1) t[l++].push_back(q);
            if (r&1) t[--r].push_back(q);
        }
    }

    void dfs(int v, vl& ans) {
        ll save = dsu.get_rollback_time();
        for (const auto& q : t[v]) dsu.merge(q.v, q.u);
        if (v >= n) {
            ans[v-n] = dsu.get_components();
        } else {
            dfs(v*2, ans); dfs(v*2+1, ans);
        }
        dsu.rollback(save); 
    }

    vl solve() {
        vl ans(n);
        dfs(1, ans);
        return ans;
    }

};

int main(){
//    freopen("connect.in", "r", stdin);
//    freopen("connect.out", "w", stdout);

    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(20);
    G(n) G(k)
    queryseg q(k+1, n);
    string cur;
    
    map<pl, ll> unmatched_queries; //edge, type 

    F(time, 0, k) {
        GS(s)
        cur += s;
        if (s == "?") continue;
        G(u) G(v) --u, --v;
        if (u<v) swap(u, v);
        if (s == "+") {
            unmatched_queries[{u, v}] = time;
        } else if (s == "-") {
            q.add_query(unmatched_queries[{u, v}], time+1, queryseg::query(u, v));
            unmatched_queries.erase({u, v});
        }
    }

    for (const auto &[edges, t]: unmatched_queries) {
        q.add_query(t, k, queryseg::query(edges.K, edges.V));
    }

    auto answers = q.solve();

    F(i, 0, cur.size()) if (cur[i] == '?') cout << answers[i] << '\n';

}

