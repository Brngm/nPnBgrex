// tool2.cpp
#include "include.h"
#include "handy_types.h"
#include "struct.h"
#include "tool3.h"

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// graph_from_file
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
const char *graph_from_file(const char *InfileGraph, graph *g,
                            vect_tuple_ll *OrdEdges,
                            const char *OrderedEdeges) {

  // ============================================
  // first reading
  // ============================================
  std::ifstream InfG;
  InfG.open(InfileGraph, std::ios::in);
  if (!(InfG.is_open())) {
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: Opening input_graph_file '" << InfileGraph << "' failure"
           << std::endl;
    ERRORS = ERRORA.str();
    return ERRORS.c_str();
  }

  std::string line;
  long long id_node_max = 0, nbe = 0;
  while (std::getline(InfG, line)) {
    if (!line.empty()) {
      if (!(line[0] == '#')) { // not a comment
        std::istringstream sline(line);
        std::string tok1;
        std::getline(sline, tok1, '\t');
        nodeid tok1int = std::stoi(tok1);
        std::string tok2;
        std::getline(sline, tok2, '\t');
        nodeid tok2int = std::stoi(tok2);
        if (id_node_max < tok1int) {
          id_node_max = tok1int;
        }
        if (id_node_max < tok2int) {
          id_node_max = tok2int;
        }
        if (tok1int != tok2int) {
          nbe = nbe + 1;
          if ((strcmp(OrderedEdeges, "1") == 0)) {
            (*OrdEdges).push_back(std::make_tuple(tok1int, tok2int));
          }
        }
      }
    }
  }
  InfG.close();

  (*g).nbv = id_node_max + 1; // initializes nbv (the number of vertices)
  (*g).nbe = nbe;             // initializes nbe (the number of edges)

  vect_vect_tuple_ld graphweight;
  graphweight.resize((*g).nbv);
  (*g).sumWeight =
      0; // initializes sumWeight (the sum of the weights of all edges)

  // ============================================
  // second reading
  // ============================================
  InfG.open(InfileGraph);
  while (std::getline(InfG, line)) {
    if (!line.empty()) {
      if (!(line[0] == '#')) { // not a comment
        std::istringstream sline(line);
        std::string tok1;
        std::getline(sline, tok1, '\t');
        nodeid tok1int = std::stoi(tok1);
        std::string tok2;
        std::getline(sline, tok2, '\t');
        nodeid tok2int = std::stoi(tok2);
        std::string tok3;
        std::getline(sline, tok3, '\t');
        double tok3double = stod(tok3);
        if (tok1int != tok2int) {
          graphweight[tok1int].push_back(std::make_tuple(tok2int, tok3double));
          graphweight[tok2int].push_back(std::make_tuple(tok1int, tok3double));
          (*g).sumWeight = (*g).sumWeight + tok3double;
        }
      }
    }
  }
  InfG.close();

  if ((*g).nbe >
      0) { // initializes meanWeight (the average of the weights of all edges)
    (*g).meanWeight = (*g).sumWeight / (*g).nbe;
  } else {
    (*g).meanWeight = 1;
  }

  (*g).neighbors.resize((*g).nbv);
  (*g).weight.resize((*g).nbv);
  for (long unsigned int k = 0; k < (*g).nbv; ++k) {
    std::sort(graphweight[k].begin(), graphweight[k].end(),
              Acomp_tuple_ld_ON_l);
    for (auto it = graphweight[k].begin(); it != graphweight[k].end(); it++) {
      (*g).neighbors[k].push_back(
          std::get<0>(*it)); // initializes the edge {k,*it}
      (*g).weight[k].push_back(
          std::get<1>(*it)); // initializes the weight of the edge {k,*it}
    }
  }

  if (!(strcmp(OrderedEdeges, "1") == 0)) { // Edges are not ordered
    buildOrderedEdges_True_False(g, OrdEdges);
  }
  return "";
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ClustP_from_file
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
const char *ClustP_from_file(const char *InfileClust, long unsigned int gnbv,
                             clusteringP *clust) {

  // initialize clust
  long long NBV = gnbv;
  vect_l NodeAssigned;
  NodeAssigned.resize(NBV);

  vect_set_l c;
  c.resize(NBV);
  (*clust).cP = c;
  vect_l memb;
  memb.resize(NBV);
  (*clust).membP = memb;
  vect_b alive;
  alive.resize(NBV);
  (*clust).aliveP = alive;

  (*clust).with_nodes.resize(NBV);

  for (long unsigned int i = 0; i < gnbv; i++) {
    set_l mod_i;
    (*clust).cP[i] = mod_i;
    (*clust).membP[i] = -1;
    (*clust).aliveP[i] = false;
    NodeAssigned[i] = -1;

    vect_l with_nodes_i;
    (*clust).with_nodes[i] = with_nodes_i;
  }

  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int fd = open(InfileClust, O_RDONLY);
  if (fd == -1) {
    close(fd);
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: Opening Infile ClustZero '" << InfileClust << "' failure"
           << std::endl;
    ERRORS = ERRORA.str();
    return ERRORS.c_str();
  }

  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    close(fd);
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: Opening Infile ClustZero '" << InfileClust << "' failure"
           << std::endl;
    ERRORS = ERRORA.str();
    return ERRORS.c_str();
  }

  size_t filesize = sb.st_size;

  char *data = (char *)mmap(nullptr, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data == MAP_FAILED) {
    close(fd);
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: Opening Infile ClustZero '" << InfileClust << "' failure"
           << std::endl;
    ERRORS = ERRORA.str();
    return ERRORS.c_str();
  }

  char *ptr = data;
  char *end = data + filesize;

  size_t line_number = 0;
  long unsigned int IdMod = 0;

  while (ptr < end) {
    ++line_number;

    char *line_start = ptr;

    while (ptr < end && *ptr != '\n')
      ++ptr;

    char *line_end = ptr;
    ++ptr;

    if (line_end - line_start < 3 || line_start[0] != 'm' ||
        line_start[1] != '\t')
      continue;

    std::unordered_set<std::string_view> L1;
    std::unordered_set<std::string_view> L2;

    L1.reserve(16);
    L2.reserve(16);

    bool seen_bang = false;

    char *tok_start = line_start + 2;
    char *p = tok_start;

    while (p <= line_end) {
      if (p == line_end || *p == '\t') {
        std::string_view token(tok_start, p - tok_start);

        if (token == "!") {
          if (seen_bang) {
            close(fd);
            static std::string ERRORS;
            std::ostringstream ERRORA;
            ERRORA << "ERROR: multiple '!' (line " << line_number << ")\n";
            ERRORS = ERRORA.str();
            return ERRORS.c_str();
          }
          seen_bang = true;
        } else {
          // conversion string_view -> long long (sans stoi)
          long long y;
          auto res =
              std::from_chars(token.data(), token.data() + token.size(), y);

          if (res.ec != std::errc() || res.ptr != token.data() + token.size()) {
            close(fd);
            static std::string ERRORS;
            std::ostringstream ERRORA;
            ERRORA << "ERROR: invalid integer -> '" << token << "' (line "
                   << line_number << ")\n";
            ERRORS = ERRORA.str();
            return ERRORS.c_str();
          }

          if (!seen_bang) {
            L1.insert(token);
            // std::cout << y << " in L1 IdMod=" << IdMod << "\n";

            if ((y < 0)) {
              close(fd);
              static std::string ERRORS;
              std::ostringstream ERRORA;
              ERRORA << "ERROR: In ClustZero file '" << InfileClust;
              ERRORA << "': " << token << "<0" << "' (line " << line_number
                     << ")\n";
              ERRORS = ERRORA.str();
              return ERRORS.c_str();
            }

            if (y > (NBV - 1)) {
              close(fd);
              static std::string ERRORS;
              std::ostringstream ERRORA;
              ERRORA << "ERROR In ClustZero file '" << InfileClust << "': ";
              ERRORA << "token " << token << ">" << NBV - 1
                     << " when id node in [0," << NBV - 1 << "], because "
                     << NBV << " nodes in the graph";
              ERRORA << " (line " << line_number << ")\n";
              ERRORS = ERRORA.str();
              return ERRORS.c_str();
            }

            if (!(NodeAssigned[y] == -1)) {
              close(fd);
              static std::string ERRORS;
              std::ostringstream ERRORA;
              ERRORA << "ERROR In ClustZero file '" << InfileClust << "': ";
              ERRORA
                  << "Unable to assign node " << token
                  << " to partitioning module " << IdMod
                  << " because it is already affected to partitioning module "
                  << NodeAssigned[y]
                  << " when no overlap is allowed in partition";
              ERRORA << " (line " << line_number << ")\n";
              ERRORS = ERRORA.str();
              return ERRORS.c_str();
            }

            (*clust).cP[IdMod].insert(y);
            (*clust).aliveP[IdMod] = true;
            (*clust).membP[y] = IdMod;
            NodeAssigned[y] = IdMod;
          } else {
            // ignore extension (because we want a partitional clustering)
          }
        }

        tok_start = p + 1;
      }
      ++p;
    }

    if ((!seen_bang)) {
      close(fd);
      static std::string ERRORS;
      std::ostringstream ERRORA;
      ERRORA << "ERROR In ClustZero file '" << InfileClust << "': ";
      ERRORA << "missing '!' (line " << line_number << ")\n";
      ERRORS = ERRORA.str();
      return ERRORS.c_str();
    }

    if (seen_bang) {
      // std::cout << line_number << " OK\n";
      IdMod = IdMod + 1;
    }
  }

  munmap(data, filesize);
  close(fd);

  (*clust).nbvP = NBV; // the number of vertices of the support graph (g.nbv)
  (*clust).nbmP = 0;   // the number of living modules
  for (long unsigned int i = 0; i < gnbv; i++) {
    if ((*clust).aliveP[i]) {
      (*clust).nbmP = (*clust).nbmP + 1;
    }
    if ((*clust).membP[i] == -1) {
      static std::string ERRORS;
      std::ostringstream ERRORA;
      ERRORA << "ERROR: In ClustZero file '" << InfileClust << "', node " << i
             << " is not affected to a module " << std::endl;
      ERRORS = ERRORA.str();
      return ERRORS.c_str();
    }
  }

  return "";
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// clust_with_connex_modules
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
tuple_dddd clust_with_connex_modules(graph *graph, clusteringP *clust) {
  double TP = 0, FP = 0, FN = (*graph).sumWeight;
  double TN = (((*graph).nbv * ((*graph).nbv - 1) * 0.5) - (*graph).nbe) *
              (*graph).meanWeight;

  clusteringP newclust;
  newclust.nbvP = (*graph).nbv;
  newclust.nbmP = 0;
  newclust.cP.resize((*graph).nbv);
  newclust.membP.resize((*graph).nbv);
  newclust.aliveP.resize((*graph).nbv);
  newclust.with_nodes.resize((*graph).nbv);

  for (long unsigned int i = 0; i < (*graph).nbv; i++) {
    newclust.aliveP[i] = false;
  }

  for (std::size_t ii = 0; ii < (*clust).cP.size(); ii++) {
    if ((*clust).aliveP[ii]) {
      set_set_l componentS = Localpartcon(graph, clust, ii);
      for (auto it = componentS.begin(); it != componentS.end(); it++) {
        set_l mod_it;
        for (auto i : *it) {
          mod_it.insert(i);
          newclust.membP[i] = newclust.nbmP;
          for (auto j : *it) {
            if (i < j) {
              long long pos_node = nodeposition(&((*graph).neighbors[i]), j);
              if (!(pos_node == -1)) { // one edge {i,j} in graph
                TP = TP + (*graph).weight[i].at(pos_node);
                FN = FN - (*graph).weight[i].at(pos_node);
              } else { // no edge {i,j} in graph
                FP = FP + (*graph).meanWeight;
                TN = TN - (*graph).meanWeight;
              }
            }
          }
        }
        newclust.cP[newclust.nbmP] = mod_it;
        newclust.aliveP[newclust.nbmP] = true;
        newclust.nbmP = newclust.nbmP + 1;
      }
    }
  }

  emptyClustP(clust);
  (*clust).nbvP = newclust.nbvP;
  (*clust).nbmP = newclust.nbmP;
  (*clust).cP.resize((*graph).nbv);
  (*clust).membP.resize((*graph).nbv);
  (*clust).aliveP.resize((*graph).nbv);
  (*clust).with_nodes.resize((*graph).nbv);
  //
  for (long unsigned int i = 0; i < newclust.nbvP; i++) {
    (*clust).membP[i] = newclust.membP[i];
    (*clust).aliveP[i] = newclust.aliveP[i];
    (*clust).cP[i] = newclust.cP[i];
  }

  return std::make_tuple(TP, TN, FP, FN);
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// MakePartition
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
tuple_dddd MakePartition(graph *g, clusteringP *clust, vect_tuple_ll *OrdEdges,
                         tuple_dddd TP_TN_FP_FN, double scale, double Epsilon,
                         long long VerboseInt) {
  double Beta = s2beta(scale);

  // double TP=0, TN=0, FP=0, FN=0;
  // std::tie(TP, TN, FP, FN) = TP_TN_FP_FN;
  auto [TP, TN, FP, FN] = TP_TN_FP_FN;

  double Prec, Rec, FSCORE;
  std::tie(Prec, Rec, FSCORE) = PRF(TP, FP, FN, Beta);

  double newTP, newFP, newFN, newTN;
  double newPrec, newRec, newFSCORE;

  double varyingTPiciL, varyingFPiciL, varyingFNiciL;
  double varyingTPici, varyingFPici, varyingFNici;

  double newTPici, newFPici, newFNici, newTNici;
  double newPrecici, newRecici, newFSCOREici;
  nodeid move_node = 0, with_node = 0;

  bool CONT = true;
  while (CONT) {
    time_t timerintermed = time(NULL);
    double CONTFSCORE = FSCORE;

    FastProgressPercent bar((*OrdEdges).size());
    for (long unsigned int IT = 0; IT < (*OrdEdges).size();
         ++IT) { // loop on OrdEdges
      newFSCORE = FSCORE;
      newPrec = Prec;
      newRec = Rec;
      long long xe = std::get<0>((*OrdEdges)[IT]);
      long long mod_xe = (*clust).membP[xe];

      long long ye = std::get<1>((*OrdEdges)[IT]);
      long long mod_ye = (*clust).membP[ye];

      // *****************************************
      // IF xe Leaving its module
      // *****************************************
      std::tie(varyingTPiciL, varyingFPiciL, varyingFNiciL) =
          GainLeaving(g, clust, xe, mod_xe);

      // *****************************************
      // IF xe can Leave its module
      // *****************************************
      if ((*clust).cP[mod_xe].size() > 1) {
        newTPici = TP + varyingTPiciL;
        newFPici = FP + varyingFPiciL;
        newFNici = FN + varyingFNiciL;
        newTNici = TN - varyingFPiciL;
        std::tie(newPrecici, newRecici, newFSCOREici) =
            PRF(newTPici, newFPici, newFNici, Beta);
        if ((newFSCOREici > newFSCORE) ||
            ((newPrecici > newPrec) & (newRecici >= newRec)) ||
            ((newPrecici >= newPrec) & (newRecici > newRec))) {
          move_node = xe;
          with_node = -1;
          newFSCORE = newFSCOREici;
          newPrec = newPrecici;
          newRec = newRecici;
          newTP = newTPici;
          newFP = newFPici;
          newFN = newFNici;
          newTN = newTNici;
        }
      }

      // *****************************************
      // IF xe Leave its module && xe MOve into ye's module
      // *****************************************
      if (mod_xe != mod_ye) {
        std::tie(varyingTPici, varyingFPici, varyingFNici) =
            GainJoining(g, clust, xe, mod_ye);

        newTPici = TP + varyingTPiciL;
        newFPici = FP + varyingFPiciL;
        newFNici = FN + varyingFNiciL;
        newTNici = TN - varyingFPiciL;
        newTPici = newTPici + varyingTPici;
        newFPici = newFPici + varyingFPici;
        newFNici = newFNici + varyingFNici;
        newTNici = newTNici - varyingFPici;
        std::tie(newPrecici, newRecici, newFSCOREici) =
            PRF(newTPici, newFPici, newFNici, Beta);
        if ((newFSCOREici > newFSCORE) ||
            ((newPrecici > newPrec) & (newRecici >= newRec)) ||
            ((newPrecici >= newPrec) & (newRecici > newRec))) {
          move_node = xe;
          with_node = ye;
          newFSCORE = newFSCOREici;
          newPrec = newPrecici;
          newRec = newRecici;
          newTP = newTPici;
          newFP = newFPici;
          newFN = newFNici;
          newTN = newTNici;
        }
      }

      // *****************************************
      // IF ye Leaving its module
      // *****************************************
      std::tie(varyingTPiciL, varyingFPiciL, varyingFNiciL) =
          GainLeaving(g, clust, ye, mod_ye);

      // *****************************************
      // IF ye can Leave its module
      // *****************************************
      if ((*clust).cP[mod_ye].size() > 1) {
        newTPici = TP + varyingTPiciL;
        newFPici = FP + varyingFPiciL;
        newFNici = FN + varyingFNiciL;
        newTNici = TN - varyingFPiciL;
        std::tie(newPrecici, newRecici, newFSCOREici) =
            PRF(newTPici, newFPici, newFNici, Beta);
        if ((newFSCOREici > newFSCORE) ||
            ((newPrecici > newPrec) & (newRecici >= newRec)) ||
            ((newPrecici >= newPrec) & (newRecici > newRec))) {
          move_node = ye;
          with_node = -1;
          newFSCORE = newFSCOREici;
          newPrec = newPrecici;
          newRec = newRecici;
          newTP = newTPici;
          newFP = newFPici;
          newFN = newFNici;
          newTN = newTNici;
        }
      }

      // *****************************************
      // IF ye Leave its module && ye MOve into xe's module
      // *****************************************
      if (mod_xe != mod_ye) {
        std::tie(varyingTPici, varyingFPici, varyingFNici) =
            GainJoining(g, clust, ye, mod_xe);
        newTPici = TP + varyingTPiciL;
        newFPici = FP + varyingFPiciL;
        newFNici = FN + varyingFNiciL;
        newTNici = TN - varyingFPiciL;
        newTPici = newTPici + varyingTPici;
        newFPici = newFPici + varyingFPici;
        newFNici = newFNici + varyingFNici;
        newTNici = newTNici - varyingFPici;
        std::tie(newPrecici, newRecici, newFSCOREici) =
            PRF(newTPici, newFPici, newFNici, Beta);
        if ((newFSCOREici > newFSCORE) ||
            ((newPrecici > newPrec) & (newRecici >= newRec)) ||
            ((newPrecici >= newPrec) & (newRecici > newRec))) {
          move_node = ye;
          with_node = xe;
          newFSCORE = newFSCOREici;
          newPrec = newPrecici;
          newRec = newRecici;
          newTP = newTPici;
          newFP = newFPici;
          newFN = newFNici;
          newTN = newTNici;
        }
      }

      // ============================================================================
      // EDIT
      // ============================================================================
      if ((newFSCORE > FSCORE) || ((newPrec > Prec) & (newRec >= Rec)) ||
          ((newPrec >= Prec) & (newRec > Rec))) {
        FSCORE = newFSCORE;
        Prec = newPrec;
        Rec = newRec;
        TP = newTP;
        FP = newFP;
        FN = newFN;
        TN = newTN;
        long long Oldmemb = (*clust).membP[move_node];

        if (with_node == -1) { // move_node ISOLATES itself in its own module
          (*clust).cP[Oldmemb].erase(move_node);
          long long i_freeClust = std::distance(
              (*clust).aliveP.begin(),
              std::find((*clust).aliveP.begin(), (*clust).aliveP.end(), false));
          (*clust).cP[i_freeClust].insert(move_node);
          (*clust).membP[move_node] = i_freeClust;
          //
          (*clust).aliveP[i_freeClust] = true;
          (*clust).nbmP = (*clust).nbmP + 1;
        } else { // move_node MOVES to go with with_node
          (*clust).cP[Oldmemb].erase(move_node);
          (*clust).cP[(*clust).membP[with_node]].insert(move_node);
          (*clust).membP[move_node] = (*clust).membP[with_node];
          if ((*clust).cP[Oldmemb].empty()) {
            (*clust).aliveP[Oldmemb] = false;
            (*clust).nbmP = (*clust).nbmP - 1;
          }
        }
      }
      if (VerboseInt) {
        bar.update("      MOVING NODES", IT);
      }
    }
    if (VerboseInt) {
      bar.done();
    }
    // ===================================================================
    if (VerboseInt) {
      std::cerr << "      MOVE NODES-->|CP|=" << std::to_string((*clust).nbmP)
                << ": ";
      std::cerr << "P=" << std::to_string(Prec)
                << ", R=" << std::to_string(Rec);
      std::cerr << ", F_" << std::to_string(scale) << "="
                << std::to_string(FSCORE);
      std::cerr << " (" << difftime(time(NULL), timerintermed) << " seconds)"
                << std::endl;
    }
    // ===================================================================
    CONT = ((FSCORE - CONTFSCORE) > Epsilon);
  }
  return std::make_tuple(TP, TN, FP, FN);
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// initSupergraph
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void initSupergraph(graph *g, clusteringP *clust, Supergraph *G) {
  /* */
  emptySupergraph(G);
  (*G).Super_nbv = (*clust).nbmP;
  (*G).Super_nodes.resize((*clust).nbmP);
  //
  std::map<long long, long long> D;
  long long ii = 0;
  for (std::size_t i = 0; i < (*clust).cP.size(); i++) {
    if ((*clust).aliveP[i]) {
      (*G).Super_nodes[ii] = i;
      D[i] = ii;
      ii = ii + 1;
    }
  }
  //
  vect_tuple_lld V;
  for (long unsigned int k = 0; k < (*g).nbv; ++k) {
    for (std::size_t i = 0; i < (*g).neighbors[k].size(); i++) {
      if (static_cast<long long>(k) < (*g).neighbors[k][i]) {
        if (!(D[(*clust).membP[k]] ==
              D[(*clust).membP[(*g).neighbors[k][i]]])) {
          V.push_back(std::make_tuple(D[(*clust).membP[k]],
                                      D[(*clust).membP[(*g).neighbors[k][i]]],
                                      (*g).weight[k][i]));
          V.push_back(std::make_tuple(D[(*clust).membP[(*g).neighbors[k][i]]],
                                      D[(*clust).membP[k]], (*g).weight[k][i]));
        }
      }
    }
  }
  std::sort(V.begin(), V.end(), AAcomp_tuple_lld_ON_ll);
  //
  (*G).Super_neighbors.resize((*clust).nbmP);
  (*G).Super_weight.resize((*clust).nbmP);
  (*G).Super_nbe = 0;

  long long x, y, nbe_x_y = 0, Super_nbe = 0;
  double w;
  if (V.size() > 0) {
    std::tie(x, y, w) = V[0];
    w = 0;
    for (std::size_t i = 0; i < V.size(); i++) {
      long long xi, yi;
      double wi;
      std::tie(xi, yi, wi) = V[i];
      if ((x == xi) && (y == yi)) {
        w = w + wi;
        nbe_x_y = nbe_x_y + 1;
      } else {
        Super_nbe = Super_nbe + 1;
        double w_no_edge = (((*clust).cP[(*G).Super_nodes[x]].size() *
                             (*clust).cP[(*G).Super_nodes[y]].size()) -
                            nbe_x_y) *
                           (*g).meanWeight;

        (*G).Super_neighbors[x].push_back(y);
        (*G).Super_weight[x].push_back(std::make_tuple(w, w_no_edge));

        x = xi;
        y = yi;
        w = wi;
        nbe_x_y = 1;
      }
    }
    Super_nbe = Super_nbe + 1;
    double w_no_edge = (((*clust).cP[(*G).Super_nodes[x]].size() *
                         (*clust).cP[(*G).Super_nodes[y]].size()) -
                        nbe_x_y) *
                       (*g).meanWeight;

    (*G).Super_neighbors[x].push_back(y);
    (*G).Super_weight[x].push_back(std::make_tuple(w, w_no_edge));

    (*G).Super_nbe = Super_nbe;
  }
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// initSuperClust
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void initSuperClust(Supergraph *G, clusteringP *SuperClust) {
  emptyClustP(SuperClust);
  (*SuperClust).nbvP = (*G).Super_nbv;
  (*SuperClust).nbmP = (*G).Super_nbv;
  (*SuperClust).cP.resize((*G).Super_nbv);
  (*SuperClust).membP.resize((*G).Super_nbv);
  (*SuperClust).aliveP.resize((*G).Super_nbv);
  for (long unsigned int k = 0; k < (*G).Super_nbv; ++k) {
    (*SuperClust).aliveP[k] = true;
    (*SuperClust).membP[k] = k;
    set_l mod_k;
    mod_k.insert(k);
    (*SuperClust).cP[k] = mod_k;
  }
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// MakeSuperPartition
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
tuple_dddd MakeSuperPartition(Supergraph *G, clusteringP *CLUST,
                              tuple_dddd TP_TN_FP_FN, double scale,
                              clusteringP *clust, double meanWeight,
                              double Epsilon, vect_l *idgIDG,
                              vect_tuple_ll *OrdEdges, long long VerboseInt) {

  double Beta = s2beta(scale);
  double TP = 0, TN = 0, FP = 0, FN = 0;
  std::tie(TP, TN, FP, FN) = TP_TN_FP_FN;
  double Prec = 0, Rec = 0, FSCORE = 0;
  std::tie(Prec, Rec, FSCORE) = PRF(TP, FP, FN, Beta);

  double newTP = 0, newFP = 0, newFN = 0, newTN = 0;
  double newPrec = 0, newRec = 0, newFSCORE = 0;

  double varyingTP, varyingFP, varyingFN;

  bool CONT = true;
  while (CONT) {
    time_t timerintermed = time(NULL);
    double CONTFSCORE = FSCORE;
    // ============================================================================
    // // loop on edges
    // ============================================================================
    FastProgressPercent bar((*OrdEdges).size());
    for (std::size_t IT = 0; IT < (*OrdEdges).size();
         ++IT) { // loop on OrdEdges
      newFSCORE = FSCORE;
      newPrec = Prec;
      newRec = Rec;
      long long mod_X = (*CLUST).membP[(*idgIDG)[std::get<0>((*OrdEdges)[IT])]];
      long long mod_Y = (*CLUST).membP[(*idgIDG)[std::get<1>((*OrdEdges)[IT])]];

      if ((mod_X != mod_Y)) {
        // *****************************************
        // mod_X MERGE with mod_Y
        // *****************************************
        std::tie(varyingTP, varyingFP, varyingFN) =
            SuperGainMerging(G, CLUST, mod_X, mod_Y, clust, meanWeight);
        newTP = TP + varyingTP;
        newFP = FP + varyingFP;
        newFN = FN + varyingFN;
        newTN = TN - varyingFP;
        std::tie(newPrec, newRec, newFSCORE) = PRF(newTP, newFP, newFN, Beta);
        if ((newFSCORE > FSCORE) || ((newPrec > Prec) & (newRec >= Rec)) ||
            ((newPrec >= Prec) & (newRec > Rec))) {
          FSCORE = newFSCORE;
          Prec = newPrec;
          Rec = newRec;
          TP = newTP;
          FP = newFP;
          FN = newFN;
          TN = newTN;
          for (auto itx = (*CLUST).cP[mod_X].begin();
               itx != (*CLUST).cP[mod_X].end(); itx++) {
            (*CLUST).cP[mod_Y].insert(*itx);
            (*CLUST).membP[*itx] = mod_Y;
          }
          (*CLUST).aliveP[mod_X] = false;
          (*CLUST).nbmP = (*CLUST).nbmP - 1;
        }
      }
      if (VerboseInt) {
        bar.update("      MERGING MODULES", IT);
      }
    }
    if (VerboseInt) {
      bar.done();
    }

    // ===================================================================
    if (VerboseInt) {
      std::cerr << "      MERGE MODULES-->|CP|="
                << std::to_string((*clust).nbmP) << ": ";
      std::cerr << "P=" << std::to_string(Prec)
                << ", R=" << std::to_string(Rec);
      std::cerr << ", F_" << std::to_string(scale) << "="
                << std::to_string(FSCORE);
      std::cerr << " (" << difftime(time(NULL), timerintermed) << " seconds)"
                << std::endl;
    }
    // ===================================================================
    CONT = ((FSCORE - CONTFSCORE) > Epsilon);
  }
  return std::make_tuple(TP, TN, FP, FN);
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// Superclust_2_clust
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void Superclust_2_clust(graph *g, clusteringP *clust, Supergraph *G,
                        clusteringP *SuperClust) {
  clusteringP newclust;
  newclust.nbvP = (*g).nbv;
  newclust.nbmP = 0;
  newclust.cP.resize((*g).nbv);
  newclust.membP.resize((*g).nbv);
  newclust.aliveP.resize((*g).nbv);
  for (long unsigned int i = 0; i < (*g).nbv; i++) {
    newclust.aliveP[i] = false;
  }
  long long ii = 0;
  for (long unsigned int i = 0; i < (*SuperClust).nbvP; i++) {
    if ((*SuperClust).aliveP[i]) {
      set_l mod;
      for (auto it = (*SuperClust).cP[i].begin();
           it != (*SuperClust).cP[i].end(); it++) {
        for (auto itit = (*clust).cP[(*G).Super_nodes[*it]].begin();
             itit != (*clust).cP[(*G).Super_nodes[*it]].end(); itit++) {
          mod.insert(*itit);
          newclust.membP[*itit] = ii;
        }
      }
      newclust.nbmP = newclust.nbmP + 1;
      newclust.aliveP[ii] = true;
      newclust.cP[ii] = mod;
      ii = ii + 1;
    }
  }
  emptyClustP(clust);
  (*clust).nbvP = newclust.nbvP;
  (*clust).nbmP = newclust.nbmP;
  (*clust).cP.resize((*g).nbv);
  (*clust).membP.resize((*g).nbv);
  (*clust).aliveP.resize((*g).nbv);
  (*clust).with_nodes.resize((*g).nbv);
  for (long unsigned int i = 0; i < newclust.nbvP; i++) {
    (*clust).membP[i] = newclust.membP[i];
    (*clust).aliveP[i] = newclust.aliveP[i];
    (*clust).cP[i] = newclust.cP[i];
  }
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// MakeOverlaps
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

tuple_dddd MakeOverlaps(graph *g, clusteringP *clust, tuple_dddd TP_TN_FP_FN,
                        double Beta, long long VerboseInt) {

  vect_set_l membO;
  membO.resize((*clust).nbvP);
  for (long unsigned int i = 0; i < (*clust).nbvP; i++) {
    if ((*clust).aliveP[i]) {
      for (auto it = (*clust).cP[i].begin(); it != (*clust).cP[i].end(); it++) {
        membO[*it].insert(i);
      }
    }
  }

  double TP, TN, FP, FN;
  std::tie(TP, TN, FP, FN) = TP_TN_FP_FN;
  double Prec, Rec, FSCORE;
  std::tie(Prec, Rec, FSCORE) = PRF(TP, FP, FN, Beta);

  double newTP, newFP, newFN;
  double newPrec, newRec, newFSCORE;

  double TOT_FN_VIRGIN =
      (((((*g).nbv) * ((*g).nbv - 1) * 0.5) - ((*g).nbe)) * (*g).meanWeight) +
      (*g).sumWeight;

  double varyingTP, varyingFP, varyingFN;
  double xxxTP, xxxFP, xxxFN;
  long long itint = 0;
  vect_tuple_lld vectXMF;

  FastProgressPercent bar((*g).nbv);
  for (long unsigned int k = 0; k < (*g).nbv; ++k) {
    itint = itint + 1;
    long long x = k;
    tuple_ddd varying = std::make_tuple(0, 0, 0);
    tuple_ddd xyz = std::make_tuple(0.0, 0.0, 0.0);
    bool do_it = false;
    long long mod_x = (*clust).membP[x];
    set_l AlreadySeen;
    AlreadySeen.insert(mod_x);
    for (long unsigned int IT = 0; IT < (*g).neighbors[x].size();
         ++IT) { // loop on neighbors y of x
      long long y = (*g).neighbors[x][IT];
      long long mod_y = (*clust).membP[y];
      do_it = (AlreadySeen.end() == AlreadySeen.find(mod_y));
      if (do_it) {
        AlreadySeen.insert(mod_y);
        varying = GainJoining(g, clust, x, mod_y);
        std::tie(varyingTP, varyingFP, varyingFN) = varying;
        //
        newTP = TP + varyingTP;
        newFP = FP + varyingFP;
        newFN = FN + varyingFN;
        xyz = PRF(newTP, newFP, newFN, Beta);
        std::tie(newPrec, newRec, newFSCORE) = xyz;
        // =================================================
        if (newFSCORE > FSCORE) {
          tuple_lld nid = std::make_tuple(x, mod_y, newFSCORE);
          vectXMF.push_back(nid);
        }
        // =================================================
      }
    }
    if (VerboseInt) {
      bar.update("      EXTENDING", k);
    }
  }
  if (VerboseInt) {
    bar.done();
  }

  std::sort(vectXMF.begin(), vectXMF.end(), Dcomp_tuple_lld_ON_d);

  FastProgressPercent bar2(vectXMF.size());
  for (long unsigned int IT = 0; IT < vectXMF.size(); ++IT) {
    long long x = std::get<0>(vectXMF[IT]);
    long long mod_y = std::get<1>(vectXMF[IT]);
    bool absent = (membO[mod_y].end() == ((membO))[mod_y].find(x));
    if (absent) {
      tuple_ddd varying = GainOverlaps(g, clust, &membO, x, mod_y);
      std::tie(varyingTP, varyingFP, varyingFN) = varying;
      //
      newTP = TP + varyingTP;
      newFP = FP + varyingFP;
      newFN = FN + varyingFN;
      tuple_ddd xyz = PRF(newTP, newFP, newFN, Beta);
      std::tie(newPrec, newRec, newFSCORE) = xyz;
      // =================================================
      if (newFSCORE > FSCORE) // if ((newRec > Rec) & (!(newPrec < Prec)))
      {
        std::tie(xxxTP, xxxFP, xxxFN) = ModContribution(g, clust, mod_y);
        tuple_ddd yyy = PRF(xxxTP, xxxFP, (TOT_FN_VIRGIN + xxxFN), Beta);
        double yyyFSCORE = std::get<2>(yyy);
        //
        double xxxTPnew = xxxTP + varyingTP;
        double xxxFPnew = xxxFP + varyingFP;
        double xxxFNnew = xxxFN + varyingFN;
        tuple_ddd yyynew =
            PRF(xxxTPnew, xxxFPnew, (TOT_FN_VIRGIN + xxxFNnew), Beta);
        // double yyyPrecnew=std::get<0>(yyynew);
        // double yyyRecnew=std::get<1>(yyynew);
        double yyyFSCOREnew = std::get<2>(yyynew);
        //
        if (!(yyyFSCOREnew < yyyFSCORE)) {

          FSCORE = newFSCORE;
          Prec = newPrec;
          Rec = newRec;
          TP = newTP;
          FP = newFP;
          FN = newFN;
          membO[x].insert(mod_y);
          ((*clust).with_nodes)[mod_y].push_back(x);
        }
      }
    }
    if (VerboseInt) {
      bar2.update("      EXTENDING", IT);
    }
  }
  if (VerboseInt) {
    bar2.done();
  }
  return std::make_tuple(TP, TN, FP, FN);
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// OMEGA_PO
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
tuple_dd OMEGA_PO(graph *g, clusteringP *clust) {
  // a node i is adherent to a module m if i ∉ m and i is a neighbor of all the
  // nodes of m
  double omegaP = 0; // omegaP = sum(|adherent(m)|) for m in Cp
  double omegaO = 0; // omegaO = sum(|adherent(m)|) for m in Co
  for (long unsigned int i = 0; i < (*g).nbv; ++i) {
    long unsigned int mod_i = (*clust).membP[i];
    for (long unsigned int k = 0; k < ((*clust).cP).size(); ++k) {
      if ((*clust).aliveP[k] && (!(mod_i == k))) {
        bool OK = true;
        for (auto it = (*clust).cP[k].begin(); it != (*clust).cP[k].end();
             it++) {
          long long pos = nodeposition(&((*g).neighbors[i]), *it);
          if (pos == -1) { // NO edge {i,*it} in graph
            OK = false;
            break;
          }
        }
        if (OK) {
          omegaP = omegaP + 1;

          if (std::find((*clust).with_nodes[k].begin(),
                        (*clust).with_nodes[k].end(),
                        i) == (*clust).with_nodes[k].end()) {
            for (auto it = (*clust).with_nodes[k].begin();
                 it != (*clust).with_nodes[k].end(); it++) {
              long long pos = nodeposition(&((*g).neighbors[i]), *it);
              if (pos == -1) { // NO edge {i,*it} in graph
                OK = false;
                break;
              }
            }
            if (OK) {
              omegaO = omegaO + 1;
            }
          }
        }
      }
    }
  }
  return std::make_tuple(omegaP, omegaO);
}
