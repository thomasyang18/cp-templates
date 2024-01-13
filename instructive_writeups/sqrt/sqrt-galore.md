# A lot of good sqrt ideas in this problem.

Problem: https://www.codechef.com/problems/CLOSESTQ

Looking at 300iq's code.

## Sqrt idea: Batching Updates

Classic technique of batching updates, only sqrt(n) things change so you hope those are easy to update in total

## Classic Idea: "Sweepline" on when points are alive

A subset of points are alive for the entire interval, so we can treat those points as fully persistent.

There can only be O(Sqrt) changing points, call this set C.

## Cracked idea: "Sweepline" on when a candidate distance exists in your set

A point is alive for a certain time, that means there are some candidates for closest points for that point!

The weight of our sweepline segments is a closest distance candidate.

Let's look at the classes of closest distances.

    - Persistent X Persistent - Can be solved via classic d&c
    - Persistent X Changing - Sort of ugly; since MAXCORD is small, 
    maintain a brute-forcy precomp solution to find closest persistent point in O(C)

## Insane Idea: Changing X Changing - Just intersect all pairs of segments, add those to set!

We can pay to do all pairs brute force, and we can add them to the sweepline easily by intersecting the sets. 

I've never seen that idea before nor have I seen it after this problem, but that's a cracked idea.

## Classic Idea: When writing more to data structure than read, switch ds
    (in this case, we want to maintain min of all currently active segments with sqrt decomposition)

## Takeaway

SQRT cheese is really damn versatile. 

Some nice but obvious properties like:
    - There can only be O(SQRT(N)) batches, so we can pay O(n) time
    - O(SQRT(N) * SQRT(N)) = O(N)
    