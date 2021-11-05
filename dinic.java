import java.util.*;
import static java.lang.Math.*;
public class dinic{
	List<Edge>[] g;
	long INF = Long.MAX_VALUE/10;
	int[] l, next;
	int S = 0;
	public dinic(int s) { // s is the preicse number of nodes.
		S = s+10; // Let S be the index of the source, and T =S+1 be the index of the of the sink.
		g = new List[S+2];
		l = new int[S+2];
		next = new int[S+2];
		for (int i =0 ; i < g.length; i++) {g[i] = new ArrayList<Edge>();}	
	}
	long solve() {
		long maxflow = 0;
		while (l[S+1] != -1) {
			ArrayDeque<Integer> q = new ArrayDeque<Integer>();
			q.add(S);
			Arrays.fill(l, -1);
			l[S] = 0;
			while (!q.isEmpty()) {
				int h = q.poll();
				for (Edge e:g[h]) {
					if (e.remCap() > 0 && l[e.i] == -1) {
						l[e.i] =  l[h]+1;
						q.add(e.i);
					}
				}
			}
			Arrays.fill(next, 0);
			for (long f = dfs(S, INF); f != 0; f = dfs(S, INF)) maxflow += f;
		}
		return maxflow; 
	}
	class Edge{
		int i, flip; // Flip is the opposite edge. Accessible via g[e.i][e.flip].
		long cap, flow; // max and cur
		public Edge(int w, long x, long y, int res) {
			i=w;
			cap=x;
			flow=y;
			flip = res;
		}
		public long remCap() {return cap-flow;}
		public void augment(long flo) {flow += flo;g[i].get(flip).flow-=flo;}
	}
	
	void addEdge(int a, int b, long cap) {
		g[a].add(new Edge(b, cap, 0, g[b].size()));
		g[b].add(new Edge(a, 0, 0, g[a].size()-1));
	}
	
	long dfs(int i, long flow) {
		if (i == S+1 || flow == 0) return flow;
		for(;next[i] < g[i].size(); next[i]++) {
			Edge e= g[i].get(next[i]);
			if (e.remCap() > 0 && l[i] + 1== l[e.i]) {
				long bottleNeck = dfs(e.i, min(flow, e.remCap()));
				if (bottleNeck > 0) {
					e.augment(bottleNeck);
					return bottleNeck;
				}
			}
		}
		return 0;
	}
}