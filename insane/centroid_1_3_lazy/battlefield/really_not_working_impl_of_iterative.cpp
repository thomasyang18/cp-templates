

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

const ll L = 20;

const ll N = 2e5 + 10;

ll tin[N], tout[N];
vl adj[N];

void dfs(ll i, ll p) {
    static int time = 0;
    tin[i] = time++;
    for (ll x: adj[i]) if (x-p) dfs(x, i);
    tout[i] = time;
}

namespace lca {
    ll dep[N], par[N][L], distFromRoot[N];

    void lcadfs(ll i, ll p, ll pw) {
        static int time = 0;
        dep[i] = dep[p] + 1;
        par[i][0] = p;
        distFromRoot[i] =  pw; 
        tin[i] = time++;
        F(l, 1, L) {
            par[i][l] = par[par[i][l - 1]][l - 1];
        }
        for(auto j: adj[i]) if(j - p) lcadfs(j, i, pw + 1);
        tout[i] = time;
    }

    ll lca(ll a, ll b) {
        if(dep[a] < dep[b]) swap(a, b);
        FD(l, L - 1, -1) if((dep[a] - dep[b]) >> l) a = par[a][l];
        if(a == b) return a;
        FD(l, L - 1, -1) if(par[a][l] - par[b][l]) a = par[a][l], b = par[b][l];
        return par[a][0];
    }

    ll dist(ll a, ll b){
        return distFromRoot[a] + distFromRoot[b] - 2 * distFromRoot[lca(a, b)];
    }
}

struct lztree {
    typedef ll T;
    typedef ll U;
    // combining segtree nodes a and b
        
    ll f(ll a, ll b) {
        return max(a, b); 
    }

 // applying updates a and b (in that order)
    U g(U b, U a) { return a + b; }
    // applying update b to segtree node a
    T h(U b, T a) {return a+b;}

    ll n;

    T idT = -1e18;
    vector<T> t;
    U idU = 0;
    vector<U> d;

    void calc(ll p) { t[p] = h(d[p], f(t[p * 2], t[p * 2 + 1])); }

    void apply(ll p, U v) {
        t[p] = h(v, t[p]);
        if(p < n) d[p] = g(v, d[p]);
    }

    void push(ll p) {
        p += n;
        FD(s, L, 0) {
            ll i = p >> s;
            if(d[i] != idU) {
                apply(i * 2, d[i]);
                apply(i * 2 + 1, d[i]);
                d[i] = idU;
            }
        }
    }
    #define rein(p) lower_bound(A(mytins), p) - mytins.begin()

    // difference between tshi: i reindexx before doing shit 
    // void modify(ll p, T v) {
    //     p = rein(p);
    //     push(p);
    //     t[p += n] = v;
    //     while(p > 1) calc(p /= 2);
    // }

    void modify(ll l, ll r, U v) {
        l = rein(l), r = rein(r);

        if (l >= r) return;

        push(l), push(r - 1);
        bool cl = false, cr = false;
        for(l += n, r += n; l < r; l /= 2, r /= 2) {
            if(cl) calc(l - 1);
            if(cr) calc(r);
            if(l & 1) apply(l++, v), cl = true;
            if(r & 1) apply(--r, v), cr = true;
        }
        for(--l; r; l /= 2, r /= 2) {
            if(cl) calc(l);
            if(cr) calc(r);
        }
    }

    T query(ll l, ll r) {
        l = rein(l), r = rein(r);

                if (l >= r) return idT;


        push(l), push(r - 1);
        T resl = idT, resr = idT;
        for(l += n, r += n; l < r; l /= 2, r /= 2) {
            if(l & 1) resl = f(resl, t[l++]);
            if(r & 1) resr = f(t[--r], resr);
        }
        return f(resl, resr);
    }

    lztree() {}
    // This lazy tree wants to effectively query tin/tout ranges.
    // So reindex everything by tin; it is the responsibility of the passer to pass in correct tin/tout ranges.
    // (point modifies pass in tin ranges)

    // pass in pair of (node, value)
    vl mytins;
    lztree(vector<pl> pts) {
        assert(pts.size());
        sort(A(pts), [&](const pl &a, const pl& b) {
              return tin[a.K] < tin[b.K];
        });
        n = pts.size();
        mytins.resize(n);
        t.resize(2*n, idT);
        d.resize(n, idU);
        F(i, 0, pts.size()) {
            mytins[i] = tin[pts[i].K];
            t[i+n] = pts[i].V;
        }
        FD(i, n-1, -1) t[i] = f(t[i*2], t[i*2+1]);
    }  
};


struct tree_node {
    // operates on the assumption that both c[0] and c[1] are filled, or neither are filled 
    // (binary tree)
    ll n;
    ll centroid; 
    tree_node * par = nullptr; 
    tree_node* c[2] = {nullptr, nullptr};

    pl times = {1e9, -1e9};
    ll lazy = 0;

    // inefficient way of building for now 
    // this is so ugly

    lztree seg;
    set<ll> debugUniverse;

    tree_node(ll single, tree_node*_par = nullptr) : n(1), par(_par) {
        centroid = single;
        tie(times.K, times.V) = make_tuple(tin[single], tout[single]);

        // This is VERY important since this 
        // dumb segtree cannot even support point update queries, 
        // only subtree...
        // Point update queries are hard since under edge decomposition,
        // many, many nodes can share the same centroid. 
        seg = lztree({pair(single, 0)});

        debugUniverse.insert(single);
    }

    tree_node(vector<pl> edges, 
        tree_node *_par = nullptr) : n(edges.size() + 1), par(_par) {
        
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
            debugUniverse.insert(x);
        }

        auto do_distances = [&](){
            map<ll, ll> distances;

            auto dfs = [&](auto &&self, ll i, ll p, ll d) -> void {
                distances[i] = d;
                for (ll x: adj[i]) if (x-p) self(self, x, i, d + 1);
                // our implementation fails with repeating edges... ffs...
            };
            // Now this is a hack if i've ever seen one.
            // Again, we care about the distance from us to parent's centroid, not to our current centroid,
            // because we want to do the flipping thing.
            // For the root node, well uh, just do the normal centroid ig, everything should work out under evaluate? 
            // not sure abt this, revisit TODO 
            ll pcent = par ? par->centroid : centroid;
            dfs(dfs, pcent, -1, 0);
            
            seg = lztree(vector<pl>(A(distances)));
        };

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

            do_distances();
                return;
            }
            assert(false);
        } else if (n == 2) {
            c[0] = new tree_node(edges[0].K, this);
            c[1] = new tree_node(edges[0].V, this);
            centroid = c[0]->centroid; // this can just be whatever, doesn't matter

            do_distances();
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

        do_distances();
        
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

    void push() {
        seg.modify(tin[1], tout[1], lazy);
        F(i, 0, 2) if (c[i]) c[i]->lazy += lazy;
        lazy = 0; // reset to identity
    }

    ll evaluate() {
        push();
        return seg.query(tin[1], tout[1]);
    }

    bool leaf() const {
        return c[0] == nullptr;
    }

    ~tree_node() {
        F(i, 0, 2) if (c[i]) delete c[i];
    }
};

struct centroid_tree {
    tree_node root;
    ll n;
    vector<tree_node*> callInQuery;
    vector<tree_node*> callInUpdate;
    centroid_tree(vector<pl> adj) : root(adj) , n(adj.size()+1), callInQuery(n+1), callInUpdate(n+1) {
        vl parents(n+1);
        auto dfs = [&](auto &&self, tree_node* cur) -> void {
            assert(cur);
            if (!callInQuery[cur->centroid]) callInQuery[cur->centroid] = cur;

            if (!parents[cur->centroid]) callInUpdate[cur->centroid] = cur;
            parents[cur->centroid]++;
            F(i, 0, 2) if (cur->c[i]) {
                self(self, cur->c[i]);

                // cout << "(";
                // for (auto x: cur->debugUniverse) cout << x << ","; 
                // cout << ")" << " ";
                
                // cout << "(";
                // for (auto x: cur->c[i]->debugUniverse) cout << x << ","; 
                // cout << ")" << endl;
            }
            parents[cur->centroid]--;
        };
        dfs(dfs, &root);
        F(i, 1, n+1) assert(callInQuery[i]), assert(callInUpdate[i]);
    }

    vector<tree_node*> chain(ll node_index, const vector<tree_node*> callIn) { // gets the correct chain all the way down to the deepest needed occurence
        assert(1 <= node_index and node_index <= n);
        vector<tree_node*> res{callIn[node_index]};
        while (res.back()->par) res.push_back(res.back()->par);
        reverse(A(res));
        return res;
    }

    // Nodes on the path, we do the actual applicaiton.
    // Nodes off the path, we add to their push value.
    void update(ll node_index, ll value) {
        cout << "UPDATE " << node_index << " " << value << endl;
        auto nodes = chain(node_index, callInUpdate);

        cout << "wat? " << nodes.size() << endl;
        F(i, 0, nodes.size() - 1) {
            nodes[i]->push();
            ll addedLazily = 0;
            tree_node* next_dfs = nullptr;
            F(j, 0, 2) {    
                auto &cur = nodes[i]->c[j];
                if (tin[node_index] <= cur->times.K and cur->times.V <= tout[node_index]) {
                    cur->lazy += value;
                    addedLazily++;
                } else {
                    next_dfs = cur;
                }

                cout << j << " : ";
                for (auto x: cur->debugUniverse) cout << x << " "; cout << endl;
            }
            if (addedLazily == 2) return; // no more updating needed
            else { // in either 0 or 1 case, you always take next_dfs? idk this is sus (makes sense in 0 not in 1)
                cout << "YO " << addedLazily << endl;
                nodes[i]->seg.modify(tin[node_index], tout[node_index], value);
            } 
        }
        // repeatedly select the child that is NOT fully encompassed inside of me, until 
        // I am no longer the centroid. 

        auto cur = nodes.back();
        cur->seg.modify(tin[node_index], tout[node_index], value);
    }
    
    ll query(ll node_index) {
        ll ans = -1e18;
        auto nodes = chain(node_index, callInQuery);
        F(i, 0, nodes.size() - 1) {
            nodes[i]->push();
            cout << "Iter " << i << endl;

            ll directContribution = lca::dist(node_index, nodes[i]->centroid); 
            F(j, 0, 2) if (nodes[i]->c[j] != nodes[i+1]) {    
                cout << "YA " << node_index << " ; " << nodes[i]->c[j]->centroid << "; " << directContribution << " ; " << nodes[i]->c[j]->evaluate() << endl;
                cout << nodes[i]->c[j]->n << endl;
                ans = max(ans, nodes[i]->c[j]->evaluate() + directContribution);
            } 

        }

        cout << "FUCK OFF AT THIS INDEX " << ans << endl;

        auto& bck = nodes.back();
        bck->push();

        if (!bck->leaf()) {
            F(i, 0, 2) ans = max(ans,
                (bck->c[i]->leaf() ? lca::dist(bck->c[i]->centroid, bck->centroid) : 0) + bck->c[i]->evaluate());
        } else {
            ans =max(ans, bck->evaluate());
        }

        return ans;
    }
};

int main(){
//    freopen("a.in", "r", stdin);
//    freopen("a.out", "w", stdout);

    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(20);
    
    G(n) G(q) 
    
    vector<pl> tadj(n-1);
    for (auto &[x, y]: tadj) {
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
    }
    dfs(1, 1);
    lca::lcadfs(1, 1, 0);
    
    centroid_tree c(tadj);

    ll updateCount = 0;
    
    while(q--) {
        G(t) 

        int crit = 4;

        if (++updateCount == crit) cout << "****************************************************************************" << endl;

        
        if (t == 1) {
            G(x) G(y)
            c.update(x, -y);
        } else {
            G(x)
            cout << c.query(x) << endl;
        }

        if (updateCount == crit) cout << "****************************************************************************" << endl;

    }

}
