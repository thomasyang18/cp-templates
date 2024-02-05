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

#define M 1000000007 // 998244353


struct tree_node {
    // operates on the assumption that both c[0] and c[1] are filled, or neither are filled 
    // (binary tree)
    ll n, centroid; 
    tree_node * par = nullptr; 
    tree_node* c[2] = {nullptr, nullptr};

    // inefficient way of building for now 
    // this is so ugly

    tree_node(ll single, tree_node*_par = nullptr) : n(1), par(_par) {
        centroid = single;
    }

    tree_node(vector<pl> edges, 
        tree_node *_par = nullptr) : n(edges.size() + 1), par(_par) {

        map<ll, ll> sz;
        map<ll, vl> adj;
        for (auto [x, y]: edges) {
            adj[x].push_back(y), adj[y].push_back(x);
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
                F(i, 0, 2) c[i] = new tree_node(v[i], this);
                return;
            }
            assert(false);
        } else if (n == 2) {
            c[0] = new tree_node(edges[0].K, this);
            c[1] = new tree_node(edges[0].V, this);
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
            c[i] = new tree_node(move(nedge[i]), this);
        }
    }

    ~tree_node() {
        F(i, 0, 2) if (c[i]) delete c[i];
    }
};

struct centroid_tree {
    ll n;
    vector<tree_node*> callIn;
    tree_node root;
    centroid_tree(vector<pl> edges) : n(edges.size()+1), callIn(n+1, nullptr)
    , root(edges, nullptr) 
    {
        auto fillCallIn = [&](auto &&self, tree_node* cur) -> void {
            assert(1 <= cur->centroid and cur->centroid <= n);
            if (!callIn[cur->centroid]) callIn[cur->centroid] = cur;
            F(i, 0, 2) if (cur->c[i]) self(self, cur->c[i]);
        };
        fillCallIn(fillCallIn, &root);
        F(i, 1, n+1) assert(callIn[i]);
    }
};


int main(){
//    freopen("a.in", "r", stdin);
//    freopen("a.out", "w", stdout);

    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(20);
    
    G(n)
    vector<pl> adj(n-1);
    for (auto &[x, y]: adj) cin >> x >> y;
    
    centroid_tree c(adj);
}
