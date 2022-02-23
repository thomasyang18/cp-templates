public class string_hash {
	static long pow = (long)(Math.random()*M/2) + 500;
	static long M = (long)1e9+7;
	long[] h,p,h2,p2;
	public string_hash(char[] in) {
		h = new long[in.length+1];
		h2 = new long[in.length+1];
		p = new long[in.length+1];
		p2 = new long[in.length+1];
		p[0] = p2[0] = 1;
		for (int i = 0; i < in.length; i++) {
			p[i+1] = (p[i] *pow) %M;
			p2[i+1] = p2[i]*pow;
			h[i+1] = (h[i] * pow + in[i])%M;
			h2[i+1] = h2[i] * pow + in[i];
		}
	}
	public long hash(int l, int r) {
		long val = (h[++r] - p[r-l]*h[l] % M + M) %M;
		long val2 = h2[r] - p2[r-l]*h2[l];
		return val * M + val2;
	}
	public boolean query(int l, int r, int l2, int r2) {return hash(l,r) == hash(l2,r2);}	
}
