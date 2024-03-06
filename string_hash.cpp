
const ll NN = 1e6+10;
typedef pair<unsigned ll, ll> hsh;
 
#define M 1000000321
#define OP(x, y) constexpr hsh operator x (const hsh a, const hsh b) { return { a.K x b.K, (a.V y b.V + M) % M }; }
OP(+, +) OP(*, *) OP(-, + M -)
mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
uniform_int_distribution<ll> dist(256, M - 1);
 
const array<hsh, NN> p = []{
    array<hsh, NN> p;
    p[0] = { 1, 1 }, p[1] = { dist(gen) | 1, dist(gen) };
    F(i, 2, NN) p[i] = p[i-1] * p[1];
    return p;
}();

typedef pair<hsh, ll> comp_str; // compressed string 


comp_str combine(comp_str a, comp_str b) {
    return {a.K * p[b.V] + b.K, a.V + b.V};
}

struct shash {
    vector<hsh> h{{0, 0}};
    hsh sub_hash(ll l, ll len) {
        return h[l+len] - p[len]*h[l];
    }

    shash() {}

    shash(string s) {
        F(i, 0, s.size()) h.push_back(h[i]*p[1] + hsh{s[i], s[i]});
    }   

    void operator += (char c){
        h.push_back(h.back() * p[1] + hsh{c, c});
    }

    comp_str c_str(ll l, ll len) {
        return {sub_hash(l, len), len};
    }
};
