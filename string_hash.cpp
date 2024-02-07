
typedef pair<unsigned ll, ll> hsh;
 
#define K first
#define V second
#define M 1000000321
#define OP(x, y) constexpr hsh operator x (const hsh a, const hsh b) { return { a.K x b.K, (a.V y b.V + M) % M }; }
OP(+, +) OP(*, *) OP(-, + M -)
mt19937 gen(chrono::steady_clock::now().time_since_epoch().count());
uniform_int_distribution<ll> dist(256, M - 1);
 
ll ans[NN];
hsh p[NN], h[NN];
 
 
hsh sub_hash(ll l, ll len) {
    return h[l+len] - p[len]*h[l];
}


    GS(s)
 
    p[0] = { 1, 1 }, p[1] = { dist(gen) | 1, dist(gen) };
    
    for (ll i=0; i<s.size(); ++i) {
	    p[i+1] = p[i]*p[1];
	    h[i+1] = (h[i]*p[1] + hsh{s[i], s[i]});
	}
 
