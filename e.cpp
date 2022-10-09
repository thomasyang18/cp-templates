#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")

#include<bits/stdc++.h>
#include<math.h>
using namespace std;

typedef long long int ll;
typedef long double ld;
typedef pair<ll, ll> pl;

#define K first
#define V second
#define G(x) ll x; cin >> x;
#define GD(x) ld x; cin >> x;
#define GS(s) string s; cin >> s;
#define EX(x) { cout << x << '\n'; exit(0); }
#define A(a) (a).begin(), (a).end()

#define NN 300010
#define M 1000000007 // 998244353


namespace seg {
	    typedef ll T;
	        T id=-1e18;
		    T f(T a, T b) {return max(a,b);}

		        T t[2 * NN];
			    ll n=NN;  // array size

			        void modify(ll p, T value) {  // set value at position p
					      for (p+=n, t[p] = value; p /= 2;) t[p] = f(t[2*p], t[2*p+1]);
					          }

				    T query(ll l, ll r) { // fold f on interval [l, r)
					          T resl=id, resr=id;
						        for (l += n, r += n; l < r; l /= 2, r /= 2) {
								        if (l&1) resl = f(resl, t[l++]);
									        if (r&1) resr = f(t[--r], resr);
										      }
							      return f(resl, resr);
							          }
}

vector<pl> ranges[NN];




int main(){
//    freopen("a.in", "r", stdin);
//    freopen("a.out", "w", stdout);

    ios_base::sync_with_stdio(false); cin.tie(0);
    cout << fixed << setprecision(20);

	G(n) G(m) G(k)

	vector<vector<ll>> p;
	vector<vector<ll>> dp;
	
	for (int i = 0; i < m; i++){
		p.push_back(vector<ll>());
		dp.push_back(vector<ll>());
		for (int j = 0; j < n; j++ ){ G(x)
			p[i].push_back(x);
			dp[i].push_back(p[i][j]);
		}
	}

	vector<pair<pl, ll>> intervals;

	while(k--){
		G(a) G(b) G(c)
		intervals.push_back({{a-1, 0}, c-1});
		intervals.push_back({{b-1,1}, c-1});
		ranges[c-1].push_back({a-1,b-1});
	}

	// m is days, n is cities

	ll ans = 0;

	// group the intervals (a,b) together on line dp[i], and then they all can point exactly to the position dp[i+1][c]

	sort(A(intervals));

	for (int i = m-2; i >= 0; i--){
		// build segtree
		for (int j = 0; j < n; j++) seg::modify(j, dp[i+1][j]);
		
		for (int j = 0; j < n; j++){
			dp[i][j] = max(dp[i][j], p[i][j] + dp[i+1][j]);
			if (ranges[j].size()){
				dp[i][j] = max(dp[i][j], p[i][j] + seg::query(ranges[j][0].first, ranges[j][0].second+1));
			}
		}

		multiset<ll> maxInInterval;
		int fp = 0;

		for (int j= 0 ; j < n; j++){
			while (fp < intervals.size() and intervals[fp].first.first <= j and intervals[fp].first.second == 0) {
				// increment
				maxInInterval.insert(dp[i+1][intervals[fp++].second]);
			}

			if (maxInInterval.size()) dp[i][j] = max(dp[i][j], p[i][j] + (*maxInInterval.begin()));

			while (fp < intervals.size() and intervals[fp].first.first <= j) {
				maxInInterval.erase(maxInInterval.find(dp[i+1][intervals[fp++].second]));
			}
		}

	}
	for (auto x: dp) for (auto y: x) ans = max(ans, y);

	cout << ans << '\n';
	

}
