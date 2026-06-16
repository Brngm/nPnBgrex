// tool2.h

#ifndef TOOL2_H
#define TOOL2_H
#include "include.h"
#include "handy_types.h"
#include "struct.h"
double s2beta(double);
const char *graph_from_file(const char *, graph *, vect_tuple_ll *, const char *);
const char *ClustP_from_file(const char *, long unsigned int , clusteringP *);
tuple_dddd clust_with_connex_modules(graph *, clusteringP *);
tuple_dddd MakePartition(graph *, clusteringP *, vect_tuple_ll *, tuple_dddd , double , double , long long );
void initSupergraph(graph *, clusteringP *, Supergraph *);
void initSuperClust(Supergraph *, clusteringP *);
tuple_dddd MakeSuperPartition(Supergraph *, clusteringP *, tuple_dddd , double , clusteringP *, double ,
                              double , vect_l *, vect_tuple_ll *, long long );
void Superclust_2_clust(graph *, clusteringP *, Supergraph *, clusteringP *);

tuple_dddd MakeOverlaps(graph *, clusteringP *, tuple_dddd , double, long long);

const char *ClustPO_from_file(const char *, long unsigned int,
                                  clusteringO *, const char *);

tuple_dd OMEGA_PO(graph *, clusteringP *);
#endif