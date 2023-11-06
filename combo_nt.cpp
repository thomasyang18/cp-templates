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
