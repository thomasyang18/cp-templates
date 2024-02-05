// this is the battefield to ac on probelm F 

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
vector<pair<int, long long>> adj[N];

void dfs(ll i, ll p) {
    static int time = 0;
    tin[i] = time++;
    for (auto [x, _]: adj[i]) if (x-p) dfs(x, i);
    tout[i] = time;
}

namespace lca {
    const int L = 20;
    int dep[N], par[N][L];
    ll distFromRoot[N];

    void lcadfs(ll i, ll p, ll pw) {
        static int time = 0;
        dep[i] = dep[p] + 1;
        par[i][0] = p;
        distFromRoot[i] =  pw; 
        tin[i] = time++;
        F(l, 1, L) {
            par[i][l] = par[par[i][l - 1]][l - 1];
        }
        for(auto [j, w]: adj[i]) if(j - p) lcadfs(j, i, pw + w);
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


typedef pair<ll, int> T;
typedef ll U;
    // combining segtree nodes a and b
        
    T f(T a, T b) {
        return max(a, b); 
    }

 // applying updates a and b (in that order)
    U g(U b, U a) { return a + b; }
    // applying update b to segtree node a
    T h(U b, T a) {return {a.K + b, a.V};}


    constexpr T idT = {-1e18, -1};

    constexpr U idU = 0;
struct lztree {

    ll n, nL;

    vector<T> t;
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

    // difference between tshi: i reindexx before doing shit 
    // void modify(ll p, T v) {
    //     p = rein(p);
    //     push(p);
    //     t[p += n] = v;
    //     while(p > 1) calc(p /= 2);
    // }

    void modify(ll l, ll r, U v) {
        if (v == idU) return;
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

    T query() const {  return t[1]; }

    lztree() {}
    // This lazy tree wants to effectively query tin/tout ranges.
    // So reindex everything by tin; it is the responsibility of the passer to pass in correct tin/tout ranges.
    // (point modifies pass in tin ranges)

    // pass in pair of (node, value)
    vl mytins;
    lztree(vector<pair<int, long long>> pts) {
        assert(pts.size());
        sort(A(pts), [&](const pl &a, const pl& b) {
              return tin[a.K] < tin[b.K];
        });

        n = pts.size();
        nL = 32 - __builtin_clz(n);
        mytins.resize(n);
        t.resize(2*n, idT);
        d.resize(n, idU);
        F(i, 0, pts.size()) {
            mytins[i] = tin[pts[i].K];
            t[i+n] = {pts[i].V, -pts[i].K};
        }
        FD(i, n-1, -1) t[i] = f(t[i*2], t[i*2+1]);
    }  
};

int a[N];
int sz[N];

struct centroid_tree {
    // operates on the assumption that both c[0] and c[1] are filled, or neither are filled 
    // (binary tree)
    int centroid;
    centroid_tree* c[2] = {nullptr, nullptr};

    pair<int, int> times = {1e9, -1e9};
    U lazy = 0;

    lztree* seg = nullptr;
    vector<pair<int, T>>* arr = nullptr; // 

    constexpr static ll THRESHOLD = 3; // threshold for just storing repr as array 

    centroid_tree(vector<pair<pl, ll>> edges, 
        centroid_tree *_par = nullptr) {
        ll n(edges.size() + 1);
        
        for (auto [a, w]: edges) {
            auto [x, y] = a;

            adj[x].clear(), adj[y].clear();
            times.K = min(times.K, tin[x]);
            times.V = max(times.V, tout[x]);

            times.K = min(times.K, tin[y]);
            times.V = max(times.V, tout[y]);
        }

        for (auto [a, w]: edges){
            auto [x, y] = a;
            adj[x].emplace_back(y, w), adj[y].emplace_back(x, w);
        }

        auto do_distances = [&](){
            vector<pair<int, long long>> distances;

            auto dfs = [&](auto &&self, ll i, ll p, ll d) -> void {
                distances.emplace_back(i, d - a[i]);
                for (auto [x, w]: adj[i]) if (x-p) self(self, x, i, d + w);
                // our implementation fails with repeating edges... ffs...
            };
            // Now this is a hack if i've ever seen one.
            // Again, we care about the distance from us to parent's centroid, not to our current centroid,
            // because we want to do the flipping thing.
            // For the root node, well uh, just do the normal centroid ig, everything should work out under evaluate? 
            // not sure abt this, revisit TODO 
            ll pcent = _par ? _par->centroid : 1;
            dfs(dfs, pcent, -1, 0);
            
            seg = new lztree(move(distances));
        };
        do_distances();

        if (n <= THRESHOLD) {
            // do leaf construction
            arr = new vector<pair<int, T>>;
            set<ll> uniq;
            for (auto [a, w]: edges) {
                auto [x, y] = a;
                uniq.insert(x), uniq.insert(y);
            }
            for (auto x: uniq) arr->emplace_back(x, pair(-a[x], -x));
            return;
        } 

        function<ll(ll, ll)> getSize = [&](ll i, ll p) {
            sz[i] = 1;
            for (auto [x, _]: adj[i]) if (x-p)
                sz[i] += getSize(x, i);
            return sz[i];
        };

        function<ll(ll, ll)> findcent = [&](ll i, ll p) {
            for (auto [x, _]: adj[i]) if (x-p and sz[x] > n/2)
                return findcent(x, i);
            return i;
        };

        getSize(edges[0].K.K, -1);
        centroid = findcent(edges[0].K.K, -1);        
        getSize(centroid, -1);

        ll tot = 0;
        vector<pair<pl, ll>> nedge[2];
        for (auto [x, _]: adj[centroid]) {
            ll idx;
            if (tot + sz[x] <= 2 * n / 3) {
                tot += sz[x];
                idx = 0;
            } else idx = 1;

            function<void(ll, ll)> add_edges = [&](ll i, ll p) {
                for (auto [x, w]: adj[i]) if (x-p) add_edges(x, i);
                else {
                    nedge[idx].emplace_back(pair(i, p), w);    
                }
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
        if (lazy == idU) return;
        // full application
        seg->modify(tin[1], tout[1], lazy);

        if (leaf()) {
            assert(arr);
            for (auto &[_, v]: *arr) v = h(lazy, v);
        }
        else F(i, 0, 2) c[i]->lazy += lazy;

        lazy = idU; // reset to identity
    }

    // TODO: FILL OUT WITH LAZY SEGTREE OR SOMETH - DONE
    T evaluate() {
        push();
        return seg->query();
    }

    // segtree feels kinda wasteful here? all we are doing is range updating and then range querying aggregates over the whole 
    // tree. perhaps there is some kind of better data structure for this.  
    void update(ll node_index, ll value) {
        push();

        if (leaf()) {    
            assert(arr);
            for (auto &[x, v]: *arr) 
                if (tin[node_index] <= tin[x] and tin[x] < tout[node_index]) 
                    v = h(value, v);
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

        if (contains) {
            // go do ur recursion bro
        } else return; // stop here


        // cout << "ACTUALLY APPLYING LOL! " << endl;
        // for (auto x: care) cout << x << " "; cout << endl;


        // TODO - DONE 
        seg->modify(tin[node_index], tout[node_index], value);
        // segment tree::update(node_index, node_value) or something

        F(i, 0, 2) {
            assert(c[i]);
            c[i]->update(node_index, value);
        }
    }

    T query(ll node_index) {
        push();

        if (leaf()) {
            T res = idT;
            // perhaps make thsi faster, making a bunch of log(n) calls...
            assert(arr);
            for (const auto &[k, v]: *arr) 
                res = f(res, h(lca::dist(node_index, k), v));

            // cout << "RETURNING " << res << endl;
            
            return res;
        }

        // cout << "is my universe actually correct...? " << endl;
        // cout << "QUERYING "  << centroid << endl;
        // for (auto x: care) cout << x << " "; cout << endl;
        // for (auto x: care) cout << seg.query(tin[x], tin[x]+1) << " "; cout << endl;

        // cout << "wat? " << evaluate() << endl;
        // cout << "======================" << endl;

        if (node_index == centroid) {
            T res = idT;
            F(i, 0, 2) res = f(res, c[i]->evaluate());

            // warning: above edge case is kinda hacky, but ig the way i set it up makes sense like that 
            // This works since both children will point to me, the centroid. 
            // cout << "RETURNING ZERO " << res << endl;

            return res;
        }

        ll directContribution = lca::dist(node_index, centroid); 

        // ll directContribution = f(node_index, centroid); path length, etc.

        T ans = idT; 

        // cout << " YA " << node_index << " " << centroid << " ; " << n << endl;

        // cout << "? " << directContribution << endl;

        ll tried = 0;
        F(i, 0, 2) {
            assert(c[i]);
            auto [tl, tr] = c[i]->times;

            auto pos = lower_bound(A(c[i]->seg->mytins), tin[node_index]);
            bool contains = pos != c[i]->seg->mytins.end() and *pos == tin[node_index];

            if (!contains) {
                ans = f(ans, h(directContribution, c[i]->evaluate()));
                // cout << "THE OTHER UNIVERSE???? " << endl;
                // for (auto x: c[i]->care) cout << x << " "; cout << endl;
                // for (auto x: c[i]->care) cout << c[i]->seg.query(tin[x], tin[x]+1) << " "; cout << endl;
                
                // cout << "WTF " << c[i]->evaluate() << " " << directContribution << endl;
            } else {
                // go further down the tree; assert that the condition is true for debugging purposes
                assert(contains);
                ans = max(ans, c[i]->query(node_index));
                tried++;
            }
        }
        assert(tried == 1);

        // cout << "ANSWER: " << ans << endl;

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

    F(i ,1, n+1) cin >> a[i];
    
    vector<pair<pl, ll>> tadj;

    F(i, 2, n+1) {
        G(p) G(w)
        adj[p].emplace_back(i, w);
        tadj.emplace_back(pair(i, p), w);
    }

    dfs(1, 1);
    lca::lcadfs(1, 1, 0);
    
    centroid_tree c(tadj);

    ll updateCount = 0;
    
    while(q--) {
        G(x) G(y) G(v)
        
        // ll crit = 4;
        // if (++updateCount == crit) cout << "****************************************************************************" << endl;
        
        c.update(y, -v);
        auto [a, b] = c.query(x);
        cout << -b << " " << a << endl;

        // if (updateCount == crit) cout << "****************************************************************************" << endl;
    }

}
