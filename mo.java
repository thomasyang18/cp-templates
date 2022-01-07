import java.util.*;

public class mo {
	int n,sqrt;
	int[] arr,ans;
	Query[] q;
	int tot = 0;
	class Query implements Comparable<Query>{
		int l, r, i;
		public Query(int a, int b, int c) {l=a;r=b;i=c;}
		public int compareTo(Query o) {
			if (l/sqrt == o.l/sqrt) return l/sqrt %2 == 1 ? o.r-r: r-o.r;
			return l/sqrt-o.l/sqrt;
		}
	}
	void update(int idx, int delta){// THE MEAT OF THE CODE
	}
	void solve() {
		int cl = 0, cr= -1;
		for (Query query: q) {
			while (cl>query.l) update(--cl,1);
			while (cr<query.r) update(++cr,1);
			while (cl<query.l) update(cl++,-1);
			while (cr>query.r) update(cr--,-1);
			ans[query.i] = tot;
		}
	}
	void solve(long[] a, int[] l, int[]r) {
		n =  a.length;
		sqrt = max(1,n/(int)Math.sqrt(l.length)); arr = new int[n];
		for (int i = 0 ; i < n; i++) {
			arr[i] = (int)a[i];
		}
		q = new Query[l.length];
		ans = new int[l.length];
		for (int i = 0; i < l.length; i++) {
			q[i] = new Query(l[i],r[i],i);
		}
		Arrays.sort(q);
	}
}
