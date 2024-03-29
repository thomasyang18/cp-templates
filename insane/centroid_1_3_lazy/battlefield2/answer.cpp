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
ll sz[N];

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

struct segtree {
    typedef pl T;
    T id={0, 0};
    T f(T a, T b) {return {a.K + b.K, a.V + b.V};}

    ll n;
    vector<T> t;

    segtree () {}

    vl mytins;
    segtree(vector<pair<int, long long>> pts) {
        if (!pts.size()) {
            n = 0;
            return;
        }
        assert(pts.size());
        sort(A(pts), [&](const auto &a, const auto &b) {
              return tin[a.K] < tin[b.K];
        });
        n = pts.size();
        mytins.resize(n);
        t.resize(2*n, id);
        F(i, 0, pts.size()) {
            mytins[i] = tin[pts[i].K];
            assert(pts[i].V);
            t[i+n] = {pts[i].V, 1};
        }
        FD(i, n-1, -1) t[i] = f(t[2*i], t[2*i+1]); 
    }  


    T query(ll l, ll r) { // fold f on interval [l, r)
        if (!n) return id;

        #define rein(p) lower_bound(A(mytins), p) - mytins.begin()
        l = rein(l), r = rein(r);
            
        T resl=id, resr=id;
        for (l += n, r += n; l < r; l /= 2, r /= 2) {
            if (l&1) resl = f(resl, t[l++]);
            if (r&1) resr = f(t[--r], resr);
        }
        return f(resl, resr);
    }
};

#undef rein 

ll timesWasCentroid[N];


#define DEBUG_FLAG 0
#define DEBUG_QUERY 01 && DEBUG_FLAG
#define DEBUG_UPDATE 00 && DEBUG_FLAG

struct centroid_tree {
    int centroid; 
    centroid_tree* c[2] = {nullptr, nullptr};

    pair<int, int> times = {1e9, -1e9};
    
    ll lazy = 0;

    ll contValue = 0; // dist(centroid, j) * A_j = value
    ll sumA = 0; // sum of A in subtree 

    segtree* seg = nullptr;

    // arr in this case simply stores a[i] values 
    vector<pair<int, long long>> arr; 

    constexpr static ll THRESHOLD = 5; // threshold for just storing repr as array 

    // set<ll> debugCare;
    // set<ll> actuallyActive;

    vl tins;

    centroid_tree(vector<pair<int, int>>& edges, 
        centroid_tree *_par = nullptr) {
        int n = edges.size() + 1;
        
        for (auto [x, y]: edges) {
            adj[x].clear(), adj[y].clear();
            auto upd = [&](ll x) {
                // TRYING SOMETHING NEW TO AVOID DOUBLE COUNTING CENTROID 
                // STUFFS 

                // Let's actually include everything, straight up. 

                times.K = min(times.K, tin[x]);
                times.V = max(times.V, tout[x]);
            };
            upd(x), upd(y);
        }

        for (auto [x, y]: edges) {
            adj[x].push_back(y), adj[y].push_back(x);
        }

        if (_par) {
            if (!_par) return;
            vector<pair<int, long long>> distances;

            auto dfs = [&](auto &&self, ll i, ll p, long long d) -> void {
                // let's try this? idk this probably leads to us never counting
                // the centroid (i.e. query index) which could be bad 
                if (timesWasCentroid[i] == 0) {
                    distances.emplace_back(i, d);

                    // actuallyActive.insert(i);

                
                }

                for (ll x: adj[i]) if (x-p) self(self, x, i, d + 1);

                tins.push_back(tin[i]);
                // debugCare.insert(i);
            };

            dfs(dfs, _par->centroid, -1, 0);

            sort(A(tins));
            seg = new segtree(move(distances));
        }

        if (n <= THRESHOLD) {
            // do leaf construction
            set<ll> uniq;
            for (auto [x, y]: edges) {
                // trying something new here
                auto upd = [&](ll x) {
                    if (timesWasCentroid[x] == 0) {
                        uniq.insert(x);
                        // actuallyActive.insert(x);
                    }
                };
                upd(x), upd(y);
            }

            for (auto x: uniq) arr.emplace_back(x, 0);
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

        edges.clear();

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

        timesWasCentroid[centroid]++;
        F(i, 0, 2) c[i] = new centroid_tree(nedge[i], this);
        timesWasCentroid[centroid]--;

        if (timesWasCentroid[centroid] == 0) {
            arr.emplace_back(centroid, 0);
            // idfk whats going on anymore
        } // default init for centroid!!!!
        // since we're only keeping track of one node, this should be easy to update... right???
    }

    bool leaf() const {
        return c[0] == nullptr;
    }

    bool root() const {
        return seg == nullptr;
    }

    // node_index must exist as an actual, physical node in my interprtation of things. 
    // though maybe we can change it later, look into it later
    bool containsNode(ll node_index) const {
        if (root()) return true;
        auto pos = lower_bound(A(tins), tin[node_index]);

        bool result = pos != tins.end() and *pos == tin[node_index];

        return result;
    }

    void push() {
        if (lazy == 0) return;
        if (!root()) {
            auto [sdist, cnt] = seg->query(tin[1], tout[1]);
            contValue += sdist * lazy;
            sumA += cnt * lazy;
        }

        for (auto &[_, v]: arr) v += lazy;  // all nodes have this now...
        
        if (!leaf()) F(i, 0, 2) c[i]->lazy += lazy;
        lazy = 0; 
    }

    pair<ll, ll> evaluate() {
        push();
        return {contValue, sumA}; 
    }

    void update(ll node_index, ll value) {
        push();

        // if (DEBUG_UPDATE) {
        //     cout << "UPDATING THIS UNIVERSE: " << endl;
        //     cout << "MY CENTROID: " << centroid << endl;
        //     for (auto x: actuallyActive) cout << x << " "; cout << endl;
        // }

        // hacky
        if (leaf()) {    
            if (DEBUG_UPDATE) {
                cout << "universees collide " << endl;
                // for (auto x: actuallyActive) cout << x << " "; cout << endl;
            } 

            for (auto &[x, v]: arr) 
                if (tin[node_index] <= tin[x] and tin[x] < tout[node_index]) 
                    v += value; 
            // also need to do this to keep truth intact
            if (!root()) {
                auto [sdist, cnt] = seg->query(tin[node_index], tout[node_index]);
                contValue += sdist * value;
                sumA += cnt * value;
            }
            return;
        }
        
        // if all nodes in this tree are marked, apply lazy value 
        if (tin[node_index] <= times.K and times.V <= tout[node_index]) {
            lazy += value;

            if (DEBUG_UPDATE) {
                cout << "Lazily added to these nodes: " << endl;
                // for (auto x: actuallyActive) cout << x << " "; cout << endl;
            }
            return;
        }

        // If I have no ancestors, return.
        if (tout[node_index] <= times.K || times.V <= tin[node_index]) return; 
        // I know that an ancestor of node_index exists in my set. If node_index is in this tree, it must be a partial intersection;
        // else, no intersection
        if (!containsNode(node_index)) return;


        if (DEBUG_UPDATE) {
            cout << "Did a modification to " << node_index << " of " << value << " to these nodes " << endl;
            // for (auto x: actuallyActive) cout << x << " "; cout << endl;
        }

        if (!root()) {
            auto [sdist, cnt] = seg->query(tin[node_index], tout[node_index]);
            contValue += sdist * value;
            sumA += cnt * value;
        }

        // update the centroid too ! wasteful duplicated code hto fix up

        for (auto &[x, v]: arr) 
            if (tin[node_index] <= tin[x] and tin[x] < tout[node_index]) 
                v += value; 


        F(i, 0, 2) c[i]->update(node_index, value);
    }


    ll query(int node_index) {
        push();

        if (leaf()) {
            if (DEBUG_QUERY) {
                cout << "Directly linked to: " << endl;
                // for (auto x: actuallyActive) cout << x << " "; cout << endl;
            }

            ll res = 0;
            for (const auto &[k, v]: arr) {
                res += lca::dist(node_index, k) * v;

                if (DEBUG_QUERY) cout << "(" << k << ", " << v << ") "; 
            }
            if (DEBUG_QUERY) cout << endl;
            return res;
        }

        ll ans = 0;
        if (arr.size()) {
            // compute value of centroid here... centroid will never be called otherwise. 
            assert(arr[0].K == centroid);
            ans += lca::dist(node_index, arr[0].K) * arr[0].V;

            // if (DEBUG_QUERY) cout << "WE ARE INCLUDING CENTROID WITH VALUE: " << centroid << ", " << arr[0].V << endl;
        } 

        // the following is potentially incorrect 
        // because of double counting the centroid? we'll see tho

        if (node_index == centroid) { // my children have the answer
            // this is 100% wrong but idk how its working 
            // cout << "YO UFUCED UP THIS IS GONNA BE WRONG " << endl;
            ll res = 0;

            if (DEBUG_QUERY) cout << "Checking: " << endl;
            F(i, 0, 2) {
                auto [vv, sma] = c[i]->evaluate();

                res += vv;

                if (DEBUG_QUERY) {
                    cout << "??? " << vv << " " << sma << endl;

                    // for (auto x: c[i]->actuallyActive) cout << x << " "; cout << endl;
                }
            }

            return res;
        }

        centroid_tree* todfs = nullptr;

        F(i, 0, 2) {
            if (!c[i]->containsNode(node_index)) {
                
                auto [v, sma] = c[i]->evaluate();

                
                ans += v + sma * lca::dist(node_index, centroid);

                if (DEBUG_QUERY) {
                    cout << "Actually cared about: " << endl;
                    // for (auto x: c[i]->actuallyActive) cout << x << " "; cout << endl;
                    // cout << "Extra info debug care " << endl;
                    // for (auto x: c[i]->debugCare) cout << x << " "; cout << endl;
                    // for (auto x: c[i]->tins) cout << x << " "; cout << endl;
                    // cout << tin[node_index] << endl;

                    // cout << "WTF " << v << " " << sma << endl;

                    // cout << "No? " << endl;
                    // ll bn = c[i]->seg->n;
                    // F(j, 0, bn) cout << c[i]->seg->t[j+bn].K << " "; 
                    // cout << endl;
                }
            } else {
                todfs = c[i];
                // ans += c[i]->query(node_index);
            }
        }
        // for debugging purposes, this makes tracking calls easiser
        assert(todfs);
        if (DEBUG_QUERY) {
            cout << "Going into full universe " << endl;
            // for (auto x: todfs->debugCare) {
                // cout << x << " ";
            // }
            cout << endl;
        }
        ans += todfs->query(node_index);

        return ans;
    }


    ~centroid_tree() {
        F(i, 0, 2) if (c[i]) delete c[i];
        if (seg) delete seg;
    }
};

#undef K 
#undef V

namespace chinese_guys_solution {
    #define go(u) for(int i=head[u],v=e[i].to;i;i=e[i].link,v=e[i].to) 
    int n,head[200010],o=0,dep[200010],sz[200010],fa[200010],top[200010],son[200010],id[200010],re[200010],x=0;
    int *dn[200010],DN[3800000],M=200000;
    struct edge{
        int to,link;
    }e[400010];
    void add_edge(int u,int v){
        e[++o]={v,head[u]},head[u]=o;
        e[++o]={u,head[v]},head[v]=o;
    }
    void dfs(int u,int pre){
        sz[u]=1,dep[u]=dep[fa[u]=pre]+1;
        go(u) if(v^pre){
            dfs(v,u),sz[u]+=sz[v];
            if(sz[son[u]]<sz[v]) son[u]=v;
        }
    }
    void dfs3(int u,int rt,int *up){
        up[id[u]]=rt;
        go(u) if(v^fa[u]) dfs3(v,rt,up);
    }
    void dfs2(int u,int tp){
        top[u]=tp,re[id[u]=++x]=u;
        if(son[u]) dfs2(son[u],tp);
        go(u) if(v!=fa[u]&&v!=son[u]) dfs2(v,v);
        if(son[u]){
            dn[u]=DN+M-id[u]-sz[son[u]]-1,M+=sz[u]-sz[son[u]]-1;
            go(u) if(v!=fa[u]&&v!=son[u]) dfs3(v,v,dn[u]);
        }
    }
    int way(int u,int v){
        if(id[son[u]]<=id[v]&&id[u]+sz[son[u]]>=id[v]) return son[u];
        return dn[u][id[v]];
    }
    ll tr[800010],C[800010],B[800010],K[800010],sA;
    int ad[800010];
    void add(int t,int c){
        ad[t]+=c;
        tr[t]+=c*C[t];
    }
    void add(int l,int r,int t,ll k,ll b){
        tr[t]+=(l+r)*k*(r-l+1)/2+(r-l+1)*b;
        K[t]+=k,B[t]+=b;
    }
    void pushdown(int l,int r,int t){
        if(ad[t]) add(t<<1,ad[t]),add(t<<1|1,ad[t]),ad[t]=0;
        int mid=(l+r)>>1;
        if(K[t]||B[t]) add(l,mid,t<<1,K[t],B[t]),add(mid+1,r,t<<1|1,K[t],B[t]),K[t]=B[t]=0;
    }
    void pushup(int t){
        tr[t]=tr[t<<1]+tr[t<<1|1];
    }
    void build(int l,int r,int t){
        if(l==r) return C[t]=sz[re[l]],void();
        int mid=(l+r)>>1;
        build(l,mid,t<<1),build(mid+1,r,t<<1|1);
        C[t]=C[t<<1]+C[t<<1|1];
    }
    void add(int ql,int qr,int l,int r,int t,ll k,ll b){
        if(ql<=l&&r<=qr) return add(l,r,t,k,b);
        int mid=(l+r)>>1;pushdown(l,r,t);
        if(ql<=mid) add(ql,qr,l,mid,t<<1,k,b);
        if(mid<qr) add(ql,qr,mid+1,r,t<<1|1,k,b);
        pushup(t);
    }
    void add(int ql,int qr,int l,int r,int t,int c){
        if(ql<=l&&r<=qr) return add(t,c);
        int mid=(l+r)>>1;pushdown(l,r,t);
        if(ql<=mid) add(ql,qr,l,mid,t<<1,c);
        if(mid<qr) add(ql,qr,mid+1,r,t<<1|1,c);
        pushup(t);
    }
    ll qry(int ql,int qr,int l,int r,int t){
        if(ql<=l&&r<=qr) return tr[t];
        int mid=(l+r)>>1;pushdown(l,r,t);
        if(qr<=mid) return qry(ql,qr,l,mid,t<<1);
        if(mid<ql) return qry(ql,qr,mid+1,r,t<<1|1);
        return qry(ql,qr,l,mid,t<<1)+qry(ql,qr,mid+1,r,t<<1|1);
    }
    void up(int u,int c){
        while(u){
            add(id[top[u]],id[u],1,n,1,0,c);
            u=fa[top[u]];
        }
    }
    void OPA(int u,int v){
        if(id[v]<id[u]||id[v]>id[u]+sz[u]-1){
            add(id[u],id[u]+sz[u]-1,1,n,1,1),sA+=sz[u];
            if(fa[u]) up(fa[u],sz[u]);
            return;
        }
        add(1,n,1,n,1,1),sA+=sz[1];
        if(u==v) return;
        u=way(u,v);
        add(id[u],id[u]+sz[u]-1,1,n,1,-1),sA-=sz[u];
        if(fa[u]) up(fa[u],-sz[u]);
    }
    void OPC(int u,int v){
        int lc=0,rc=0;
        while(top[u]!=top[v]){
            if(dep[top[u]]<dep[top[v]]) swap(u,v),swap(lc,rc);
            add(id[top[u]],id[u],1,n,1,-1,id[u]+lc+1),lc+=id[u]-id[top[u]]+1;
            u=fa[top[u]];
        }
        if(dep[u]>dep[v]) swap(u,v),swap(lc,rc);
        up(u,lc);
        add(id[u],id[v],1,n,1,-1,id[v]+rc+1),rc+=id[v]-id[u]+1;
        if(fa[u]) up(fa[u],rc);
        sA+=lc+rc;
    }
    ll OPQ(int u){
        ll res=sA*dep[u]+tr[1];
        while(u){
            res-=2*qry(id[top[u]],id[u],1,n,1);
            u=fa[top[u]];
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
    chinese_guys_solution::n = n;
    
    vector<pair<int, int>> tadj(n-1);
    for (auto &[x, y]: tadj) {
        cin >> x >> y;
        adj[x].push_back(y);
        adj[y].push_back(x);
        chinese_guys_solution::add_edge(x, y);
    }
    dfs(1, 1);
    lca::lcadfs(1, 1, 0);

    chinese_guys_solution::dfs(1, 0);
     chinese_guys_solution::dfs2(1, 1);
     chinese_guys_solution::build(1, n, 1);
    
    centroid_tree c(tadj);
G(q) 
    while(q--) {
        G(t) 
        
        if (t == 1) {
            G(u) G(v)
            if (u == v) c.update(1, 1);
            else {
                if (tin[v] <= tin[u] and tin[u] < tout[v]) {
                    // invert the query
                    c.update(1, 1);

                    FD(i, 20, -1) {
                        ll extra = lca::dep[u] - (lca::dep[v] + 1);
                        if (extra & 1<<i) u = lca::par[u][i];
                    }

                    c.update(u, -1);
                } else c.update(v, 1);
            }
        } else if (t == 2) {
            G(u) G(v)
            // continue;
            chinese_guys_solution::OPC(u, v);
        }
        else {
            G(x)
            ll res = c.query(x);
            // cout << res << endl;
            cout << res + chinese_guys_solution::OPQ(x) << endl;
        }
    }
}
