// tool3.cpp
#include "include.h"
#include "handy_types.h"
#include "struct.h"

// ====================================================================================
// s2beta
// ====================================================================================
double s2beta(double s) { return std::tan((M_PI * s) / 2.0); }

// ====================================================================================
// tokenize
// ====================================================================================
void tokenize(std::string const &str, const char delim,
              std::vector<std::string> &out) {
  size_t start;
  size_t end = 0;
  while ((start = str.find_first_not_of(delim, end)) != std::string::npos) {
    end = str.find(delim, start);
    out.push_back(str.substr(start, end - start));
  }
}

// ====================================================================================
// Acomp_tuple_ld_ON_l (Comparison to be called in std::sort)
// ====================================================================================
bool Acomp_tuple_ld_ON_l(tuple_ld a, tuple_ld b) { // For Ascending order
  return (std::get<0>(a) < std::get<0>(b));
}

// ====================================================================================
// Acomp_tuple_ldd_ON_l (Comparison to be called in std::sort)
// ====================================================================================
bool Acomp_tuple_ldd_ON_l(tuple_ldd a, tuple_ldd b) { // For Ascending order
  return (std::get<0>(a) < std::get<0>(b));
}

// ====================================================================================
// AAcomp_tuple_lld_ON_ll (Comparison to be called in std::sort)
// ====================================================================================
bool AAcomp_tuple_lld_ON_ll(tuple_lld a,
                            tuple_lld b) { // For Ascending Ascending order
  if (std::get<0>(a) == std::get<0>(b)) {
    return (std::get<1>(a) < std::get<1>(b));
  } else {
    return (std::get<0>(a) < std::get<0>(b));
  }
}

// ====================================================================================
// Dcomp_tuple_lld_ON_d (Comparison to be called in std::sort)
// ====================================================================================
bool Dcomp_tuple_lld_ON_d(tuple_lld a, tuple_lld b) { // For Descending order
  double xa = std::get<2>(a);
  double xb = std::get<2>(b);
  return (xa > xb);
}

// ====================================================================================
// Acomp_tuple_lld_ON_d (Comparison to be called in std::sort)
// ====================================================================================
bool Acomp_tuple_lld_ON_d(tuple_lld a, tuple_lld b) { // For Aescending order
  double xa = std::get<2>(a);
  double xb = std::get<2>(b);
  return (xa < xb);
}

// ====================================================================================
// Acomp_tuple_ll_ON_l (Comparison to be called in std::sort)
// ====================================================================================
bool Acomp_tuple_ll_ON_l2(tuple_ll a, tuple_ll b) { // For Ascending order
  return (std::get<1>(a) + (1.0 / (std::get<0>(a) + 1)) <
          std::get<1>(b) + (1.0 / (std::get<0>(b) + 1)));
}

// ====================================================================================
// Dcomp_tuple_tuple_ll_vect_tuple_ll_ON_tuple_ll2 (Comparison to be called in
// std::sort)
// ====================================================================================
bool Dcomp_tuple_tuple_ll_vect_tuple_ll_ON_tuple_ll2(
    tuple_tuple_ll_vect_tuple_ll a,
    tuple_tuple_ll_vect_tuple_ll b) { // For Dscending order
  // X.sort(key = lambda z : DEG[z["i"]]+(1.0/(z["i"]+1)), reverse = True) #
  // True
  return (
      std::get<1>(std::get<0>(a)) + (1.0 / (std::get<0>(std::get<0>(a)) + 1)) >
      std::get<1>(std::get<0>(b)) + (1.0 / (std::get<0>(std::get<0>(b)) + 1)));
}

// ====================================================================================
// buildOrderedEdges_True_False
// ====================================================================================

void buildOrderedEdges_True_False(graph *g, vect_tuple_ll *OrdEdges) {
  // Translating in C++ from Python (too slow):
  // def buildOrderedEdges_True_False(g):
  //     """
  //     """
  //     nbv=len(g["nodes"]); nbe=len(g["links"])
  //     DEG=[0 for i in range(nbv)]
  //     X=[{"i":i, "n":[]} for i in range(nbv)]
  //
  //     for k in range(nbe):
  //         i=g["links"][k]["source"]
  //         j=g["links"][k]["target"]
  //         X[i]["n"].append(j); DEG[i]=DEG[i]+1
  //         X[j]["n"].append(i); DEG[j]=DEG[j]+1
  //
  //     X.sort(key = lambda z : DEG[z["i"]]+(1.0/(z["i"]+1)), reverse = True) #
  //     True in buildOrderedEdges_[True]_False for DIC in X: DIC["n"].sort(key
  //     = lambda z : DEG[z]+(1.0/(z+1)), reverse = False) # False in
  //     buildOrderedEdges_True_[False]
  //
  //     OrderedEdges=[]
  //     for k in range(nbv):
  //         for DIC in X:
  //             if len(DIC["n"]) >= k+1:
  //                 if DIC["i"] < DIC["n"][k]:
  //                     OrderedEdges.append((DIC["i"], DIC["n"][k]))
  //     return OrderedEdges

  vect_tuple_tuple_ll_vect_tuple_ll X;
  for (long unsigned int k = 0; k < (*g).nbv; ++k) {
    tuple_ll a = std::make_tuple(k, (*g).neighbors[k].size()); // a: (k, deg(k))
    vect_tuple_ll neg_a;
    for (std::size_t i = 0; i < (*g).neighbors[k].size(); i++) {
      neg_a.push_back(std::make_tuple(
          (*g).neighbors[k][i], ((*g).neighbors[(*g).neighbors[k][i]]).size()));
    }
    std::sort(neg_a.begin(), neg_a.end(),
              Acomp_tuple_ll_ON_l2); // buildOrderedEdges_True_[False]:
                                     // neigngors, Reverse = False
    X.push_back(std::make_tuple(a, neg_a));
  }

  std::sort(
      X.begin(), X.end(),
      Dcomp_tuple_tuple_ll_vect_tuple_ll_ON_tuple_ll2); // buildOrderedEdges_[True]_False:
                                                        // Global, reverse
                                                        // = True

  vect_tuple_lld OrdEdgeslld;

  for (std::size_t i = 0; i < X.size(); ++i) {
    long long ii = std::get<0>(std::get<0>(X[i]));
    for (std::size_t j = 0; j < (std::get<1>(X[i])).size(); ++j) {
      long long jj = std::get<0>((std::get<1>(X[i]))[j]);
      if (ii < jj) {
        OrdEdgeslld.push_back(std::make_tuple(ii, jj, (i + (j * (*g).nbv))));
      }
    }
  }

  std::sort(OrdEdgeslld.begin(), OrdEdgeslld.end(), Acomp_tuple_lld_ON_d);

  for (std::size_t i = 0; i < OrdEdgeslld.size(); ++i) {
    long long ii = std::get<0>(OrdEdgeslld[i]);
    long long jj = std::get<1>(OrdEdgeslld[i]);
    (*OrdEdges).push_back(std::make_tuple(ii, jj));
  }
}

// ====================================================================================
// PRF
// ====================================================================================
tuple_ddd PRF(double TP, double FP, double FN, double Beta) {
  // See: Two antagonistic objectives for one multi-scale graph clustering
  // framework,
  //      Bruno Gaume, Ixandra Achitouv, David Chavalarias,
  //      Nature Scientific Reports (2025).
  //      https://www.nature.com/articles/s41598-025-90454-w

  double Prec = ((TP + FP) > 0) ? TP / (TP + FP) : 0;
  double Rec = ((TP + FN) > 0) ? TP / (TP + FN) : 0;
  double FSCORE = ((Prec + Rec) > 0)
                      ? (1 + (Beta * Beta)) *
                            ((Prec * Rec) / (((Beta * Beta) * Prec) + Rec))
                      : 0;

  if (Beta > 1000000000) {
    return std::make_tuple(Prec, Rec, Rec);
  } else {
    return std::make_tuple(Prec, Rec, FSCORE);
  }
}

// ====================================================================================
// nodeposition: The position of a neighboring node
// ====================================================================================
long long nodeposition(vect_l *vi, nodeid j) {
  // vi is a vector<long long> containing the OUTneighbors of a node i (the
  // elements of vi shall already be sorted) IF j is in vi THEN Return the
  // position of j in vi ELSE Return -1
  //    COMPLEXITY: On average, logarithmic in N=vi.size() (the number of
  //    OUTneighbors of the node i) Performs approximately log2(N)+1 element
  //    comparisons.
  std::vector<nodeid>::iterator low;
  long long poslow;
  low = std::lower_bound(
      (*vi).begin(), (*vi).end(),
      j); // COMPLEXITY: On average, logarithmic in vi.size()
          // (see https://cplusplus.com/reference/algorithm/lower_bound/)
  if (!(low == (*vi).end())) {
    poslow = low - (*vi).begin();
    if ((*vi).at(poslow) == j) {
      return poslow;
    } else {
      return -1;
    }
  } else {
    return -1;
  }
}

// ====================================================================================
// emptyClustP
// ====================================================================================
void emptyClustP(clusteringP *clust) {
  (*clust).nbvP = 0;
  (*clust).nbmP = 0;
  (*clust).aliveP.clear();
  (*clust).membP.clear();
  (*clust).cP.clear();
  (*clust).with_nodes.clear();
}

// ====================================================================================
// emptySupergraph
// ====================================================================================
void emptySupergraph(Supergraph *G) {
  (*G).Super_nbv = 0;
  (*G).Super_nodes.clear();
  (*G).Super_nbe = 0;
  (*G).Super_neighbors.clear();
  (*G).Super_weight.clear();
}

// ====================================================================================
// Localpartcon
// ====================================================================================
set_set_l Localpartcon(graph *graph, clusteringP *clust, long long m) {
  set_set_l componentS;
  vect_l MOD;
  vect_b visited;
  for (auto it = (*clust).cP[m].begin(); it != (*clust).cP[m].end(); it++) {
    MOD.push_back(*it);
    visited.push_back(false);
  }
  long long SIZEMOD = (*clust).cP[m].size();
  long long k = 0;
  long long nbvisited = 0;
  bool CONT = true;
  while (CONT) {
    if (not visited[k]) {
      set_l component;
      std::queue<nodeid> q;
      q.push(MOD[k]);
      visited[k] = true;
      while (!q.empty()) {
        nodeid node = q.front();
        q.pop();
        component.insert(node);
        nbvisited = nbvisited + 1;
        for (long long i = 0; i < SIZEMOD; ++i) {
          if (((std::binary_search((*graph).neighbors[node].begin(),
                                   (*graph).neighbors[node].end(), MOD[i]))) &&
              !visited[i]) { // one edge {node, MOD[i]} in *graph
            visited[i] = true;
            q.push(MOD[i]);
          }
        }
      }
      if (!component.empty()) {
        componentS.insert(component);
      }
    }
    k = k + 1;
    if ((k == SIZEMOD) or (nbvisited == SIZEMOD)) {
      CONT = false;
    }
  }
  return componentS;
}

// ====================================================================================
// GainLeaving (when a node leaves a module, how TruePositives,  FalsePositives,
// and  FalseNegatives, vary)
// ====================================================================================
tuple_ddd GainLeaving(graph *g, clusteringP *clust, nodeid x, long long mod_x) {
  double ACC = 0;
  long long nbe = 0;
  for (auto it = (*clust).cP[mod_x].begin(); it != (*clust).cP[mod_x].end();
       it++) {
    if (x != *it) {
      long long pos = nodeposition(&((*g).neighbors[x]), *it);
      if (!(pos == -1)) { // one edge {x,*it} in g
        nbe++;
        ACC = ACC + (*g).weight[x].at(pos);
      }
    }
  }
  // varyingTP, varyingFP, varyingFN
  return std::make_tuple(
      -ACC, -(((*clust).cP[mod_x].size() - 1 - nbe) * (*g).meanWeight), ACC);
}

// ====================================================================================
// GainJoining (when a node joins a module, how TruePositives,  FalsePositives,
// and  FalseNegatives, vary)
// ====================================================================================
tuple_ddd GainJoining(graph *g, clusteringP *clust, nodeid x, long long mod_y) {
  double ACC = 0;
  long long nbe = 0;
  for (auto it = (*clust).cP[mod_y].begin(); it != (*clust).cP[mod_y].end();
       it++) {
    long long pos = nodeposition(&((*g).neighbors[x]), *it);
    if (!(pos == -1)) { // one edge {x,*it} in graph
      nbe++;
      ACC = ACC + (*g).weight[x].at(pos);
    }
  }
  // varyingTP, varyingFP, varyingFN
  return std::make_tuple(
      ACC, ((*clust).cP[mod_y].size() - nbe) * (*g).meanWeight, -ACC);
}

// ====================================================================================
// SuperGainLeaving (when a node leaves a module, how TruePositives,
// FalsePositives, and  FalseNegatives, vary)
// ====================================================================================
tuple_ddd SuperGainLeaving(Supergraph *G, clusteringP *CLUST, nodeid x,
                           long long mod_x, clusteringP *clust,
                           double meanWeight) {
  long long pos;
  double varyingTP = 0, varyingFP = 0;
  double a, b;
  for (auto it = (*CLUST).cP[mod_x].begin(); it != (*CLUST).cP[mod_x].end();
       it++) {
    if (x != *it) {
      pos = nodeposition(&((*G).Super_neighbors[x]), *it);
      if (!(pos == -1)) { // {x,*it} connected in G
        std::tie(a, b) = (*G).Super_weight[x][pos];
        varyingTP = varyingTP - a;
        varyingFP = varyingFP - b;
      } else { // {x,*it} not connected in G
        varyingFP = varyingFP - (((*clust).cP[(*G).Super_nodes[x]].size() *
                                  (*clust).cP[(*G).Super_nodes[*it]].size()) *
                                 meanWeight);
      }
    }
  }
  // varyingTP, varyingFP, varyingFN
  return std::make_tuple(varyingTP, varyingFP, -varyingTP);
}

// ====================================================================================
// SuperGainJoining (when a node joins a module, how TruePositives,
// FalsePositives, and  FalseNegatives, vary)
// ====================================================================================
tuple_ddd SuperGainJoining(Supergraph *G, clusteringP *CLUST, nodeid x,
                           long long mod_y, clusteringP *clust,
                           double meanWeight) {
  long long pos;
  double varyingTP = 0, varyingFP = 0;
  double a, b;
  for (auto it = (*CLUST).cP[mod_y].begin(); it != (*CLUST).cP[mod_y].end();
       it++) {
    pos = nodeposition(&((*G).Super_neighbors[x]), *it);
    if (!(pos == -1)) { // {x,*it} connected in G
      std::tie(a, b) = (*G).Super_weight[x][pos];
      varyingTP = varyingTP + a;
      varyingFP = varyingFP + b;
    } else { // {x,*it} not connected in G
      varyingFP = varyingFP + (((*clust).cP[(*G).Super_nodes[x]].size() *
                                (*clust).cP[(*G).Super_nodes[*it]].size()) *
                               meanWeight);
    }
  }
  // varyingTP, varyingFP, varyingFN
  return std::make_tuple(varyingTP, varyingFP, -varyingTP);
}

// ====================================================================================
// SuperGainMerging (when a modul mod_x MERGE with a modul mod_y, how
// TruePositives,  FalsePositives, and  FalseNegatives, vary)
// ====================================================================================
tuple_ddd SuperGainMerging(Supergraph *G, clusteringP *CLUST, long long mod_x,
                           long long mod_y, clusteringP *clust,
                           double meanWeight) {
  double varyingTP = 0, varyingFP = 0;
  double a, b;
  for (auto itx = (*CLUST).cP[mod_x].begin(); itx != (*CLUST).cP[mod_x].end();
       itx++) {
    for (auto it = (*CLUST).cP[mod_y].begin(); it != (*CLUST).cP[mod_y].end();
         it++) {
      long long pos = nodeposition(&((*G).Super_neighbors[*itx]), *it);
      if (!(pos == -1)) { // {*itx,*it} connected in G
        std::tie(a, b) = (*G).Super_weight[*itx][pos];
        varyingTP = varyingTP + a;
        varyingFP = varyingFP + b;
      } else { // {*itx,*it} not connected in G
        varyingFP = varyingFP + (((*clust).cP[(*G).Super_nodes[*itx]].size() *
                                  (*clust).cP[(*G).Super_nodes[*it]].size()) *
                                 meanWeight);
      }
    }
  }
  // varyingTP, varyingFP, varyingFN
  return std::make_tuple(varyingTP, varyingFP, -varyingTP);
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// OVERLAPS
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

// ====================================================================================
// CardIntersec
// ====================================================================================
long unsigned int CardIntersec(set_l *a, set_l *b) { // return |*a ∩ *b|
  long unsigned int acc = 0;
  if ((*a).size() < (*b).size()) {
    for (auto i : (*a)) {
      if ((*b).end() != (*b).find(i)) {
        acc = acc + 1;
      }
    }
  } else {
    for (auto i : (*b)) {
      if ((*a).end() != (*a).find(i)) {
        acc = acc + 1;
      }
    }
  }
  return acc;
}

// ====================================================================================
// EmptyIntersec
// ====================================================================================
bool EmptyIntersec(set_l *a, set_l *b) {
  if ((*a).size() < (*b).size()) {
    for (auto i : (*a)) {
      if ((*b).end() != (*b).find(i)) {
        return false;
      }
    }
  } else {
    for (auto i : (*b)) {
      if ((*a).end() != (*a).find(i)) {
        return false;
      }
    }
  }
  return true;
}

// ====================================================================================
// GainOverlaps
// ====================================================================================
tuple_ddd GainOverlaps(graph *g, clusteringP *clust, vect_set_l *membO,
                       long long x, long long mody) {
  long long pos;
  double XTP = 0, XFP = 0;
  for (auto it = (*clust).cP[mody].begin(); it != (*clust).cP[mody].end();
       it++) {
    if (EmptyIntersec(
            &(*membO)[x],
            &(*membO)[*it])) { // x and *it are not together in any modules
      pos = nodeposition(&(*g).neighbors[x], *it);
      if (!(pos == -1)) { // one edge {x,*it} in graph
        XTP = XTP + ((*g).weight)[x].at(pos);
      } else {
        XFP = XFP + (*g).meanWeight;
      }
    }
  }

  for (auto it = (*clust).with_nodes[mody].begin();
       it != (*clust).with_nodes[mody].end(); it++) {
    if (EmptyIntersec(
            &(*membO)[x],
            &(*membO)[*it])) { // x and *it are not together in any modules
      pos = nodeposition(&(*g).neighbors[x], *it);
      if (!(pos == -1)) { // one edge {x,*it} in graph
        XTP = XTP + ((*g).weight)[x].at(pos);
      } else {
        XFP = XFP + (*g).meanWeight;
      }
    }
  }
  return std::make_tuple(XTP, XFP, -XTP);
}

// ====================================================================================
// ModContribution
// ====================================================================================
tuple_ddd ModContribution(graph *g, clusteringP *clust, long long mody) {
  double TP = 0, FP = 0;
  long long pos_node;

  for (auto i : (*clust).cP[mody]) {
    for (auto j : (*clust).cP[mody]) {
      if (i < j) {
        pos_node = nodeposition(&(*g).neighbors[i], j);
        if (!(pos_node == -1)) { // one edge {i,j} in graph
          TP = TP + (*g).weight[i].at(pos_node);
        } else { // no edge {i,j} in graph
          FP = FP + (*g).meanWeight;
        }
      }
    }
  }

  for (auto i : (*clust).with_nodes[mody]) {
    for (auto j : (*clust).with_nodes[mody]) {
      if (i < j) {
        pos_node = nodeposition(&(*g).neighbors[i], j);
        if (!(pos_node == -1)) { // one edge {i,j} in graph
          TP = TP + (*g).weight[i].at(pos_node);
        } else { // no edge {i,j} in graph
          FP = FP + (*g).meanWeight;
        }
      }
    }
  }

  for (auto i : (*clust).cP[mody]) {
    for (auto j : (*clust).with_nodes[mody]) {
      if (i < j) {
        pos_node = nodeposition(&(*g).neighbors[i], j);
        if (!(pos_node == -1)) { // one edge {i,j} in graph
          TP = TP + (*g).weight[i].at(pos_node);
        } else { // no edge {i,j} in graph
          FP = FP + (*g).meanWeight;
        }
      }
    }
  }

  for (auto i : (*clust).with_nodes[mody]) {
    for (auto j : (*clust).cP[mody]) {
      if (i < j) {
        pos_node = nodeposition(&(*g).neighbors[i], j);
        if (!(pos_node == -1)) { // one edge {i,j} in graph
          TP = TP + (*g).weight[i].at(pos_node);
        } else { // no edge {i,j} in graph
          FP = FP + (*g).meanWeight;
        }
      }
    }
  }

  return std::make_tuple(TP, FP, -TP);
}

// ====================================================================================
// Acomp_mod_ON_size (Comparison to be called in std::sort)
// ====================================================================================
bool Acomp_mod_ON_size(set_l a, set_l b) { // For Ascending order
  return (a.size() < b.size());
}
