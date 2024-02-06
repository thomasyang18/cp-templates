edge centroid decomposition vs normal centroid decompoistion

- normal centroid decomposition stores along side it (centroid, bag of nodes that i care about that go up to centroid) 

it's precisely the nodes hanging off of one subtree of the centroid 

- if we want to compute f(v, i) for a fixed node v, for all nodes i quickly, centroid decomp lets you do that

with updates, the idea is that if only one subtree goes through an erratic change at every step, we can dfs down that. it's log(n) depth, so we win. 

the definition is intuitive to see

it's ad hoccy and sort of cancer to maintain in a nice way though. 

usually what we do is also store the relative contribution from one subtree to its parent, then hope recalculation of values is fast.

but often, it's not even limited by the operation; it's simply that recomputing contribution relative to parent is O(child size) at least. 

there are ways to get around this - f(max) => use sets or maps, diameter => only two subtrees at most matter, invertible operations = easy to do, or just straight up use segtree for the child nodes, which i guess is one way to do it...


btw, you have to subtract out the child contribution for operations s.t. if you allowed "doubling back" paths, the answer would change. 

like actually, 90% of issues just arises from the need to subtract out the child contribution, so annoying.

============================================================================

edge centroid decomposition

a headache to maintain and reason about , multiple canonical interpretations of what you want to do 

sort of clear, pie in the sky idea of what I want to maintain, but not really (e.g. i first calculate distances from centroid node to other nodes via a dfs, then later use lca garbage to find the distances directly??? like really thats so error prone, not to mention the like 4 ways im keeping truth values in the centroid tree)

PLUS THE EDGE CASE WHEN I GET DOWN TO A LEAF NODE!!! that shit is utter retardedness, we need a better way to reason about it 

my reworked implementation i think should now work for point updates if i finangle with it, since now if a node is a centroid, it is no longer passed on to its children, as it should be.



e.g. 

	- can we support path queries? (i don't know how)


what's nice about subtree and point queries is that no matter what interpetation, you're only going to visit log(n) subtrees

path queries are not like that 

so the idea is that 

====================

path queries are hard since there's no structured way to update a whole set of nodes fast

like with subtree updates, we can cheese it by using euler tour but path updates are hld by default idk 



yeah the issue is that we're cheesing subtree updates i guess? 


