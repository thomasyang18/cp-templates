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

const int N = 2e5 + 10;

int tin[N], tout[N];
vector<int> adj[N];

void dfs(ll i, ll p) {
    static int time = 0;
    tin[i] = time++;
    for (ll x: adj[i]) if (x-p) dfs(x, i);
    tout[i] = time;
}

namespace lca {
    const int L = 20;
    int dep[N], par[N][L];
    long long distFromRoot[N];

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

    int lca(ll a, ll b) {
        if(dep[a] < dep[b]) swap(a, b);
        FD(l, L - 1, -1) if((dep[a] - dep[b]) >> l) a = par[a][l];
        if(a == b) return a;
        FD(l, L - 1, -1) if(par[a][l] - par[b][l]) a = par[a][l], b = par[b][l];
        return par[a][0];
    }

    long long dist(ll a, ll b){
        return distFromRoot[a] + distFromRoot[b] - 2 * distFromRoot[lca(a, b)];
    }
}


typedef long long T;
typedef long long U;
// combining segtree nodes a and b
    
T f(T a, T b) {
    return max(a, b); 
}

// applying updates a and b (in that order)
U g(U b, U a) { return a + b; }
// applying update b to segtree node a
T h(U b, T a) {return a+b;}


struct lztree {
    int n, nL;

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
        FD(s, nL, 0) {
            ll i = p >> s;
            if(d[i] != idU) {
                apply(i * 2, d[i]);
                apply(i * 2 + 1, d[i]);
                d[i] = idU;
            }
        }
    }
    #define rein(p) lower_bound(A(mytins), p) - mytins.begin()

    void modify(int l, int r, U v) {
        // return;
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

    // wow can't believe this actually gets just recalcualted ty jdurie
    T query() const {  return t[1]; }

    lztree() {}
    // This lazy tree wants to effectively query tin/tout ranges.
    // So reindex everything by tin; it is the responsibility of the passer to pass in correct tin/tout ranges.
    // (point modifies pass in tin ranges)

    // pass in pair of (node, value)
    vl mytins;
    lztree(vector<pair<int, long long>> pts) {
        assert(pts.size());
        sort(A(pts), [&](const auto &a, const auto &b) {
              return tin[a.K] < tin[b.K];
        });
        n = pts.size();
        nL = 32 - __builtin_clz(n);
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


// cancer global variable time!
int sz[N];

struct centroid_tree {
    // operates on the assumption that both c[0] and c[1] are filled, or neither are filled 
    // (binary tree)
    int centroid; 
    centroid_tree* c[2] = {nullptr, nullptr};

    pair<int, int> times = {1e9, -1e9};
    
    T lazy = 0;

    // inefficient way of building for now 
    // this is so ugly

    lztree* seg = nullptr;
    vector<pair<int, T>>* arr = nullptr; // 

    constexpr static ll THRESHOLD = 20; // threshold for just storing repr as array 

    // set<ll> debugCare;

    centroid_tree(vector<pair<int, int>> edges, 
        centroid_tree *_par = nullptr) {
        int n = edges.size() + 1;
        
        for (auto [x, y]: edges) {
            adj[x].clear(), adj[y].clear();
            times.K = min(times.K, tin[x]);
            times.V = max(times.V, tout[x]);

            times.K = min(times.K, tin[y]);
            times.V = max(times.V, tout[y]);
        }

        for (auto [x, y]: edges) {
            adj[x].push_back(y), adj[y].push_back(x);
            // debugCare.insert(x), debugCare.insert(y);
        }

        auto do_distances = [&](){
            vector<pair<int, long long>> distances;

            auto dfs = [&](auto &&self, ll i, ll p, long long d) -> void {
                distances.emplace_back(i, d);
                for (ll x: adj[i]) if (x-p) self(self, x, i, d + 1);
            };
            // Now this is a hack if i've ever seen one.
            // Again, we care about the distance from us to parent's centroid, not to our current centroid,
            // because we want to do the flipping thing.
            // For the root node, well uh, just do the normal centroid ig, everything should work out under evaluate? 
            // not sure abt this, revisit TODO 
            ll pcent = _par ? _par->centroid : 1; // doesn't actually matter if parent doesn't exist
            // one opt is to special case this out, don't initialize lazy tree, and for coontains query, simply return true 
            // also we can make contains a member function 
            dfs(dfs, pcent, -1, 0);
            
            seg = new lztree(move(distances));
        };
        do_distances();

        if (n <= THRESHOLD) {
            // do leaf construction
            arr = new vector<pair<int, T>>;
            set<ll> uniq;
            for (auto [x, y]: edges) uniq.insert(x), uniq.insert(y);
            for (auto x: uniq) arr->emplace_back(x, 0);
            return;
        } 

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

        // start from any arbitrary index
        getSize(edges[0].K, -1);
        centroid = findcent(edges[0].K, -1);            
        getSize(centroid, -1);

        ll tot = 0;
        vector<pair<int, int>> nedge[2];
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

        edges.clear();

        F(i, 0, 2) {
            assert(nedge[i].size());
            c[i] = new centroid_tree(move(nedge[i]), this);
        }
    
    }

    bool leaf() const {
        return c[0] == nullptr;
    }

    void push() {
        if (!lazy) return;
        // perhaps make this faster
        seg->modify(tin[1], tout[1], lazy);

        if (leaf()) {
            assert(arr);
            for (auto &[_, v]: *arr) v += lazy;
        }
        else F(i, 0, 2) c[i]->lazy += lazy;

        lazy = 0; // reset to identity
    }

    // TODO: FILL OUT WITH LAZY SEGTREE OR SOMETH - DONE
    T evaluate() {
        push();
        return seg->query();
    }

    void update(ll node_index, U value) {
        push();
        // hacky
        if (leaf()) {    
            assert(arr);
            for (auto &[x, v]: *arr) 
                if (tin[node_index] <= tin[x] and tin[x] < tout[node_index]) 
                    v += value;
            // also need to do this to keep truth intact
            seg->modify(tin[node_index], tout[node_index], value);
            return;
        }
        
        if (tin[node_index] <= times.K and times.V <= tout[node_index]) {
            lazy += value;
            return;
        }

        // This only works for a centroid tree that has no ancestors of the descired node_index
        if (tout[node_index] <= times.K || times.V <= tin[node_index]) return; 

        // If I have ancestors of my node_index, this will fail horribly.
        // But if I have a node as my ancestor, I need to check if node_index is actually visited by this set of 
        // centroid nodes.
        // If if its, then it's partial; go recurse.
        // OTHERWISE, I must not affect anything in the subtree of node_index, therefore we are good to return here. 
        
        auto pos = lower_bound(A(seg->mytins), tin[node_index]);
        bool contains = pos != seg->mytins.end() and *pos == tin[node_index];
        if (!contains) return;

        // TODO - DONE 
        // apply to entire subtree here 
        seg->modify(tin[node_index], tout[node_index], value);
        
        F(i, 0, 2) c[i]->update(node_index, value);
    }

    T query(int node_index) {
        // cout << "QUERYING INTO NIVERSE " << endl;
        // for (auto x: debugCare) cout << x << " "; cout << endl;

        push();
        if (leaf()) {
            T res = -1e18;
            // perhaps make thsi faster, making a bunch of log(n) calls...
            assert(arr);
            for (const auto &[k, v]: *arr) 
                res = max(res, lca::dist(node_index, k) + v);

            // cout << "RETURNING " << res << endl;
            
            return res;
        }

        if (node_index == centroid) {
            T res = -1e18;
            F(i, 0, 2) res = max(res, c[i]->evaluate());


            // cout << "RETURNING BIG BOI??? " << res << endl;
            return res;
        }

        ll directContribution = lca::dist(node_index, centroid); 

        T ans = -1e18; 

        ll tried = 0;
        F(i, 0, 2) {
            assert(c[i]);

            auto pos = lower_bound(A(c[i]->seg->mytins), tin[node_index]);
            bool contains = pos != c[i]->seg->mytins.end() and *pos == tin[node_index];

            if (!contains) {
                ans = max(ans, c[i]->evaluate() + directContribution);

                // cout << "Okay, we didn't recurse yet. We are on universe: " << endl;
                // for (auto x: debugCare) cout << x << " "; cout << endl;
                // cout << "with centroid " << centroid << endl;

                // cout << "THE OTHER UNIVERSE???? " << endl;
                // for (auto x: c[i]->debugCare) cout << x << " "; cout << endl;
                // ll idx = 0;
                // for (auto x: c[i]->debugCare) {
                //     auto &fuck = c[i]->seg;
                //     cout << fuck->t[idx +fuck->n] << " "; 
                //     ++idx;
                // }
                // cout << endl;
                
                
                // cout << "WTF " << c[i]->evaluate() << " " << directContribution << endl;
            } else {
                // go further down the tree; assert that the condition is true for debugging purposes
                assert(contains);
                ans = max(ans, c[i]->query(node_index));
                tried++;
            }
        }
        assert(tried == 1);

        // cout << "Returning ans " << ans << endl;

        return ans;
    }


    ~centroid_tree() {
        F(i, 0, 2) if (c[i]) delete c[i];
        if (seg) delete seg;
        if (arr) delete arr;
    }
};

int main(){
//    freopen("a.in", "r", stdin);
//    freopen("a.out", "w", stdout);

    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(20);
    
    G(n) G(q) 
    
    vector<pair<int, int>> tadj(n-1);
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
        // if (q%10000 == 0) cerr << "WTF " << q/10000 << endl;
        G(t) 
        
        // ll crit = 6;
        // if (++updateCount == crit) cout << "****************************************************************************" << endl;
        if (t == 1) {
            G(x) G(y)
            c.update(x, -y);
        } else {
            G(x)
            cout << c.query(x) << endl;
        }

        // if (updateCount == crit) cout << "****************************************************************************" << endl;
    }

}
