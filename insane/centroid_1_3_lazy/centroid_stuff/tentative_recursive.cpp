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

const ll N = 2e5 + 10;

ll tin[N], tout[N];
vl adj[N];

void dfs(ll i, ll p) {
    static int time = 0;
    tin[i] = time++;
    for (ll x: adj[i]) if (x-p) dfs(x, i);
    tout[i] = time;
}

struct centroid_tree {
    // operates on the assumption that both c[0] and c[1] are filled, or neither are filled 
    // (binary tree)
    ll n, centroid; 
    centroid_tree * par = nullptr; 
    centroid_tree* c[2] = {nullptr, nullptr};

    pl times = {1e9, -1e9};
    ll lazy = 0;

    // inefficient way of building for now 
    // this is so ugly

    centroid_tree(ll single, centroid_tree*_par = nullptr) : n(1), par(_par) {
        centroid = single;
        tie(times.K, times.V) = make_tuple(tin[single], tout[single]);
    }

    centroid_tree(vector<pl> edges, 
        centroid_tree *_par = nullptr) : n(edges.size() + 1), par(_par) {
        
        // it is totally possible to migrate these values to the outside
        // and do some preprocessing magic with some "used nodes" vector 
        // and get this down to nlogn instead of nlog^2n.
        // but for now, let's just go with this simple implementation
        // the main reason being is that now this is tied up wtih tin/tout times, which 
        // will have to be a global variable at first, i think. 
        map<ll, ll> sz;
        map<ll, vl> adj;
        for (auto [x, y]: edges) {
            adj[x].push_back(y), adj[y].push_back(x);
        }

        for (auto &[x, _]: adj) {
            times.K = min(times.K, tin[x]);
            times.V = max(times.V, tout[x]);
        }

        /*
            do some processing with the adjlist here 
        */

        if (n == 3) {
            for (const auto &[x, v]: adj) if (v.size() == 2) {
                vector<pl> nedges[2] {
                    {pair(x, v[0])},
                    {pair(x, v[1])}
                };
                centroid = x;
                F(i, 0, 2) c[i] = new centroid_tree(v[i], this);
                return;
            }
            assert(false);
        } else if (n == 2) {
            c[0] = new centroid_tree(edges[0].K, this);
            c[1] = new centroid_tree(edges[0].V, this);
            centroid = c[0]->centroid; // this can just be whatever, doesn't matter
            return;
        }
        assert(n != 1);

        function<ll(ll, ll)> getSize = [&](ll i, ll p) {
            sz[i] = 1;
            for (auto x: adj[i]) if (x-p)
                sz[i] += getSize(x, i);
            return sz[i];
        };

        function<ll(ll, ll)> findcent = [&](ll i, ll p) {
            for (auto x: adj[i]) if (x-p and sz[x] > n/2)
                return findcent(x, i);
            return i;
        };

        ll root = adj.begin()->K;
        getSize(root, -1);
        centroid = findcent(root, -1);
        getSize(centroid, -1);

        ll tot = 0;
        vector<pl> nedge[2];
        for (auto x: adj[centroid]) {
            ll idx;
            if (tot + sz[x] <= 2 * n / 3) {
                tot += sz[x];
                idx = 0;
            } else idx = 1;

            function<void(ll, ll)> add_edges = [&](ll i, ll p) {
                nedge[idx].emplace_back(i, p);
                for (auto x: adj[i]) if (x-p) add_edges(x, i);
            };
            add_edges(x, centroid);
        }
        sz.clear(); adj.clear();

        F(i, 0, 2) {
            assert(nedge[i].size());
            c[i] = new centroid_tree(move(nedge[i]), this);
        }
    }

    void push() {
        F(i, 0, 2) if (c[i]) c[i]->lazy += lazy;
        lazy = 0; // reset to identity
    }

    // TODO: FILL OUT WITH LAZY SEGTREE OR SOMETH
    ll evaluate() {
        push();
        return 0;
    }

    // segtree feels kinda wasteful here? all we are doing is range updating and then range querying aggregates over the whole 
    // tree. perhaps there is some kind of better data structure for this.  
    void update(ll node_index, ll value) {
        push();
        // outside range already
        if (tout[node_index] <= times.K || times.V <= tin[node_index]) return; 
        
        // inside range 
        if (tin[node_index] <= times.K and times.V <= tout[node_index]) {
            lazy += value;
            return;
        }

        // segment tree::update(node_index, node_value) or something

        F(i, 0, 2) {
            assert(c[i]);
            c[i]->update(node_index, value);
        }
    }

    ll query(ll node_index) {
        push();
        if (node_index == centroid) {
            // perhaps some special processing when the endpoints are identical?
            return 0;
        }

        ll directContribution = 0;
        // ll directContribution = f(node_index, centroid); path length, etc.

        ll ans = -1e18; 

        F(i, 0, 2) {
            assert(c[i]);
            auto [tl, tr] = c[i]->times;
            if (!(tl <= tin[node_index] and tin[node_index] < tr)) {
                ans = max(ans, c[i]->evaluate() + directContribution);
            } else {
                // go further down the tree; assert that the condition is true for debugging purposes
                assert(tl <= tin[node_index] and tin[node_index] < tr);
                ans = max(ans, c[i]->query(node_index));
            }
        }

        return ans;
    }


    ~centroid_tree() {
        F(i, 0, 2) if (c[i]) delete c[i];
    }
};

int main(){
//    freopen("a.in", "r", stdin);
//    freopen("a.out", "w", stdout);

    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(20);
    
    G(n)
    vector<pl> tadj(n-1);
    for (auto &[x, y]: tadj) {
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    dfs(1, 1);
    
    centroid_tree c(tadj);

    
}
