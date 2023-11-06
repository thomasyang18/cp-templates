ll f[NN*2], fi[NN*2];
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

// large n, small k
ll ncr(ll n, ll k) {
    if (min(n, k) < 0) return 0;
    if (n < k) return 0;
    if (n == k || k == 0) return 1;

    ll ans = 1;
    F(i, 1, k+1) ans = ans * inv(i)%M;
    F(i, n-k+1, n+1) ans = ans*(i%M)%M;
    return ans;
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
	F(i, 1, NN) f[i] = f[i-1]*i%M;
	F(i, 0, NN) fi[i] = inv(f[i]);

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
