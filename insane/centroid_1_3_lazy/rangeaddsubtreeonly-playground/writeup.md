# What is the problem?

- Range add Y to all the nodes in a subtree X 
- Compute sum of dist(v, i) * A_i for all nodes I 

This experiment should prove that 

- We can handle non-idempotent operations, implying that we count contribution from every node exactly once 
    - (excluding query vertex = centroid shenanigans, we MAY double count there. That is probably the biggest problem with the current impl).
- We can handle more types of operations

# Approach

The query is sum across all dist(v, i) * A_i

Centroid_tree should then store dist(centroid, j) * A_j for all A_j that it cares about?

Then, the answer is simply

dist(centroid, j) * A_j = V

+ (sum A_subtree) * dist(v, centroid)

So we need to constantly maintain sum of A values in subtree, and the value V. 

Here, the interesting thing is that we don't actually need lazy segtree at all???? 

All we do is store the distances from centroid to whatever, then when we want to increment V and sum of A, it's trivial... all we need is 
number of nodes we're affecting to update sum of A_subtree, and the sum of distances for dist(centroid, j). Lol? wtf??? 

The key difference is this is sort of like "normal" centroid decomp, where our data structure doesn't actually store the universe itself, but the universe can be easily calculated and recalculated with just an aid of a simple ds. 

# Before running the experiment

I suspect that if we find a way to generalize path queries on top of this, we'll become unstoppable. Arbitrary range subtree updates, arbitrary path updates, then centroid-tree like queries? Pretty poggers if you ask me. 


# Results

Okay so got a working implementation.

To handle https://codeforces.com/gym/102979/problem/A in particular, we can process subtrres and centroids seperately since contribution from addition is nice. 

