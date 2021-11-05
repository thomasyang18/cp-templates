import java.util.*;
public class scc {
	static int n;
	static List<Integer>[] g, rg, adj;
	static int[] scc;
	static Stack<Integer> stack = new Stack<Integer>();
	
	static void dfs1(int i) {
		if (scc[i]++!=0)return;
		for (int x: g[i]) dfs1(x);
		stack.push(i);
	}
	static void dfs2(int i, int v) {
		if (scc[i] != -1) return;
		scc[i]=v;
		for (int x: rg[i]) dfs2(x,v);
	}
	
	static void solve() {
		scc = new int[n];
		Arrays.fill(scc, 0);
		for (int i = 0; i < n; i++) dfs1(i);
		Arrays.fill(scc, -1);
		int v = 0;
		while(!stack.isEmpty()) {
			int h =stack.pop();
			if (scc[h]==-1) dfs2(h,v++);
		}
		for (int i = 0; i < n; i++) {
			for (int x: g[i]) {
				if (scc[x] == scc[i]) continue;
				adj[scc[i]].add(scc[x]);
			}
		}
	}
}
