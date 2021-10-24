---
title: Problem Solving with Nial - Nial Introduction, Chapter 5
---


# Problem Solving with Nial
### Chapter 5

- [The Problem](#the-problem)
- [Testing for a Path](#testing-for-a-path)
- [Returning the Path Found](#returning-the-path-found)
- [Finding Paths in a Cyclic Graph](#finding-paths-in-a-cyclic-graph)

Q'Nial provides an interactive environment for experimenting while
trying to solve a problem. The interactive approach allows thinking
about the problem by trying steps towards a solution.

This chapter consists of a log of an interactive session used to explore
a problem in graph theory. The recording of the log is started using

    set "log ;


## The Problem

Consider a directed graph consisting of nodes a, b, ... and edges that
indicate a directed link between some of the nodes. For example,
consider the following graph:

                      +---+
       +-------->-----| b |
       |              +---+
       |
     +---+      +---+     +---+     +---+
     | a |--->--| c |-->--| g |-->--| k |
     +---+      +---+     +---+     +---+
                                      |
     +---+                +---+       |
     | d |------------>---| e |--->---+
     +---+                +---+


One way to represent the graph is to provide a list of edges in which
each edge is a pair of nodes indicating a link from the first node to
the second.

    Edges := ("a "b) ("a "c) ("c "g) ("g "k) ("d "e) ("e "k)
    
    +-----------------------+
    |a b|a c|c g|g k|d e|e k|
    +-----------------------+
    

This array describes the graph completely.

Suppose the requirement is to write an operation that, given an edgelist
for a graph and two nodes Node1 and Node2, determines whether or not
there exists a path from Node1 to Node2 along the links in the graph.
Visual inspection of the example graph reveals that there exists a path
from a to k but not one from b to d. The task is to determine how to
test this finding computationally.

## Testing for a Path

We begin experimenting by looking for the path from a to k. First, look
for all the links that start at a.

         EACH first Edge
         
    a a c g d e
    
         EACH first Edges = "a
    o
    

Using *equal* returns only a single result. Try *match*.

         EACH first Edges match "a
         
    lloooo
    

This gives a bitstring, a string of booleans, that can be used with
*sublist* to extract the desired links.

        Lnksfrm_a := EACH first Edges match "a sublist Edges
        
    +-------+
    |a b|a c|
    +-------+
    

Then, test whether or not a direct path to k has been found.

         "k in EACH second Lnksfrm_a
         
    o

The answer is negative. Now repeat the process to see if there is a link
through b or c.

         Lnksfrm_b := EACH first Edges match "b sublist Edges
         
         Lnksfrm_c := EACH first Edges match "c sublist Edges
         
    +---+
    |c g|
    +---+
    

There are no links found from b and only one from c. Test to see if the
link from c completes a path.

         "k in EACH second Lnksfrm_c
         
    o
    

The result is negative so the process is repeated on the node to which c
links.

         Lnksfrm_g := EACH first Edges match "g sublist Edges
         
    +---+
    |g k|
    +---+
    

         "k in EACH second Lnksfrm_g
         
    l
    

A path has been found. This exploration indicates that a path test can
be written in this style. It will have two parts: a test to see if a
direct link occurs or the use of a loop to find whether or not one of
the links leads to a path.

    pathtest is OP Edges Node1 Node2 {
      Lnksfrm_node1 := EACH first Edges match Node1 sublist Edges;
      IF Node2 in EACH second Lnksfrm_Node1 THEN
        % direct path Node1 to Node2;
        True
      ELSE
        % loop through linked nodes to see if a path to Node2 exists;
        Linkednodes := EACH second Lnksfrm_node1;
        Found := False;
        I := 0;
        WHILE not Found and (I < tally Linked nodes) DO
          Node := Linkednodes@I;
          Lnksfrm_node := EACH first Edges match Node sublist Edges;
          Found := Node2 in EACH second Lnksfrm_Node;
          I := I + 1;
        ENDWHILE;
        Found
      ENDIF }
      

         pathtest Edges "a "k
    o

The test of this version shows that the algorithm fails.

         pathtest Edges "a "b
    l
    

         pathtest Edges "a "c
    l
    

         pathtest Edges "a "g
    l
    

Further testing shows that the algorithm is close to working. It finds a
path over two links but not over three. The problem is apparently in the
loop that looks for a path from a node in the list of linked nodes.

Instead of trying to test for a path directly, it is necessary to call
the routine recursively to get the path test to go down an arbitrary
number of links.

    pathtest is OP Edges Node1 Node2 {
      Lnksfrm_node1 := EACH first Edges match Node1 sublist Edges;
      IF Node2 in EACH second Lnksfrm_Node1 THEN
        % direct path from Node1 to Node2;
        True
      ELSE
        % loop through linked nodes to see if a path to Node2 exists;
        Linkednodes := EACH second Lnksfrm_node1;
        Found := False;
        I := 0;
        WHILE not Found and (I < tally Linked nodes) DO
          Found := pathtest Edges Linkednodes@I Node2;
          I := I + 1;
        ENDWHILE;
        Found
      ENDIF }

         pathtest Edges "a "k
    l
    

         pathtest Edges "b "d
    o
    

The operation has succeeded.


## Returning the Path Found

An alternative solution would be to return the path if one is found.
This is only a minor change in the algorithm. Instead of returning a
boolean value in the direct test, the link would be returned. In the
loop, an empty path would be set and it would be replaced by a found
path in the loop. The first attempt is:

    findpath is OP Edges Node1 Node2 {
      Lnksfrm_node1 := EACH first Edges match Node1 sublist Edges;
      IF Node2 in EACH second Lnksfrm_node1 THEN
        % direct path from Node1 to Node2;
        Node1 Node2
      ELSE
        % loop through linked nodes to see if a path to Node2 exists;
        Linkednodes := EACH second Lnksfrm_node1;
        Foundpath := Null;
        I := 0;
        WHILE empty Foundpath and (I < tally Linkednodes) DO
          Foundpath := findpath Edges Linkednodes@I Node2;
          IF not empty Foundpath THEN
             Foundpath := Linkednodes@I hitch Foundpath;
          ENDIF;
          I := I + 1;
        ENDWHILE;
        Foundpath
      ENDIF }
      

         findpath Edges "a "c
     a c
         findpath Edges "a "k
     c g g k
     

The operation works on a direct link but has a missing node and a
repetition in a path of length four. The problem is caused by the path
computation after the recursion has returned a path. It should be Node1
rather than the linked node that is added to the path.

    findpath is OP Edges Node1 Node2 {
      Lnksfrm_node1 := EACH first Edges match Node1 sublist Edges;
      IF Node2 in EACH second Lnksfrm_Node1 THEN
        % direct path from Node1 to Node2;
        Node1 Node2
      ELSE
        % loop through linked nodes to see if a path to Node2 exists;
        Linkednodes := EACH second Lnksfrm_node1;
        Foundpath := Null;
        I := 0;
        WHILE empty Foundpath and (I < tally Linkednodes) DO
          Foundpath := findpath Edges Linkednodes@I Node2;
          I := I + 1;
        ENDWHILE;
        IF not empty Foundpath THEN
          Foundpath := Node1 hitch Foundpath;
        ENDIF;
        Foundpath
      ENDIF }
      

         findpath Edges "a "k
    a c g k
    


## Finding Paths in a Cyclic Graph

Now consider the situation where a graph has more than one path between
two nodes. It may be desirable to compute all the paths between nodes.
Consider the graph which is the same as the previous one except that it
adds a link from a to d.

         Edges:=("a "b)("a "c)("c "g)("g "k)("d "e)("e "k)("a "d)
    +---------------------------+
    |a b|a c|c g|g k|d e|e k|a d|
    +---------------------------+
    

The modification needed is twofold. On a direct link, the result must be
returned as a solitary list holding the direct link to indicate one
existing path. For the linked nodes, the loop has to be changed to check
all of the nodes for paths and these have to be combined to give the
list of all paths.

    findpaths is OP Edges Node1 Node2 {
      Lnksfrm_node1 := EACH first Edges match Node1 sublist Edges;
      IF Node2 in EACH second Lnksfrm_Node1 THEN 
        % there is a direct path from Node1 to Node2;
        Paths := [ Node1 Node2 ];
      ELSE
        Paths := Null;
      ENDIF;
      % loop through linked nodes to add any additional paths;
      Linkednodes := EACH second Lnksfrm_node1;
      FOR Node WITH Linkednodes except [Node2] DO 
        Newpaths := findpaths Edges Node Node2;
        Paths := Paths link (Node1 EACHRIGHT hitch Newpaths);
      ENDFOR;
      Paths }
      

         findpaths Edges "a "k
    +---------------+
    |a c g k|a d e k|
    +---------------+
    

The algorithm worked on the first attempt.

The limits of the algorithm can be explored. Consider the test:

         Cyclic_graph := ("a "b)("b "c)("b "d)("c "a)
    +---------------+
    |a b|b c|b d|c a|
    +---------------+
    

         findpath Cyclic_graph "a "d
    system warning: C stack overflow
    

The algorithm fails with a stack overflow. The problem is caused by the
recursion going around the cycle in the graph until the internal stack
in Q'Nial runs out of space. The algorithm has an implicit assumption
that the chains of links will all terminate. The assumption would be
valid if it is known that the directed graph does not have cycles (i.e.
it is acyclic). To make the algorithm work in the general case, it needs
to be altered to avoid cycles. The approach is to add a fourth parameter
that records the nodes that have been encountered in pursuing a path and
to use it to avoid a recursion that cycles.

The expression calling *findpaths* uses Null as the initial value for the
new parameter Encountered. Encountered is a list which is extended when
a new node is considered. The list is used to remove can didates from
the list of linked nodes that have already been processed.

    findpaths is OPERATION Edges Node1 Node2 Encountered {
      Lnksfrm_node1 := EACH first Edges match Node1 sublist Edges;
      IF Node2 in EACH second Lnksfrm_Node1 THEN
        % direct path from Node1 to Node2;
        Paths := [ Node1 Node2 ];
      ELSE
        Paths := Null;
      ENDIF;
      Encountered := Encountered append Node1;
      % loop through linked nodes not encountered to add paths ;
      Linkednodes :=EACH second Lnksfrm_node1 except Encountered;
      FOR Node WITH Linkednodes except [Node2] DO
        Newpaths := findpaths Edges Node Node2 Encountered;
        Paths := Paths link (Node1 EACHRIGHT hitch Newpaths)
      ENDFOR }
      
 
         findpaths Cyclic_graph "a "d Null
    +-----+
    |a b d|
    +-----+
    

Exploring directed graph computations could continue in other ways. For
example, the use of Edges is only one way to represent a graph. Another
is to use a boolean table where a true in row i and column j indicates
that there is a link between nodes i and j. With this representation, a
correspondence between the node names and the indices is maintained.
