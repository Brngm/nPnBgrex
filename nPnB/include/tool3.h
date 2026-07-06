// tool3.h

#ifndef TOOL3_H
#define TOOL3_H
#include "include.h"
#include "handy_types.h"
#include "struct.h"

double s2beta(double);
void tokenize(std::string const &, const char , std::vector<std::string> &);
bool Acomp_tuple_ld_ON_l(tuple_ld, tuple_ld);
bool Acomp_tuple_ldd_ON_l(tuple_ldd, tuple_ldd);
bool AAcomp_tuple_lld_ON_ll(tuple_lld, tuple_lld);
bool Dcomp_tuple_lld_ON_d(tuple_lld, tuple_lld);
bool Acomp_tuple_lld_ON_d(tuple_lld , tuple_lld );
bool Acomp_tuple_ll_ON_l2(tuple_ll, tuple_ll );
bool Dcomp_tuple_tuple_ll_vect_tuple_ll_ON_tuple_ll2(tuple_tuple_ll_vect_tuple_ll, tuple_tuple_ll_vect_tuple_ll);
void buildOrderedEdges_True_False(graph *, vect_tuple_ll *);
tuple_ddd PRF(double, double , double , double );
long long nodeposition(vect_l *, nodeid );
void print_graph(graph *);
void emptyClustP(clusteringP *);
void print_clustP(clusteringP *);
void emptySupergraph(Supergraph *);
void print_Supergraph(clusteringP *, Supergraph *);
set_set_l Localpartcon(graph *, clusteringP *, long long );
tuple_ddd GainLeaving(graph *, clusteringP *, nodeid , long long );
tuple_ddd GainJoining(graph *, clusteringP *, nodeid , long long );
tuple_ddd SuperGainLeaving(Supergraph *, clusteringP *, nodeid , long long , clusteringP *, double );
tuple_ddd SuperGainJoining(Supergraph *, clusteringP *, nodeid , long long , clusteringP *, double );
tuple_ddd SuperGainMerging(Supergraph *, clusteringP *, long long , long long , clusteringP *, double );
void ClustO_from_string(std::string , clusteringO *, long unsigned int );
void print_clustO(clusteringO *);
long unsigned int CardIntersec(set_l *, set_l *);
tuple_ddd GainOverlaps(graph *, clusteringP *, vect_set_l *,long long, long long );
tuple_ddd ModContribution(graph *, clusteringP *, long long );

bool Acomp_mod_ON_size(set_l, set_l);
void eliminate_duplicates(clusteringO *);

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// TODO pas utilisés
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void OrdEdges_from_string(std::string , vect_tuple_ll *);
void graph_from_string(char *, graph *);
void MakeAdherents(graph *, clusteringP *, vect_vect_l *);
#endif