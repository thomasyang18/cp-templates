// Verified on https://judge.yosupo.jp/submission/162957
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

#define NN 500010
#define M 1000000007 // 998244353

vector<ll> g[2*NN], rev[2*NN];
ll scc[2*NN];
vector<ll> ord;
ll n;

ll ind(bool sat, int v) {return v + (sat ? 0:n);}

// DON'T CALL P! CALL ADD! ADD WILL ADD BOTH EDGES FOR YOU!
void p(bool bx, int x, bool by, int y) {
    g[ind(bx,x)].push_back(ind(by,y));
    rev[ind(by,y)].push_back(ind(bx,x));
}

// Adds a disjunction
void add(bool bx, int x, bool by, int y) {
    p(!bx,x,by,y);
    p(!by,y,bx,x);
}


void dfs2(int v, int c) {
    if (scc[v] > 0) return;
    scc[v] = c;
    for (int o: rev[v]) dfs2(o,c);
}
void dfs(int v) {
    if (scc[v]++ > 0) return;
    for (int o: g[v]) dfs(o);
    ord.push_back(v);
}

vector<ll> solve() { 
    for (int i = 0; i < n*2; i++) scc[i] = 0;
    for (int i = 0; i < n*2; i++) dfs(i); 
    for (int i = 0; i < n*2; i++) scc[i] = 0;
    while (ord.size()) {
        dfs2(ord.back(),1+2*n-ord.size());
        ord.pop_back();
    }

    vector<ll> res(n);
    for (int i = 0; i < n; i++) {
        if (scc[i] == scc[i+n]) return {};
        if (scc[i] > scc[i+n]) res[i] = false;
        else res[i] = true;
    } 
    return res;
}




int main(){
//    freopen("a.in", "r", stdin);
//    freopen("a.out", "w", stdout);

    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(20);
    
    GS(s) GS(s2) cin >> n; G(m)
    while (m--){
        G(a) G(b) G(c)
        add(a < 0, abs(a)-1, b < 0, abs(b) - 1);
    }
    auto res = solve();
    cout << "s ";
    if (res.size()) {
        cout << "SATISFIABLE\n";
        cout << "v ";
        for (int i = 0; i < res.size(); ++i) cout << (res[i] ? 1: -1) * (i+1) << " "; cout << "0\n"; 
    }  else cout << "UNSATISFIABLE\n";
}