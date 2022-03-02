// combine joe and akif template, min segtree with addition atm
namespace lseg {
    
    typedef ll T;
    typedef ll D;

    T idT = 1e18, t[2 * NN];
    D idD = 0, d[NN];
    ll n = (fill_n(d, NN, idD), NN);
    
    // Applies right value to left value
    T f(T a, T b) { return min(a,b); }
    T g(T a, D b) { return a + b; }
    D h(D a, D b) { return a + b; }

    
    // EDIT ABOVE ONLY //
    
    void apply(ll p, D v) {
        t[p] = g(t[p], v);
        if(p < n) d[p] = h(d[p], v);
    }

    void push(ll p) {
        for(ll s=26; --s; ) {
            ll i = p >> s;
            apply(2 * i, d[i]);
            apply(2 * i + 1, d[i]);
            d[i] = idD;
        }
    }

    void modifyP(ll p, T v) {
        if(p < n) { push(p += n); t[p] = v; }
        while(p /= 2) t[p] = g(f(t[2 * p], t[2 * p + 1]), d[p]);
    }

    void modifyR(ll l, ll r, D v) {
        ll l0 = (l += n), r0 = (r += n);
        for(; l < r; l /= 2, r /= 2) {
            if(l & 1) apply(l++, v);
            if(r & 1) apply(--r, v);
        }
        modifyP(l0, idT), modifyP(r0 - 1, idT);
    }

    T query(ll l, ll r) {
        l += n, r += n;
        push(l), push(r - 1);
        T resL = idT, resR = idT;
        for(; l < r; l /= 2, r /= 2) {
            if(l & 1) resL = f(resL, t[l++]);
            if(r & 1) resR = f(t[--r], resR);
        }
        return f(resL, resR);
    }
}
