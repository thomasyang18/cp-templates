#pragma GCC target ("avx2")
#pragma GCC optimize ("O3")
#pragma GCC optimize ("unroll-loops")

#include<bits/stdc++.h>
#include<math.h>
using namespace std;

typedef long long int ll;
#define NN


int main(){
	ios_base::sync_with_stdio(false); cin.tie(0);
  
}

// itereate through a bitmask efficiently
ll bm; // some bitmask to iterate through
for (ll tbm = bm; tbm > 0; tbm = tbm&(tbm-1)){
  ll i = __builtin_ctz(tbm&~(tbm-1)); // i is index, 1<<i is the mask pos as usual
}
