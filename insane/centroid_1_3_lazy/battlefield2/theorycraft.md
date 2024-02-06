# Approach

The query is sum across all dist(v, i) * A_i

Centroid_tree should then store dist(centroid, j) * A_j for all A_j that it cares about? 

Then, the answer is simply 

dist(centroid, j) * A_j = V 

+ (sum A_subtree) * dist(v, centroid)

And we'll hit every subtree once, yup, yup.

So this should also support

- Range add to subtree
- 




# there's probably some stuff where if you consider just the nodes in the current universe, there's only so many valid node_indexes to be had, so we can store distances instead of recalculating with lca every time 