#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")

#include<bits/stdc++.h>
#include<math.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;

#define G(x) ll x; cin >> x;
#define GD(x) ld x; cin >> x;
#define GS(s) string s; cin >> s;
#define EX(x) { cout << x << '\n'; exit(0); }
#define A(a) (a).begin(), (a).end()

#define M 1000000007 // 998244353

struct lseg{
    typedef ll T;
    typedef ll L;
    ll n;
    T idT = 0;
    L idL = -2e18;
    T combT(T a, T b){return a + b;}
    L combL(L a, L b){
        if (b == idL) return a;
        return b;
    }
    T applyL(T a, L b){
        if (b == idL) return a;
        return b;
    }
    vector<T> t;
    vector<L> lazy;

    lseg(ll len){
        n = 1;
        while (n < len) n*=2;
        t = vector<T>(n*2);
        lazy = vector<L>(n);
        for (int i = 0; i < n*2; i++) t[i] = idT, lazy[i/2] = idL;
    }

    void apply(ll v, L value){
        t[v] = applyL(t[v], value);
        if (v < n) lazy[v] = combL(lazy[v], value);
    }

    void build(ll p){
        while(p /= 2) t[p] = applyL(combT(t[2 * p], t[2 * p + 1]) , lazy[p]);
    }

    void push(ll p){
        for (ll h = 26; --h;) { // never hits zero so don't worry about leaf node bs
            ll v = p>>h;
            apply(2*v, lazy[v]);
            apply(2*v+1, lazy[v]);
            lazy[v] = idL;
        }
    }

    void update(ll l, ll r, L value) {
        // update on interval [l, r)
        // update whatever lazy segments intersect with L and r, and sort of recurse to the next layer
        // but try to do it w/o recursion cuz overhead moment
        l += n, r += n;
        ll l0 = l, r0 = r;
        for (; l < r; l/=2, r/=2){
            if (l&1){
                apply(l++, value);
            }
            if (r&1){
                apply(--r, value);
            }
        }
        // We call this because this range encapsulates all the updates we could possibly do on this range.
        build(l0), build(r0-1);
    }

    T query(ll l, ll r) { // fold f on interval [l, r)
        T resl=idT, resr=idT;
        l+=n, r+=n;
        // For a similar reason as build0, we push down all the updates to the nodes we could possibly visit.
        push(l), push(r-1);
        for (; l < r; l /= 2, r /= 2) {
            if (l&1) {
                resl = combT(resl, t[l++]);
            }
            if (r&1) {
                resr = combT(t[--r], resr);
            }
        }
        return combT(resl, resr);
    }
};
#define NN 1048576
namespace lseg2 {
    typedef ll T;
    typedef ll D;

    T idT = 0, t[2 * NN];
    D idD = 0, d[NN*2];
    ll n = NN;

    T f(T a, T b) { return a + b; }
    D g(D a, D b) { return a+b; }
    T h(D a, T b) {return a + b; }

    void apply(ll p, D v) {
        t[p] = h(v, t[p]);
        if(p < n) d[p] = g(v, d[p]);
    }

    void build(ll p) {
        while(p /= 2) t[p] = h(d[p], f(t[2 * p], t[2 * p + 1]));
    }

    void modify(ll l, ll r, D v) {
        ll l0 = (l += n), r0 = (r += n);
        for(; l < r; l /= 2, r /= 2) {
            if(l & 1) apply(l++, v);
            if(r & 1) apply(--r, v);
        }
        build(l0), build(r0-1);
    }

    void push(ll p) {
        for(ll s=26; --s; ) {
            ll i = p >> s;
            apply(2 * i, d[i]);
            apply(2 * i + 1, d[i]);
            d[i] = idD;
        }
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

void program1(){
    srand(30);
#define N 10
#define M 10
    ll a[N];
    for (int i = 0; i < NN*2; i++){
        lseg2::d[i] = lseg2::idD;
        lseg2::t[i] = lseg2::idT;

    }
    lseg *seg = new lseg(N);
    for (int i = 0; i < N; i++){
        ll x = rand()%M;
        a[i] = x;
       // seg->update(i,i+1,a[i]);
       lseg2::modify(i, i+1, a[i]);
    }
    for (int t = 0; t < N; t++){
        ll i = rand()%N;
        if (rand()%2){
            ll j = rand()%N;
            if (i > j) swap(i,j);
            ll x = rand()%M;
            for (int l = i; l <= j; l++) a[l] = x;
            //seg->update(i, j+1, x);
            lseg2::modify(i, j+1, x);
        }
        else{
            ll ans = a[i];

            cout << i << endl;
            for (int l = 0; l < N; l++) {
                if (i == l) cout << "," << a[l] << ", ";
                else cout << a[l] << " ";
            }
            cout << endl;

            // ll other = seg->query(i, i+1);
            ll other = lseg2::query(i, i+1);

            cout << ans << " " << other << endl;

            if (ans != other) {
                cout << "Nope\n";
                return;
            }
        }
    }

}

int main(){
    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(20);
    program1();
  // program2();
}
