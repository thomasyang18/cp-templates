import static java.lang.Math.*;
import java.util.*;

public class segtree {
	// Iterative implementation, faster for non-lazy and easy to memorize
	public static class seg{
		int n = 1;
		
		long[] t;
		long id = 0;
		long f(long a, long b) {
			return a+b;
		}
		public seg(int len) {
			while (n<len) n*=2; 
			t = new long[n*2];
			Arrays.fill(t, id);
		}
		
		void update(int p, long val) { // addition on point
			for (t[p+=n] += val; (p/=2) != 0;) t[p] = f(t[p*2], t[p*2+1]);
		}
		
		long query(int l, int r) {	// [l,r] inclusive
			long resl = id, resr = id;
			for (l+=n, r+=n+1; l<r; l/=2, r/=2) {
				if (l%2==1) resl = f(resl, t[l++]);
				if (r%2==1) resr = f(t[--r], resr);
			}
			return f(resl,resr);
		}
	}
	
	
	
	// Recursive implementation with lazy, much cleaner
	
	public static class segt{
		long[] t, lazy;
		int n;
		void update(int l, int r, long a) {update(1, 0, n-1, l, r, a);}
		long query(int l, int r) {return query(1,0,n-1,l,r);}
		public segt(int len) {t = new long[(n=len)*4]; lazy = new long[n*4];}	
		
		void push(int v) {
			lazy[v*2+1] += lazy[v];
			lazy[v*2] += lazy[v];
			t[v*2] += lazy[v];
			t[v*2+1] += lazy[v];
			lazy[v] = 0;
		}
		
		void update(int v, int tl, int tr, int l, int r, long new_val) {
			if (l > r) return;
			if (l == tl && r == tr) t[v] += new_val;
			else {
				push(v);
				int tm = (tl+tr)/2;
				update(v*2, tl, tm, l, min(r, tm), new_val);
				update(v*2+1, tm+1,tr, max(l, tm+1),r, new_val);
				t[v] = t[v*2] + t[v*2+1];
			}
		}
		
		long query(int v, int tl, int tr, int l, int r) {
			if (l > r) return 0;
			push(v);
			if (l <= tl && tr <= r) return t[v];
			int tm = (tl+tr)/2;
			return query(v*2, tl, tm, l, min(r, tm)) + query(v*2+1, tm+1, tr, max(l, tm+1), r);
		}
	}
}
