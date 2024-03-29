/*
300iq editorial for https://discuss.codechef.com/t/closestq-editorial/14517

(there exists queryseg solution that does the maintaining straight up for QlogQ * C, but it's slower and less instructive imo)


*/

#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")

#include<bits/stdc++.h>
#include<math.h>
using namespace std;

typedef int ll;
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

const ll block_sz = 1024;
const ll MX = 500;
const ll N = 2e6 + 7;
constexpr ll INF = 1e9;

ll cost(ll x, ll y) { return (x-y)*(x-y); }

struct maintain_min {
    static constexpr ll dist_sz = 1024; 
    ll sum[dist_sz] = {};
    ll cnt[dist_sz*dist_sz] = {};
    void upd(ll x, ll delta) {
        sum[x / dist_sz]+=delta;
        cnt[x]+=delta;
    }
    ll get()
    {
        F(i, 0, dist_sz) if (sum[i]) {
            for (ll j = i * dist_sz; ;++j) if (cnt[j]) return j;
        }
        return INF;
    }
};

struct pt{ll x, y;};
ll pdist(const pt& a, const pt& b) {
    return cost(a.x, b.x) + cost(a.y, b.y);
}

bool operator < (const pt &a, const pt &b){
    return (a.y != b.y ? a.y < b.y : a.x < b.x);
}

namespace closest_points {
    pt a[N], t[N];


    ll rec(ll l, ll r)
    {
        ll res = INF;
        if (r - l <= 3) {
            F(i, l, r+1) F(j, i+1, r+1) res = min(res, pdist(a[i], a[j]));
            sort(a + l, a + r + 1);
            return res;
        } 

        ll m = (l + r) / 2;
        ll mid = a[m].x;
        res = min(rec(l, m), rec(m+1, r));
        // Potentially bad to replace this. 
        // Have to understand what this loop is doing later? Or is this just classic d&c? 
        merge(a + l, a + m + 1, a + m + 1, a + r + 1, t);
        copy(t, t + r - l + 1, a + l);
        ll tsz = 0;
        F(i, l, r+1) if (cost(a[i].x, mid) < res) {
            for (ll j = tsz - 1; j >= 0 && cost(a[i].y, t[j].y) < res; --j) {
                res = min(res, pdist(a[i], t[j]));
            }
            t[tsz++] = a[i];
        }
        return res;
    }


    ll find_closest(vector <pt> p)
    {
        // sorts by x axis, default comp sorts by y axis for merging
        sort(p.begin(), p.end(), [] (const pt &a, const pt &b)
        {
            if (a.x == b.x)
            {
                return a.y < b.y;
            }
            else
            {
                return a.x < b.x;
            }
        });
        copy(A(p), a);
        return rec(0, p.size()-1);
    }
}

ll main(){
    vector<pl> ev;
    G(q)
    vector <int> die(q, q);
    vector<pt> a(q);
    {
        map <pair <int, int>, int> ind;
        F(i, 0, q) {
            string c; ll x, y;
            cin >> c >> x >> y;
            if (c == "+") {
                ev.emplace_back(1, i);
                ind[{x, y}] = i;
                a[i].x = x, a[i].y = y;
            } else {
                ev.emplace_back(-1, ind[{x, y}]);
                die[ind[{x, y}]] = i;
            }
        }
    }
    vector<vector<pl>> gr(q);

    for (ll i = 0; i < q; i += block_sz)
    {
        vector<pt> cur;
        ll st = i, en = min(q, i + block_sz);
        
        bool vis[MX+1][MX+1] = {};
        ll close[MX+1][MX+1][2]; // closest left node if f == 0 else right node
        
        // if i persist this entire block
        F(j, 0, st) if (die[ev[j].V] >= en) {
            cur.push_back(a[ev[j].V]);
        }

        F(i, 1, MX+1) F(j, 1, MX+1) vis[i][j] = false;
        for (auto [x, y] : cur) vis[x][y] = true;

        F(i, 1, MX+1) {
            ll lst = -1;
            F(j, 1, MX+1) {
                if (vis[i][j]) lst = j;
                close[i][j][0] = lst;                
            }
            lst = -1;
            FD(j, MX, 0) {
                if (vis[i][j]) lst = j;
                close[i][j][1] = lst;
            }
        }
        
        // Sets up sweepline of alive DISTANCES. 
        for (ll i = st; i < en; i++) {     
            bool c1 = ev[i].K == 1;
            bool c2 = ev[i].K == -1 && ev[i].V < st;
            if (!c1 and !c2) continue;
            // relatively expensive computation we'd like to avoid if possible

            auto [x, y] = a[ev[i].V]; 
            ll dist = INF; 
            F(i, 1, MX+1) F(t, 0, 2) if (~close[i][y][t]) 
            dist = min(dist, cost(i, x) + cost(y, close[i][y][t])); 
            if (dist == INF) continue; // currently processed point was not close to any existing points

            // sure, just matches up the alive intervals correctly
            if (c1) {
                ll lul = min(en, die[i]);
                gr[i].emplace_back(1, dist);
                if (lul != en) gr[lul].emplace_back(-1, dist);
            } else {
                gr[st].emplace_back(1, dist);
                gr[i].emplace_back(-1, dist);
            }
        }
        
        /*
            Every pair of intervals
            basically forms another interval of distances
        */
        F(j, st, en) F(k, j+1, en) 
            if (ev[j].V != ev[k].V) {
                ll _st = max({st, ev[j].V, ev[k].V});
                ll _en = min({en, die[ev[j].V], die[ev[k].V]});
                if (_st > _en) continue;
                auto [xj, yj] = a[ev[j].V];
                auto [xk, yk] = a[ev[k].V];
                ll dist = cost(xj, xk) + cost(yj, yk);
                gr[_st].emplace_back(1, dist);
                if (_en != en) gr[_en].emplace_back(-1, dist);
            }

        // Process sweepline 
        ll rlx = closest_points::find_closest(cur);
        maintain_min ds;
        for (ll i = st; i < en; i++)
        {
            for (auto [delta, x] : gr[i]) ds.upd(x, delta); 
            gr[i].clear();
            ll res = min(ds.get(), rlx);
            cout << (res == INF ? 0: res) << endl;
        }
    }
}

