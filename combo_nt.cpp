ll f[FN], fi[FN];
constexpr ll bpow(ll b, ll e) {
	ll r = 1; for (; e; e/=2) {
		if (e&1) r=r*b%M;
		b=b*b%M;
	} 
	return r;
}
constexpr ll inv(ll b) {
	return bpow(b, M-2);
}

ll ncr(ll n, ll k) {
    if (min(n, k) < 0) return 0;
    if (n < k) return 0;
    if (n == k || k == 0) return 1;
return f[n] * fi[n-k]%M * fi[k]%M;
}

ll sab(ll n, ll k) {
    return ncr(n+k-1, k-1);
}


	f[0] = 1;
	F(i, 1, FN) f[i] = f[i-1]*i%M;
 
    fi[FN-1] = bpow(f[FN-1], M-2);
    
    for (ll i = FN-2; i >= 0; --i)
        fi[i] = fi[i + 1] * (i+1) % M;



// large n, small k
ll ncr(ll n, ll k) {
	// static map<pl, ll> cache 
// 0c0 = 1, but sometimes they want non empty subsets 
    if (min(n, k) < 0) return 0;
    if (n < k) return 0;
    if (n == k || k == 0) return 1;
    if (k > n/2) return ncr(n, n - k);

    ll ans = 1;
    F(i, 1, k+1) ans = ans * inv(i)%M;
    F(i, n-k+1, n+1) ans = ans*(i%M)%M;
    return ans;
}

===========================================

// sieve 

void precalc(){
    static ll lst[NN];
	F(i, 0, NN) lst[i] = i;

	F(i, 1, NN) {
		for (ll j = i; j < NN; j+=i) factors[j].push_back(i);
	} 
    
    for (ll i = 2; i < NN; ++i){
        if (lst[i] == i) mu[i] = 1;
        for (ll j = 2 * i; j < NN; j += i){
            lst[j] = min(lst[j], lst[i]);
            if (lst[j] == lst[i])
                mu[j] = 0;
            else
                mu[j] = -mu[i];
        }
    }
}

============================================
