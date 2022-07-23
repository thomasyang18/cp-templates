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

#define NN 200010
#define M 1000000007 // 998244353

ll n;
vector<ll> g[NN], rg[NN], adj[NN];
ll scc[NN];
vector<ll> scc_stack;

void scc_dfs1(ll i){
    if (scc[i]++!=0) return;
    for (int x: g[i]) scc_dfs1(x);
		scc_stack.push_back(i);
}

void scc_dfs2(int i, int v) {
    if (scc[i] != -1) return;
    scc[i]=v;
    for (int x: rg[i]) scc_dfs2(x,v);
}

void scc_solve() {
    // all zero indexed
    for (int i = 0; i < n; i++) scc[i] = 0;
    for (int i = 0; i < n; i++) scc_dfs1(i);
    for (int i = 0; i < n; i++) scc[i] = -1;
    int v = 0;
    while(scc_stack.size()) {
        int h = scc_stack.back();
        scc_stack.pop_back();
        if (scc[h]==-1) scc_dfs2(h,v++);
    }
    for (int i = 0; i < n; i++) {
        for (int x: g[i]) {
            if (scc[x] == scc[i]) continue;
            adj[scc[i]].push_back(scc[x]);
        }
    }
}

void add_edge(ll u, ll v){
    g[u].push_back(v);
    g[v].push_back(u);
}

int main(){
//    freopen("a.in", "r", stdin);
//    freopen("a.out", "w", stdout);

    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(20);
    
    cin >> n; G(m)
    while(m--){
        G(u) G(v) add_edge(--u,--v);
    }

    scc_solve();
    

}
