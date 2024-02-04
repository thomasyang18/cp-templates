#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")

#include<bits/stdc++.h>
#include<math.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;
typedef vector<ll> vl;
#define FD(i, r, l) for(ll i = r; i > (l); --i)

#define K first
#define V second
#define G(x) ll x; cin >> x;
#define GD(x) ld x; cin >> x;
#define GS(s) string s; cin >> s;
#define EX(x) { cout << x << '\n'; exit(0); }
#define A(a) (a).begin(), (a).end()
#define F(i, l, r) for (ll i = l; i < (r); ++i)

#define NN
#define M 1000000007 // 998244353

void prufer(int n){
    srand(time(NULL));
	map<int, int> not_leaves;
	set<int> leaves;
	vector<int> code;
	for (int i = 1; i <= n-2; i++) {
        int x = rand()%n + 1;
        not_leaves[x]++;
		code.push_back(x);
	}
	for (int i = 1; i <= n; i++) if (!not_leaves.count(i)) leaves.insert(i);
 
	for (int x: code) {
		assert(leaves.size());
		assert(not_leaves.count(x));
		cout << x << " " << *leaves.begin() << '\n';
		leaves.erase(leaves.begin());
		not_leaves[x]--;
		if (!not_leaves[x]) {
			not_leaves.erase(x);
			leaves.insert(x);
		}
	}
 
	assert(leaves.size() == 2);
	cout << *leaves.begin() << " " << *leaves.rbegin() << endl;
}

int main(){
//    freopen("a.in", "r", stdin);
//    freopen("a.out", "w", stdout);

    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(20);
    // int n = 1e5, q = 1e5;
    G(n) G(q)
    // cout << n << " " << q << endl;
    // F(i, 0, n) cout << rand()%M + 1 << " \n"[i==n-1];
    cout << n << " " << q << endl;

    prufer(n);


    F(i, 0, q) {
        int t = rand()%2 + 1;
        int u = rand()%n + 1;
        int v = rand()%n + 1;
        if (t == 1) cout << t << " " << u << " " << v << endl;
        else cout << t << " " << u << endl;
    }
}

