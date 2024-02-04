
vector<pl> tree[N];

#define L 20
ll dep[N], par[N][L], distFromRoot[N];
ll tin[N], tout[N];
void dfs(ll i, ll p, ll pw) {
    static int time = 0;
    dep[i] = dep[p] + 1;
    par[i][0] = p;
    distFromRoot[i] = pw;
    tin[i] = time++;
    F(l, 1, L) {
        par[i][l] = par[par[i][l - 1]][l - 1];
    }
    for(auto [j, w]: tree[i]) if(j - p) dfs(j, i, pw + w);
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

typedef pl T;

    typedef ll U;
// combining segtree nodes a and b
    
pl f(pl a, pl b) {
    if (a.K == b.K) return min(a, b);    
    return max(a, b); 
}

 // applying updates a and b (in that order)
    U g(U b, U a) { return a + b; }
    // applying update b to segtree node a
    T h(U b, T a) { 
        a.K += b;
        return a;
     }

struct lztree {

    ll n;

    T idT = {-1e18, -1};
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

    vl mytins;
    lztree(vector<pl> pts) {
        assert(pts.size());
        n = pts.size();
        mytins.resize(n);
        t.resize(2*n, idT);
        d.resize(n, idU);
        F(i, 0, pts.size()) {
            mytins[i] = tin[pts[i].K];
            t[i+n] = pl(pts[i].V, pts[i].K);
        }
        FD(i, n-1, -1) t[i] = f(t[i*2], t[i*2+1]);
    }
};