       #=======================================#
       #  bec v2.2                             #
       #  Copyright (C) 2025-2026 Bruno Gaume  #
       #=======================================#

1.DESCRIPTION
=============
Networks are one of the main conceptual structures for modeling complex systems, where
nodes represent the basic lowest-level entities of the system and edges represent their
interactions. Graph clustering algorithms are then used to identify modules: over-densely
connected sets of nodes of these networks as representations of entities at higher scales
of description in these systems.

bec is a C++ graph clustering program that finds the entities of a network with a hand on
the description scale within the nPnB framework.

2.REFERENCES
============
**nPnB:**
    B. GAUME, I. ACHITOUV, AND D. CHAVALARIAS: *Two antagonistic objectives for one multi-scale graph clustering framework*
    Nature Scientific Reports, 15 (2025), p. 13368.
    https://hal.science/hal-05046050v1

**bec:**
    B. GAUME: *Find Optimal clusterings in nPnB framework is hard, BEC.2: an algorithm to find high-performing ones*
    Forthcoming (2026)

**description scales:**
    B. GAUME: *Defining the appropriate scales for describing a complex system*
    Forthcoming (2026)

3.AUTHOR & COPYRIGHT
====================
This program is copyright (C) 2025-2026 by Bruno Gaume:
    Email:	gaume.bruno@gmail.com

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation;

This program is distributed in the hope that it will be useful,
but WITHOUT ANY GUARANTEE OF ANY KIND.

4.COMPILING
===========
To compile the program type the command "make".

5.USAGE
=======
command line usage:
```bec [scaleP] [input_graph_file] [output_clustering_file] [options]```

scaleP:
```The description scale in [0,1] for clustering by partition (see The nPnB framework REFERENCE).```

input_graph_file:
    ```The file in which the network is stored (see INPUT GRAPH FORMAT).```

output_clustering_file:
    ```The file in which the community structure will be stored (see OUPUT CLUSTERING FORMAT).```

**options:**
    
    -ox	set  scaleO to x in [0,1]. The stickiness scale, overlap amount for the gluant extension of the partition modules into overlapping modules. (default = "no" do not extend to save time).

    -ex	set  epsilon to x in [0,1]. The smaller epsilon, the better the quality of the output clustering, but the slower the computing. (default = 0.01).

    -rx	set  rankedEdeges to x. (default = '0', the edges are not ranked in the input graph, the program will take care of it).
    
    -zx	set  clustZero to x. Read the INITIAL PARTITION to amend from the x file (default = '0', the programm will take care of it: each node in its own module)
    
    -vx	set  verbose to x.  Display the progress. (default = '0', silent)

**example** of command line:
   ./bin/bec 0.49 ./GraphExample.txt ./clustOutput.txt -o0.1 -e0.01 -r0 -z0

6.INPUT GRAPH FORMAT
==================
The format is a list of undirected weighted edges.
The vertices must be encoded as consecutive integers starting from 0.
Each line contains (separated by tabulations) two vertices and a POSITIVE weight that define a weighted edge.
A comment line starts with "#" and empty lines are ignored.

**EXAMPLE:**
```# name=tesselation_automate
0	2	1
0	4	1
4	5	1
2	3	1
0	1	1
1	8	1
# the line above mean that there is an edge between 1 and 8 (with weight 1)

1	4	1
1	3	0.7
# the line above mean that there is an edge between 1 and 3 (with weight 0.7)

3	8	1
5	8	1
6	8	1
5	6	1
6	7	1
```

7.OUPUT CLUSTERING FORMAT
=========================
Each line starting with m\t defines a module of the community structure, the other lines are ignored.

Each module m consists of:
    m.p:  a partition module;
    m.ext:  the extension of m.p ((m.p ∩ m.ext)=∅  and (m.p U m.ext) is a module allowing overlaps).

The character ! is necessary to separate these two set of nodes m.p and m.ext:
    before !: the partition module m.p (nodes are separated by tabulations);
    after !: the extension m.ext (nodes are separated by tabulations).

**EXAMPLE:**
```// output community structure
m	0	1	2	3	4	!   // Partition module m0 = {0,1,2,3,4} and has an empty extension
m	5	6	8	!   // Partition module m1 = {5,6,8} and has an empty extension
m	7	!	6   // Partition module m2 = {7} and has the extention {6}
// The 3 lines above define:
//    A clustering by partition: {{0,1,2,3,4}, {5,6,8}, {7}}, (three non-overlapping entities)
//    A clustering allowing overlaps: {{0,1,2,3,4}, {5,6,8}, {7,6}} (three entities with allowed overlap)

// computing context
date	2026-05-06 11:46:43 // the computing date
bec 0.49 ./data/GraphExample.txt ./data/clustOutput.txt -o0.1 -e0.01 -r0 -z0  // the line command

// values ​​of the inputs
input_InfileGraph	./data/GraphExample.txt // input file graph
input_OutfileClust	./data/clustOutput.txt // output file clustering
input_RankedEdeges	0 // input RankedEdeges used for computing the community structure
input_ClustZero	0 // input ClustZero used for computing the community structure
input_sp	0.49 // input description scale used to construct Cp the clustering by partition
input_so	0.1 // input stickiness scale, overlap amount used to extend Cp modules
input_epsilon	0.01  // input epsilon used for computing the community structure

// properties of the input graph
gnbv	9   // The number of vertices in the graph that has been clustered
gnbe	13 // The number of vertices in the graph that has been clustered
density	0.3611111111 // edge density

// time computing
secondsP	0   // Time computing for partition
secondsO	0   // Time computing for the extensions

// properties of the output community structure
|C|	3   // The number of modules

// Below is the intrinsic quality of the clustering by partition within the nPnB framework (see the nPnB framework REFERENCE)
omega_p	1.000000    // omega_p = sum(|adherent(m)|) for m in Cp (a node i is adherent to a module m if i ∉ m and i is a neighbor of all the nodes of m)
TP_p	8.700000    // True Positive
TN_p	18.561538   // True Negative
FP_p	3.907692    // False Positive
FN_p	4.000000    // False Negative
P_p	0.690055        // Precision
R_p	0.685039        // Recall
F0.5_p	0.687538    // Fscore with sigma=0.5 (with sigma=0.5, Precision and Recall are of the same importance)
Fsp_p	0.687617    // Fscore with sigma=sp (where sp is the scale of description that was used for partition clustering. Here sp=0.49)

// Below is the intrinsic quality of the clustering allowing overlaps within the nPnB framework (see the nPnB framework REFERENCE)
omega_o	0.000000    // omega_o = sum(|adherent(m)|) for m in Co (a node i is adherent to a module m if i ∉ m and i is a neighbor of all the nodes of m)
TP_o	9.700000    // True Positive
TN_o	18.561538   // True Negative
FP_o	3.907692    // False Positive
FN_o	3.000000    // False Negative
P_o	0.712832        // Precision
R_o	0.763780        // Recall
F0.5_o_	0.737427    // Fscore with sigma=0.5 (with sigma=0.5, Precision and Recall are of the same importance)
Fsp_o	0.736629    // Fscore with sigma=sp (where sp is the scale of description that was used for partition clustering. Here sp=0.49)
```

8.INPUT CLUSTERING FORMAT (if option -zx with x≠0)
==================================================
With option    -zClustZero.txt:
    The input format clustering is the same that the output format clustering.
    Each line starting with m\t defines a module of the community structure, the other lines are ignored.
    The INITIAL partitioning to amend will be the clustering by partition defined in ClustZero.txt (the partition modules, before !).

9.COMMENTS & BUG REPORT
=======================
If you find a bug, please send a bug report to gaume.bruno@gmail.com
including the input files and the line command that caused the bug.

You can also send me any comment or suggestion about the program.

	May 6, 2026. Bruno Gaume.
