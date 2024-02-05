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

#define N 200010

ll tin[N], tout[N], dep[N];
vl adj[N];
ll a[N], arr[N];

void dfs(ll i, ll p) {
    dep[i] = dep[p]+1;
    static int time = 0;
    arr[time] = i; // place in array
    tin[i] = time++;
    for (ll x: adj[i]) if (x-p) dfs(x, i);
    tout[i] = time;
}
/*

    +X to all nodes in subtree of a[i]  
    furthest (distance - a[i]) from node Y to any other node  
*/
ll gans =0 ;
void dfs2(ll i, ll p, ll dist) {
    // for now, let's not consider distance at all 

    // maximum -a[i] value
    // i know, kind of retarded, but let's just go with it for testing for now 
    // gans = max(gans, -a[i]);

    gans = max(gans, dist - a[i]);
    // gans = max(gans, - dist - a[i]); // negative distance so our thing works properly
    
    for (auto x: adj[i]) if (x-p) dfs2(x, i, dist+1);
}

int main(){
//    freopen("a.in", "r", stdin);
//    freopen("a.out", "w", stdout);

    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(20);
    
    G(n) G(q)
    F(i, 1, n) {
        G(x) G(y) adj[x].push_back(y); adj[y].push_back(x);
    }
    dfs(1, 1);
     while(q--) {
        G(t) if (t == 1) {
            G(x) G(y)
            F(i, tin[x], tout[x]) a[arr[i]] += y;
        } else {
            G(x)
            gans = -1e18;
            dfs2(x, x, 0);
            cout << gans << endl;
        }
    }
}
