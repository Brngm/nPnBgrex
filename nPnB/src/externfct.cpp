// externfct.cpp
#include "include.h"
#include "handy_types.h"
#include "struct.h"
#include "tool1.h"
#include "tool2.h"
#include "tool3.h"

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// ClustPO_from_file
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
const char *ClustPO_from_file(const char *InfileClust, long unsigned int gnbv,
                              clusteringO *clust, const char *PO) {
  // initialize clust
  long long NBV = gnbv;
  vect_l NodeAssigned;
  NodeAssigned.resize(NBV);

  // initialize clust
  vect_set_l c;
  c.resize(NBV);
  (*clust).cO = c;
  vect_set_l membO;
  membO.resize(NBV);
  (*clust).membO = membO;
  vect_b alive;
  alive.resize(NBV);
  (*clust).aliveO = alive;
  for (long unsigned int i = 0; i < gnbv; i++) {
    (*clust).aliveO[i] = false;
    NodeAssigned[i] = -1;
  }

  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int fd = open(InfileClust, O_RDONLY);
  if (fd == -1) {
    close(fd);
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: Opening Infile Czero '" << InfileClust << "' failure"
           << std::endl;
    ERRORS = ERRORA.str();
    return ERRORS.c_str();
  }

  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    close(fd);
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: Opening Infile Czero '" << InfileClust << "' failure"
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
    ERRORA << "ERROR: Opening Infile Czero '" << InfileClust << "' failure"
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

    while (ptr < end && *ptr != '\n') {
      ++ptr;
    }

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
          // conversion string_view -> long long (without stoi)
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
              ERRORA << "ERROR: In Czero file '" << InfileClust;
              ERRORA << ": " << token << "<0" << "' (line " << line_number
                     << ")\n";
              ERRORS = ERRORA.str();
              return ERRORS.c_str();
            }

            if (y > (NBV - 1)) {
              close(fd);
              static std::string ERRORS;
              std::ostringstream ERRORA;
              ERRORA << "ERROR In Czero file '" << InfileClust << ": ";
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
              ERRORA << "ERROR In Czero file '" << InfileClust << ": ";
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

            (*clust).cO[IdMod].insert(y);
            (*clust).aliveO[IdMod] = true;
            (*clust).membO[y].insert(IdMod);
            NodeAssigned[y] = IdMod;
          } else {
            if ((strcmp(PO, "O") == 0)) { // WITH EXTENSION (overlaps)
              if (!L2.insert(token).second) {
                close(fd);
                static std::string ERRORS;
                std::ostringstream ERRORA;
                ERRORA << "ERROR: duplicate token in overlapping extension -> '"
                       << y << "' (line " << line_number << ")\n";
                ERRORS = ERRORA.str();
                return ERRORS.c_str();
              }

              if (L1.find(token) != L1.end()) {
                close(fd);
                static std::string ERRORS;
                std::ostringstream ERRORA;
                ERRORA << "ERROR: token in the overlapping extension, already "
                          "present in its partitioning module -> '"
                       << y << "' (line " << line_number << ")\n";
                ERRORS = ERRORA.str();
                return ERRORS.c_str();
              }

              // std::cout << y << " in L2 IdMod=" << IdMod << "\n";

              if ((y < 0)) {
                close(fd);
                static std::string ERRORS;
                std::ostringstream ERRORA;
                ERRORA << "ERROR: In Czero file '" << InfileClust;
                ERRORA << ": " << token << "<0" << "' (line " << line_number
                       << ")\n";
                ERRORS = ERRORA.str();
                return ERRORS.c_str();
              }

              if (y > (NBV - 1)) {
                close(fd);
                static std::string ERRORS;
                std::ostringstream ERRORA;
                ERRORA << "ERROR In Czero file '" << InfileClust << ": ";
                ERRORA << "token " << token << ">" << NBV - 1
                       << " when id node in [0," << NBV - 1 << "], because "
                       << NBV << " nodes in the graph";
                ERRORA << " (line " << line_number << ")\n";
                ERRORS = ERRORA.str();
                return ERRORS.c_str();
              }

              (*clust).cO[IdMod].insert(y);
              (*clust).aliveO[IdMod] = true;
              (*clust).membO[y].insert(IdMod);
            }
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
      ERRORA << "ERROR: missing '!' (line " << line_number << ")\n";
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

  (*clust).nbvO = gnbv; // the number of vertices of the support graph (g.nbv)
  (*clust).nbmO = 0;    // the number of living modules
  for (long unsigned int i = 0; i < gnbv; i++) {
    if ((*clust).aliveO[i]) {
      (*clust).nbmO = (*clust).nbmO + 1;
    }
    if ((*clust).membO[i].size() == 0) {
      static std::string ERRORS;
      std::ostringstream ERRORA;
      ERRORA << "ERROR: In Czero file '" << InfileClust << "', node " << i
             << " is not affected to a module " << std::endl;
      ERRORS = ERRORA.str();
      return ERRORS.c_str();
    }
  }

  return "";
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// extern: TPTNFPFN_Intrinsic (To be called from Python)
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
extern "C" {
int TPTNFPFN_Intrinsic(char *InfileGraph, char *InfileClust, char *POclust,
                       char *error_msg, int error_msg_size, double *out) {
  // load InfileGraph
  graph g;
  vect_tuple_ll OrdEdges;
  const char *ERRORgraph = graph_from_file(InfileGraph, &g, &OrdEdges, "1");
  if (!(ERRORgraph != nullptr && ERRORgraph[0] == '\0')) {
    strncpy(error_msg, ERRORgraph, error_msg_size - 1);
    error_msg[error_msg_size - 1] = '\0';
    return 1; // code error
  }

  // init clust
  clusteringO clust;
  const char *ERRORclust =
      ClustPO_from_file(InfileClust, g.nbv, &clust, POclust);

  if (!(ERRORclust != nullptr && ERRORclust[0] == '\0')) {
    strncpy(error_msg, ERRORclust, error_msg_size - 1);
    error_msg[error_msg_size - 1] = '\0';
    return 1; // code error
  }

  // /*
  std::cerr << "TPTNFPFN_Intrinsic(G,C): " << "G:[" << g.nbv << " vertices, "
            << g.nbe << " edges], ";
  std::cerr << "C:[" << clust.nbvO << " vertices, ";
  std::cerr << std::to_string(clust.nbmO) << " modules]" << std::endl;
  // */

  // init TP, FP, FN, TN
  double TP = 0, TN = 0, FP = 0, FN = 0;
  TN = (((double(g.nbv) * double((g.nbv - 1)) * 0.5) - double(g.nbe)) *
        g.meanWeight);
  FN = g.sumWeight;

  long long pos;
  // ------------------------------------------------------------------------------------
  for (long unsigned int ii = 0; ii < clust.cO.size(); ii++) {
    if (clust.aliveO[ii]) {
      for (auto i : clust.cO[ii]) {
        for (auto j : clust.cO[ii]) {
          if (i < j) {
            double xxx = 0;
            for (auto k : clust.membO[i]) {
              if (clust.membO[j].end() != clust.membO[j].find(k)) {
                xxx = xxx + 1;
              }
            }
            pos = nodeposition(&(g.neighbors[i]), j);
            if (!(pos == -1)) { // one edge {i,j} in graph
              TP = TP + (g.weight[i].at(pos) / xxx);
              FN = FN - (g.weight[i].at(pos) / xxx);
            } else { // no edge {i,j} in graph
              FP = FP + (g.meanWeight / xxx);
              TN = TN - (g.meanWeight / xxx);
            }
          }
        }
      }
    }
  }
  out[0] = TP;
  out[1] = TN;
  out[2] = FP;
  out[3] = FN;
  return 0;
}
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// extern: TPTNFPFN_Extrinsic (To be called from Python)
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
extern "C" {
int TPTNFPFN_Extrinsic(char *InfileGold, char *POgold, char *InfileClust,
                       char *POclust, int gnbv, char *error_msg,
                       int error_msg_size, double *out) {

  long unsigned int unsgnbv = gnbv;

  // init GoldClust
  clusteringO clustGOLD;
  const char *ERRORclustGOLD =
      ClustPO_from_file(InfileGold, unsgnbv, &clustGOLD, POgold);
  if (!(ERRORclustGOLD != nullptr && ERRORclustGOLD[0] == '\0')) {
    strncpy(error_msg, ERRORclustGOLD, error_msg_size - 1);
    error_msg[error_msg_size - 1] = '\0';
    return 1; // code error
  }

  // init clust
  clusteringO clust;
  const char *ERRORclust =
      ClustPO_from_file(InfileClust, unsgnbv, &clust, POclust);
  if (!(ERRORclust != nullptr && ERRORclust[0] == '\0')) {
    strncpy(error_msg, ERRORclust, error_msg_size - 1);
    error_msg[error_msg_size - 1] = '\0';
    return 1; // code error
  }

  double n_edges = 0;
  for (long unsigned int ii = 0; ii < clustGOLD.cO.size(); ii++) {
    if (clustGOLD.aliveO[ii]) {
      for (auto i : clustGOLD.cO[ii]) {
        for (auto j : clustGOLD.cO[ii]) {
          if (i < j) {
            double xxx = 0;
            for (auto k : clustGOLD.membO[i]) {
              if (clustGOLD.membO[j].end() != clustGOLD.membO[j].find(k)) {
                xxx = xxx + 1;
              }
            }
            n_edges = n_edges + (1 / xxx);
          }
        }
      }
    }
  }

  // /*
  std::cerr << "TPTNFPFN_Extrinsic(Gold,C): " << "Gold:[" << clustGOLD.nbvO
            << " vertices, ";
  std::cerr << std::to_string(clustGOLD.nbmO) << " module(s)], ";
  std::cerr << "C:[" << clust.nbvO << " vertices, ";
  std::cerr << std::to_string(clust.nbmO) << " modules]" << std::endl;
  // */

  // init TP, FP, FN
  double TP = 0, TN = 0, FP = 0, FN = 0;
  TN = (((double(unsgnbv) * double((unsgnbv - 1)) * 0.5) - n_edges));
  FN = n_edges;
  bool Egold;
  // ------------------------------------------------------------------------------------
  for (long unsigned int ii = 0; ii < clust.cO.size(); ii++) {
    if (clust.aliveO[ii]) {
      for (auto i : clust.cO[ii]) {
        for (auto j : clust.cO[ii]) {
          if (i < j) {
            double xxx = 0;
            for (auto k : clust.membO[i]) {
              if (clust.membO[j].end() != clust.membO[j].find(k)) {
                xxx = xxx + 1;
              }
            }
            Egold = false;
            for (auto k : clustGOLD.membO[i]) {
              if (clustGOLD.membO[j].end() != clustGOLD.membO[j].find(k)) {
                Egold = true;
                break;
              }
            }
            //
            if (Egold) {
              TP = TP + (1.0 / xxx);
              FN = FN - (1.0 / xxx);
            } else {
              FP = FP + (1.0 / xxx);
              TN = TN - (1.0 / xxx);
            }
          }
        }
      }
    }
  }
  out[0] = TP;
  out[1] = TN;
  out[2] = FP;
  out[3] = FN;
  return 0;
}
}
