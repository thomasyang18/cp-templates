public class dsu {
	int[] p;
	public dsu(int n) {
		p = new int[n];
		for (int i = 0; i < n; i++) {
			p[i] = i;
		}
	}
	int find(int a) {return a == p[a] ? a: (p[a] = find(p[a]));}
	void merge(int a, int b) {p[find(a)] = find(b);}
	boolean check(int a, int b) {return find(a) == find(b);}
}
