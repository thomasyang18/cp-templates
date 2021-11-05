import java.util.*;
public class sat2 {
	List<Integer>[] g, rev;
	int[] scc;
	Stack<Integer> ord = new Stack<Integer>();
	public sat2(int len) {
		g = new List[len*2];
		rev = new List[len*2];
		scc = new int[g.length];
		for (int i = 0; i < g.length; i++) {g[i] = new ArrayList<Integer>();rev[i] = new ArrayList<Integer>();}
	}
	int ind(boolean sat, int v) {return v + (sat ? 0:g.length/2);}
	void add(boolean bx, int x, boolean by, int y) {
		p(!bx,x,by,y);
		p(!by,y,bx,x);
	}
	void p(boolean bx, int x, boolean by, int y) {
		g[ind(bx,x)].add(ind(by,y));
		rev[ind(by,y)].add(ind(bx,x));
	}
	boolean[] solve() {
		for (int i = 0; i < g.length; i++) dfs(i); 
		Arrays.fill(scc, 0);
		while (!ord.isEmpty()) dfs2(ord.pop(),1+g.length-ord.size());
		boolean[] res = new boolean[g.length/2];
		for (int i = 0; i < res.length; i++) {
			if (scc[i] == scc[i+res.length]) return null;
			if (scc[i] > scc[i+res.length]) res[i] = true;
			else res[i] = false;
		} 
		return res;
	}
	void dfs2(int v, int c) {
		if (scc[v] > 0) return;
		scc[v] = c;
		for (int o: rev[v]) dfs2(o,c);
	}
	void dfs(int v) {
		if (scc[v]++ > 0) return;
		for (int o: g[v]) dfs(o);
		ord.push(v);
	}
}