
#ifndef STRUCT_H
#define STRUCT_H

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// graph g; // g is a weighted graph:
typedef struct {
    long unsigned int nbv; // g.nbv is the number of vertices of g
    long unsigned int nbe; // g.nbe is the number of edges of g
    double sumWeight; // g.sumWeight is the sum of the weights of all the edges of g
    double meanWeight; // g.meanWeight is the average weights of the edges of g

    vect_vect_l neighbors; // g.neighbors[x] is a vector<long long> of the neighbors of the vertex x (g.neighbors[x] is sorted in ascending order)
    vect_vect_d weight; // g.weight[x] is a vector<double> of the weights of the edges {x,y} (where the y are the neighbors of x)
    /**
        A NOTE, g.neighbors[x] is sorted in ascending order and is aligned with g.weight[x]:
        for(long long i = 0; i < g.neighbors[30].size(); i++) {
            long long y=g.neighbors[30][i]; // y is the ith neighbor of the node 30
            double w=g.weight[30].at(i); // w is the weight of the edge {30,y}
        }
    */
} graph;

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// Supergraph G; // G is a weighted graph, a vertex represents a module of a clusteringP clust on a graph g
typedef struct {
    long unsigned int Super_nbv; // G.Super_nbv is the number of vertices of G (The number of living modules of clust)
    long unsigned int Super_nbe; // G.Super_nbe is the number of edges of G
    vect_l Super_nodes;  // clust.cP[G.Super_nodes[x]] is the module of clust that the vertex x represents
    vect_vect_l Super_neighbors; // G.Super_neighbors[x] is a vector<long long> of the neighbors of the vertex x (G.Super_neighbors[x] is sorted in ascending order)
    vect_vect_tuple_dd Super_weight; // G.Super_weight[x] is a vector<tuple<double, double>> of the Superweight of the edges {x,y} (where the y are the neighbors of x)
    /**
        G.Super_neighbors[x] is sorted in ascending order and is aligned with G.Super_weight[x]:
        for(long long i = 0; i < G.Super_neighbors[30].size(); i++) {
            long long y=G.Super_neighbors[30][i]; // y is the ith neighbor of the node 30
            tuple<double, double> W=G.Super_weight[30].at(i); // W is the Superweight of the edge {30,y}
            // IF
            //      W is equal to: <a,b>
            //      modx is equal to:  clust.cP[G.Super_nodes[30]] (modx is the module of clust that the vertex x represents)
            //      mody is equal to: clust.cP[G.Super_nodes[y]] (mody is the module of clust that the vertex y represents)
            // THEN
            //      a is equal to: the total of the weights of the edges between modx and mody in the graph g
            //      b is equal to: g.meanWeight * (the number of non-edges between the nodes of modx and the nodes of mody in the graph g)
        }
    */
} Supergraph;

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// clusteringP clust; // clust is a partitional clustering with the gluant extentions of its module.
typedef struct {
  long unsigned int nbvP; // clust.nbvP is the number of vertices of the support graph (g.nbv)
  long unsigned int nbmP; // clust.nbmP is the number of living modules
  vect_set_l cP; // PARTITIONAL CLUSTERING: clust.cP[3] is the partitional module number 3
  vect_b aliveP; // bool x=clust.aliveP[3]: IF x==true THEN module number 3 is alive ELSE it is dead
  vect_l membP;  // long unsigned int x=clust.membP[30]: The vertex number 3O is a member of the partitional module number x

  vect_vect_l with_nodes; // clust.with_nodes[3] is the gluant extention of the
                          // Pratitional module clust.cP[3]:
                          // The UNION of nodes in clust.cP[3] with nodes in clust.with_nodes[3] is the overlaping module number 3

} clusteringP;

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// clusteringO clust; // clust is an overlaping clustering
typedef struct {
  long unsigned int nbvO; // clust.nbvO is the number of vertices of the support graph (g.nbv)
  long unsigned int nbmO; // clust.nbmO is the number of living modules
  vect_set_l cO; // clust.cO[3] is the module number 3

  vect_b aliveO; // bool x=clust.aliveO[3]: // IF x==true THEN module number 3 is alive ELSE it is dead

  vect_set_l membO; // set_l s=clust.membO[3]; // s is a set<long long>: IF i ∈ s THEN node 3 belongs to the ith module ELSE node 3 does not belong to the ith module
                    // Here is the difference with struct clusteringP where membP is a vector<long long>,
                    // while here membO is a vector<set<long long>> because clust is an
                    // overlaping clustering (a node can belong to several modules)
} clusteringO;

#endif


