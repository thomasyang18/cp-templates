# What is the problem?

We have two types of operations on a tree.

1. (x, v) => Add V to all nodes in the subtree rooted at X, relative to 1.
2. (x) => Compute max(dist(x, y) - a[y])  

# The trick

Take any rooted subtree and any connected subgraph of the tree.

Fix it at some important node (let's just say it's the centroid, but this analysis will hold for all nodes)

Only these four cases can happen:

1. Full overlap
2. No overlap
3. One subtree has a partial overlap, and the rest have a full overlap.
4. One subtree has a partial/full overlap, and the rest have no overlap.

Crucially, if we're doing centroid decomposition, we can just ignore every subtree except one, which potentially needs to be fixed.

# The issue 

But with standard centroid decompoistion on vertices, a centroid node can have many children.

So let's instead focus on edge centroid decomposition. Notice that we "gather" multiple types of subtrees into one, 
but this is fine since in cases 3 and 4, only one subtree is the odd one out (i.e. it only "pollutes" one of 
the children at most, not both. The other child is still easy to update).

# Lazy tags

So one child we recurse one, while the other we either will ignore or add a lazy tag, saying "everything in this subgraph is marked". 

Well, this is just lazy propogation! 

# My implementation 

The main reason I have to do this scuffed implementation is because when doing centroid decomposition, you have to really avoid repeating edges. 

For example, dynamic diameter requires a segment tree to avoid double dipping into the same subtree twice. Which works, but is ugly... plus does it even work? 
Is it even possible to calculate a path from centroid X -> centroid Y - > Z? Probably, but it's annoying. Going through 2 centroids sucks.

So edge decomposition should in theory solve that. It's very nice, since the universe of nodes is firmly disjoint, we just have them point to the parent centroid. 

Every node (besides the root) has a lazy segtree of nodes representing its universe, and the whole idea is that the two sets (aside from the centroid) have disjoint nodes, so I can just query into the other guy, if I know the other side doesn't contain me inside of itself. 

Then past a certain threshold, I also just store a subtree directly, where you can ask directly the value to the node without going through any centroids.

So basically there's two forms of queries

costFunction(x, centroid) + precalculated cost of (centroid) to all other nodes in subtree

and

costFunction(x, y) + cost[y], i.e. just doing it directly

# Extensions

Right now, I don't really have a good grasp on the type of operations this should support.

Like the function is max(dist(x, y) - a[y]) across all nodes y, given a starting node x. 

The second query is literally just uh the function directly.

The firsrt query only works because max(dist(x, y) - a[y]) = dist(x, centroid) + max(dist(centroid, y) - a[y]) 

So the type of operations this even works under is really specific...

Is it even worth trying to generalize this? 

It's way too specific to this problem...

## Ideas

- Can we augment with point updates, maybe? This is probably #1 idea on my mind, I want this to be some kind of black box that can handle a lot of different types of vertex updates.
- Can we handle edge updates fast (probably not)? 




Like, we should theoretically be able to move this to solve dynamic diameter really easily, i think??? idk maybe not

# Pitfalls

- The above problem can apparently be solved just with HLD :/ 