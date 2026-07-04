// tool1.cpp
#include "include.h"
#include "handy_types.h"
#include "struct.h"
#include "tool2.h"
#include "tool3.h"

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// BECPART
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
tuple_dddd BECPART(graph *g, clusteringP *clust, vect_tuple_ll *OrdEdges,
                   double scaleP, double Epsilon, long long Verbose) {

  double BetaP = s2beta(scaleP);
  time_t timerbegin = time(NULL);
  time_t timerintermed = time(NULL);

  if (Verbose) {
    std::cerr << "\n"
              << "\u25B6" << " MAKING ClustZero with Connex modules in G:"
              << " |ClustZero|=" << std::to_string((*clust).nbmP) << std::endl;
  }

  // initialize clust, TP, TN, FP, FN (where modules of clust are connex in g)
  tuple_dddd TP_TN_FP_FN;
  TP_TN_FP_FN = clust_with_connex_modules(g, clust);
  double TP, TN, FP, FN;
  std::tie(TP, TN, FP, FN) = TP_TN_FP_FN;

  // initialize Prec, Rec, FSCORE (the intrinsec scores of clust with respect to
  // g)
  double Prec, Rec, FSCORE;
  std::tie(Prec, Rec, FSCORE) = PRF(TP, FP, FN, BetaP);

  double FSCORE1;
  if ((Prec + Rec) > 0) {
    FSCORE1 = (2) * ((Prec * Rec) / (Prec + Rec));
  } else {
    FSCORE1 = 0;
  }

  if (Verbose) {
    std::cerr << std::fixed << std::setprecision(0)
              << difftime(time(NULL), timerintermed) << " seconds" << std::endl;

    std::cerr << "|CP|=" << std::to_string((*clust).nbmP) << std::endl;

    std::cerr << "TP=" << std::to_string(TP);
    std::cerr << ", TN=" << std::to_string(TN);
    std::cerr << ", FP=" << std::to_string(FP);
    std::cerr << ", FN=" << std::to_string(FN) << std::endl;

    std::cerr << "P=" << std::to_string(Prec) << ", R=" << std::to_string(Rec);
    std::cerr << ", F_0.5=" << std::to_string(FSCORE1);
    std::cerr << ", F_" << std::to_string(scaleP) << "="
              << std::to_string(FSCORE) << std::endl;
  }

  if (Verbose) {
    std::cerr << "\n"
              << "\u25B6"
              << " PARTITIONING: sp=" << std::to_string(scaleP) << std::endl;
  }

  // ===================================================================
  Supergraph G;
  clusteringP CLUST;
  bool CONT = true;

  while (CONT) {
    double CONTFSCORE = FSCORE;

    TP_TN_FP_FN = MakePartition(g, clust, OrdEdges, TP_TN_FP_FN, scaleP,
                                Epsilon, Verbose);
    TP_TN_FP_FN = clust_with_connex_modules(g, clust);
    std::tie(TP, TN, FP, FN) = TP_TN_FP_FN;
    std::tie(Prec, Rec, FSCORE) = PRF(TP, FP, FN, BetaP);
    // ===================================================================
    if (Verbose) {
      std::cerr << "      -->|CP|=" << std::to_string((*clust).nbmP) << ": ";
      std::cerr << "P=" << std::to_string(Prec)
                << ", R=" << std::to_string(Rec);
      std::cerr << ", F_" << std::to_string(scaleP) << "="
                << std::to_string(FSCORE);
      std::cerr << " (" << std::fixed << std::setprecision(0)
                << difftime(time(NULL), timerintermed)
                << " seconds)\t\tNODES MOVED" << std::endl;
      timerintermed = time(NULL);
    }
    // ===================================================================

    initSupergraph(g, clust, &G);
    initSuperClust(&G, &CLUST);

    vect_l idgIDG;
    idgIDG.resize((*g).nbv);
    for (long unsigned int I = 0; I < G.Super_nbv; I++) {
      for (auto it = (*clust).cP[G.Super_nodes[I]].begin();
           it != (*clust).cP[G.Super_nodes[I]].end(); it++) {
        idgIDG[*it] = I;
      }
    }

    TP_TN_FP_FN = MakeSuperPartition(&G, &CLUST, TP_TN_FP_FN, scaleP, clust,
                                     (*g).meanWeight, Epsilon, &idgIDG,
                                     OrdEdges, Verbose);
    Superclust_2_clust(g, clust, &G, &CLUST);
    TP_TN_FP_FN = clust_with_connex_modules(g, clust);
    std::tie(TP, TN, FP, FN) = TP_TN_FP_FN;
    std::tie(Prec, Rec, FSCORE) = PRF(TP, FP, FN, BetaP);
    // ===================================================================
    if (Verbose) {
      std::cerr << "      -->|CP|=" << std::to_string((*clust).nbmP) << ": ";
      std::cerr << "P=" << std::to_string(Prec)
                << ", R=" << std::to_string(Rec);
      std::cerr << ", F_" << std::to_string(scaleP) << "="
                << std::to_string(FSCORE);
      std::cerr << " (" << std::fixed << std::setprecision(0)
                << difftime(time(NULL), timerintermed)
                << " seconds)\t\tMODULES MERGED" << std::endl;
      timerintermed = time(NULL);
    }
    // ===================================================================

    TP_TN_FP_FN = MakePartition(g, clust, OrdEdges, TP_TN_FP_FN, scaleP,
                                Epsilon, Verbose);
    TP_TN_FP_FN = clust_with_connex_modules(g, clust);
    std::tie(TP, TN, FP, FN) = TP_TN_FP_FN;
    std::tie(Prec, Rec, FSCORE) = PRF(TP, FP, FN, BetaP);
    // ===================================================================
    if (Verbose) {
      std::cerr << "      -->|CP|=" << std::to_string((*clust).nbmP) << ": ";
      std::cerr << "P=" << std::to_string(Prec)
                << ", R=" << std::to_string(Rec);
      std::cerr << ", F_" << std::to_string(scaleP) << "="
                << std::to_string(FSCORE);
      std::cerr << " (" << std::fixed << std::setprecision(0)
                << difftime(time(NULL), timerintermed)
                << " seconds)\t\tNODES MOVED" << std::endl;
      timerintermed = time(NULL);
    }
    // ===================================================================
    if (Epsilon == 0.0) { // We want to promote quality
      if (FSCORE == CONTFSCORE) {
        CONT = false;
      }
    } else { // We want to promote speed
      // CONT=((FSCORE-CONTFSCORE)>Epsilon); //  ~ 0.001 better than with :
      // CONT=false;
      CONT = false; // To speed up calculations: A single round in the while
                    // loop is sufficient for good quality.
    }
  }

  if ((Prec + Rec) > 0) {
    FSCORE1 = (2) * ((Prec * Rec) / (Prec + Rec));
  } else {
    FSCORE1 = 0;
  }

  if (Verbose) {
    std::cerr << std::fixed << std::setprecision(0)
              << difftime(time(NULL), timerbegin) << " seconds" << std::endl;

    std::cerr << "|CP|=" << std::to_string((*clust).nbmP) << std::endl;
    std::cerr << "TP=" << std::to_string(TP);
    std::cerr << ", TN=" << std::to_string(TN);
    std::cerr << ", FP=" << std::to_string(FP);
    std::cerr << ", FN=" << std::to_string(FN) << std::endl;

    std::cerr << "P=" << std::to_string(Prec) << ", R=" << std::to_string(Rec);
    std::cerr << ", F_0.5=" << std::to_string(FSCORE1);
    std::cerr << ", F_" << std::to_string(scaleP) << "="
              << std::to_string(FSCORE) << std::endl;
  }
  return std::make_tuple(TP, TN, FP, FN);
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// BECOVER
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
tuple_dddd BECOVER(graph *g, clusteringP *clust, tuple_dddd TP_TN_FP_FN,
                   double scaleO, long long Verbose) {

  time_t timerbegin = time(NULL);
  if (Verbose) {

    std::cerr << "\n"
              << "\u25B6"
              << " EXTENDING the modules: so=" << std::to_string(scaleO)
              << std::endl;
  }

  double BetaO = s2beta(scaleO);

  TP_TN_FP_FN = MakeOverlaps(g, clust, TP_TN_FP_FN, BetaO, Verbose);

  // ===================================================================
  double TP, TN, FP, FN, Prec, Rec, FSCORE;
  std::tie(TP, TN, FP, FN) = TP_TN_FP_FN;
  std::tie(Prec, Rec, FSCORE) = PRF(TP, FP, FN, BetaO);
  if (Verbose) {
    std::cerr << "      -->|CO|=" << std::to_string((*clust).nbmP) << ": ";
    std::cerr << "P=" << std::to_string(Prec) << ", R=" << std::to_string(Rec);
    std::cerr << ", F_" << std::to_string(scaleO) << "="
              << std::to_string(FSCORE);
    std::cerr << " (" << std::fixed << std::setprecision(0)
              << difftime(time(NULL), timerbegin)
              << " seconds)\t\tMODULES EXTENDED" << std::endl;
    std::cerr << std::fixed << std::setprecision(0)
              << difftime(time(NULL), timerbegin) << " seconds" << std::endl;
  }
  // ===================================================================
  return TP_TN_FP_FN;
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
// BEC
// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
const char *BEC(const char *InfileGraph, const char *OutfileClust,
                const char *ClustZero, const char *RankedEdeges,
                const char *scalePS, const char *scaleOS, const char *EpsilonS,
                const char *Verb) {

  // Check that the output file does not already exist
  std::ifstream OutC;
  OutC.open(OutfileClust, std::ios::in);
  if (!OutC.fail()) {
    OutC.close();
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: with output_clustering_file '" << OutfileClust
           << "' already exist" << std::endl;
    ERRORS = ERRORA.str();
    return ERRORS.c_str();
  }

  // Verbose
  bool Verbose = (!(strcmp(Verb, "0") == 0));
  time_t timeX = time(NULL);

  // Check scaleP
  double scaleP = -1;
  try {
    scaleP = std::stod(scalePS);
  } catch (const std::invalid_argument &e) {
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: Invalid conversion with scaleP '" << scalePS << "'"
           << std::endl;
    ERRORS = ERRORA.str();
    return ERRORS.c_str();
  } catch (const std::out_of_range &e) {
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: Value out of range with scaleP " << scalePS << std::endl;
    ERRORS = ERRORA.str();
    return ERRORS.c_str();
  }
  if ((scaleP < 0) || (scaleP > 1)) {
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: scaleP must be in [0,1] but scaleP=" << scalePS
           << std::endl;
    ERRORS = ERRORA.str();
    return ERRORS.c_str();
  }
  double BetaP = s2beta(scaleP);

  // Check scaleO
  double scaleO = -1;
  if (strcmp(scaleOS, "no") != 0) {
    try {
      scaleO = std::stod(scaleOS);
    } catch (const std::invalid_argument &e) {
      static std::string ERRORS;
      std::ostringstream ERRORA;
      ERRORA << "ERROR: Invalid conversion with scaleO '" << scaleOS << "'"
             << std::endl;
      ERRORS = ERRORA.str();
      return ERRORS.c_str();
    } catch (const std::out_of_range &e) {
      static std::string ERRORS;
      std::ostringstream ERRORA;
      ERRORA << "ERROR: Value out of range with scaleO " << scaleOS
             << std::endl;
      ERRORS = ERRORA.str();
      return ERRORS.c_str();
    }
    if ((scaleO < 0) || (scaleO > 1)) {
      static std::string ERRORS;
      std::ostringstream ERRORA;
      ERRORA << "ERROR: scaleO must be in [0,1] but scaleO=" << scaleOS
             << std::endl;
      ERRORS = ERRORA.str();
      return ERRORS.c_str();
    }
  }

  // Check Epsilon
  double Epsilon = -1;
  try {
    Epsilon = std::stod(EpsilonS);
  } catch (const std::invalid_argument &e) {
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: Invalid conversion with Epsilon '" << EpsilonS << "'"
           << std::endl;
    ERRORS = ERRORA.str();
    return ERRORS.c_str();
  } catch (const std::out_of_range &e) {
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: Value out of range with Epsilon " << EpsilonS
           << std::endl;
    ERRORS = ERRORA.str();
    return ERRORS.c_str();
  }
  if ((Epsilon < 0) || (Epsilon > 1)) {
    static std::string ERRORS;
    std::ostringstream ERRORA;
    ERRORA << "ERROR: Epsilon must be in [0,1] but Epsilon=" << EpsilonS
           << std::endl;
    ERRORS = ERRORA.str();
    return ERRORS.c_str();
  }

  // load InfileGraph
  graph g;
  vect_tuple_ll OrdEdges;
  if (Verbose) {
    std::cerr
        << "----------------------------------------------------------------"
        << std::endl;
    std::cerr << "\u25B6" << " LOADING graph: file path='" << InfileGraph << "'"
              << std::endl;
  }
  const char *ERRORS =
      graph_from_file(InfileGraph, &g, &OrdEdges, RankedEdeges);
  if (!(ERRORS != nullptr && ERRORS[0] == '\0')) {
    return ERRORS;
  }

  double edgesDensity = 0;
  if (g.nbv > 1) {
    edgesDensity = double(g.nbe) / (double(g.nbv) * (double(g.nbv) - 1) / 2);
  }

  if (Verbose) {
    std::cerr << std::fixed << std::setprecision(0)
              << difftime(time(NULL), timeX) << " seconds" << std::endl;
    std::cerr << "G: " << std::to_string(g.nbv) << " vertices, "
              << std::to_string(g.nbe) << " edges "
              << ", density=" << std::fixed << std::setprecision(10)
              << edgesDensity
              << ", mean weight=" << std::to_string(g.meanWeight) << std::endl;
  }

  if (false) { // To track order edges
    std::cerr << "ORDERED EDGES : ";
    for (long unsigned int IT = 0; IT < (OrdEdges).size(); ++IT) {
      std::cerr << "(" << std::get<0>((OrdEdges)[IT]) << ","
                << std::get<1>((OrdEdges)[IT]) << "), ";
    }
    std::cerr << std::endl;
  }

  // initialize clust
  clusteringP clust;
  if (strcmp(ClustZero, "0") == 0) { // each node in its own module
    clust.nbvP = g.nbv;
    clust.nbmP = g.nbv;
    vect_set_l c;
    c.resize(g.nbv);
    clust.cP = c;
    vect_b alive;
    alive.resize(g.nbv);
    clust.aliveP = alive;
    vect_l memb;
    memb.resize(g.nbv);
    clust.membP = memb;
    vect_vect_l with_nodes;
    with_nodes.resize(g.nbv);
    clust.with_nodes = with_nodes;

    for (long unsigned int i = 0; i < g.nbv; i++) {
      set_l mod_i;
      mod_i.insert(i);
      clust.cP[i] = mod_i;
      clust.membP[i] = i;
      clust.aliveP[i] = true;
    }
  } else { // load ClustZero
    timeX = time(NULL);
    if (Verbose) {
      std::cerr << "\n"
                << "\u25B6" << " LOADING ClustZero: file path='" << ClustZero
                << "'" << std::endl;
    }
    const char *ERRORS = ClustP_from_file(ClustZero, g.nbv, &clust);
    if (!(ERRORS != nullptr && ERRORS[0] == '\0')) {
      return ERRORS;
    }

    if (Verbose) {
      std::cerr << std::fixed << std::setprecision(0)
                << difftime(time(NULL), timeX) << " seconds" << std::endl;
      std::cerr << "|ClustZero|=" << clust.nbmP << std::endl;
    }
  }

  tuple_dddd TP_TN_FP_FN;
  time_t timerbegin = time(NULL);
  // ==================================================================================
  // partition modules
  // ==================================================================================
  time_t timerbeginBECPART = time(NULL);
  time_t timeComputBECPART = 0;

  TP_TN_FP_FN = BECPART(&g, &clust, &OrdEdges, scaleP, Epsilon, Verbose);
  double TP_p, TN_p, FP_p, FN_p;
  std::tie(TP_p, TN_p, FP_p, FN_p) = TP_TN_FP_FN;
  TN_p = ((((g.nbv) * (g.nbv - 1) * 0.5) - (g.nbe)) * g.meanWeight) - FP_p;
  double Prec_p, Rec_p, FSCOREsp_p;
  std::tie(Prec_p, Rec_p, FSCOREsp_p) = PRF(TP_p, FP_p, FN_p, BetaP);

  double FSCORE1_p;
  if ((Prec_p + Rec_p) > 0) {
    FSCORE1_p = (2) * ((Prec_p * Rec_p) / (Prec_p + Rec_p));
  } else {
    FSCORE1_p = 0;
  }
  timeComputBECPART = difftime(time(NULL), timerbeginBECPART);

  // ==================================================================================

  // ==================================================================================
  // overlaps : extension of the partition modules
  // ==================================================================================
  time_t timerbeginBECOVER = time(NULL);
  time_t timeComputBECOVER = 0;

  double TP_o = TP_p;             // True positive of CO
  double TN_o = TN_p;             // True negative of CO
  double FP_o = FP_p;             // False positive of CO
  double FN_o = FN_p;             // False negative of CO
  double Prec_o = Prec_p;         // Precision of CO
  double Rec_o = Rec_p;           // Recall of CO
  double FSCORE1_o = FSCORE1_p;   // F0.5 score of CO
  double FSCOREsp_o = FSCOREsp_p; // Fsp score of CO

  if (strcmp(scaleOS, "no") != 0) {
    TP_TN_FP_FN = BECOVER(&g, &clust, TP_TN_FP_FN, scaleO, Verbose);
    std::tie(TP_o, TN_o, FP_o, FN_o) = TP_TN_FP_FN;
    TN_o = ((((g.nbv) * (g.nbv - 1) * 0.5) - (g.nbe)) * g.meanWeight) - FP_o;
    std::tie(Prec_o, Rec_o, FSCOREsp_o) = PRF(TP_o, FP_o, FN_o, BetaP);

    if ((Prec_o + Rec_o) > 0) {
      FSCORE1_o = (2) * ((Prec_o * Rec_o) / (Prec_o + Rec_o));
    } else {
      FSCORE1_o = 0;
    }
    timeComputBECOVER = difftime(time(NULL), timerbeginBECOVER);
  }

  // A node i is adherent to a module m if i ∉ m and i is a neighbor of all the
  // nodes of m
  double omega_p; // omega_p = sum(|adherent(m)|) for m in Cp
  double omega_o; // omega_o = sum(|adherent(m)|) for m in Co
  std::tie(omega_p, omega_o) = OMEGA_PO(&g, &clust);

  // ==================================================================================

  // ==================================================================================
  // Save Clustering
  // ==================================================================================
  time_t now;
  struct tm *tm_info;
  char buffer[20];           // assez grand pour "YYYY-MM-DD HH:MM:SS"
  time(&now);                // récupère le temps actuel
  tm_info = localtime(&now); // convertit en date/heure locale
  strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", tm_info);

  std::ofstream out;
  out.open(OutfileClust);
  out << "date\t" << buffer << std::endl;
  out << "bec\t" << scalePS;
  out << " " << InfileGraph;
  out << " " << OutfileClust;
  out << " -o" << scaleOS;
  out << " -e" << Epsilon;
  out << " -r" << RankedEdeges;
  out << " -z" << ClustZero << std::endl;

  out << std::endl;

  out << "input_InfileGraph\t" << InfileGraph << std::endl;
  out << "input_OutfileClust\t" << OutfileClust << std::endl;
  out << "input_RankedEdeges\t" << RankedEdeges << std::endl;
  out << "input_ClustZero\t" << ClustZero << std::endl;

  out << "input_sp\t" << scalePS << std::endl;
  out << "input_so\t" << scaleOS << std::endl;
  out << "input_epsilon\t" << EpsilonS << std::endl;

  out << std::endl;

  out << "gnbv\t" << g.nbv << std::endl;
  out << "gnbe\t" << g.nbe << std::endl;
  out << "density\t" << std::fixed << std::setprecision(10) << edgesDensity
      << std::endl;

  out << std::endl;

  out << "secondsP\t" << std::to_string(timeComputBECPART)
      << std::endl; // time computing for partition
  out << "secondsO\t" << std::to_string(timeComputBECOVER)
      << std::endl; // time computing for the extensions

  out << std::endl;

  out << "|C|" << "\t" << std::to_string(clust.nbmP)
      << std::endl; // Number of Module

  out << std::endl;

  out << "omega_p\t" << std::to_string(omega_p)
      << std::endl; // sum(|adherent(m)|) for m in Cp
  out << "TP_p\t" << std::to_string(TP_p) << std::endl; // True positive of CP
  out << "TN_p\t" << std::to_string(TN_p) << std::endl; // True negative  of CP
  out << "FP_p\t" << std::to_string(FP_p) << std::endl; // False positive  of CP
  out << "FN_p\t" << std::to_string(FN_p) << std::endl; // False negative  of CP
  out << "P_p\t" << std::to_string(Prec_p) << std::endl; // Precision of CP
  out << "R_p\t" << std::to_string(Rec_p) << std::endl;  // Recall of CP
  out << "F0.5_p\t" << std::to_string(FSCORE1_p)
      << std::endl; // F0.5 score of CP
  out << "Fsp_p\t" << std::to_string(FSCOREsp_p)
      << std::endl; // Fsp score of CP

  out << std::endl;

  out << "omega_o\t" << std::to_string(omega_o)
      << std::endl; // sum(|adherent(m)|) for m in Co
  out << "TP_o\t" << std::to_string(TP_o) << std::endl;  // True positive of CO
  out << "TN_o\t" << std::to_string(TN_o) << std::endl;  // True negative of CO
  out << "FP_o\t" << std::to_string(FP_o) << std::endl;  // False positive of CO
  out << "FN_o\t" << std::to_string(FN_o) << std::endl;  // False negative of CO
  out << "P_o\t" << std::to_string(Prec_o) << std::endl; // Precision of CO
  out << "R_o\t" << std::to_string(Rec_o) << std::endl;  // Recall of CO
  out << "F0.5_o\t" << std::to_string(FSCORE1_o)
      << std::endl; // F0.5 score of CO
  out << "Fsp_o\t" << std::to_string(FSCOREsp_o)
      << std::endl; // Fsp score of CO

  out << std::endl;

  for (long unsigned int i = 0; i < clust.cP.size(); i++) {
    if (clust.aliveP[i]) {
      out << "m";
      for (auto it = clust.cP[i].begin(); it != clust.cP[i].end(); it++) {
        out << "\t" << std::to_string(*it);
      }

      out << "\t!";

      for (long unsigned int ii = 0; ii < clust.with_nodes[i].size(); ii++) {
        out << "\t" << std::to_string(clust.with_nodes[i][ii]);
      }
      out << std::endl;
    }
  }

  out.close();

  if (Verbose) {
    std::cerr
        << "\n----------------------------------------------------------------"
        << std::endl;
    std::cerr << difftime(time(NULL), timerbegin) << " seconds" << std::endl;

    std::cerr << "|C|" << "=" << std::to_string(clust.nbmP) << std::endl;

    std::cerr << "CP.TP=" << std::to_string(TP_p)
              << ", "; // True positive of CP
    std::cerr << "CP.TN=" << std::to_string(TN_p)
              << ", "; // True negative  of CP
    std::cerr << "CP.FP=" << std::to_string(FP_p)
              << ", "; // False positive  of CP
    std::cerr << "CP.FN=" << std::to_string(FN_p)
              << std::endl; // False negative  of CP

    std::cerr << "CP.P=" << std::to_string(Prec_p) << ", "; // Precision of CP
    std::cerr << "CP.R=" << std::to_string(Rec_p) << ", ";  // Recall of CP
    std::cerr << "CP.F_0.5=" << std::to_string(FSCORE1_p)
              << ", "; // F0.5 score of CP
    std::cerr << "CP.F_" << std::to_string(scaleP) << "="
              << std::to_string(FSCOREsp_p) << std::endl
              << std::endl; // Fsp score of CP

    std::cerr << "CO.TP=" << std::to_string(TP_o)
              << ", "; // True positive of CO
    std::cerr << "CO.TN=" << std::to_string(TN_o)
              << ", "; // True negative of CO
    std::cerr << "CO.FP=" << std::to_string(FP_o)
              << ", "; // False positive of CO
    std::cerr << "CO.FN=" << std::to_string(FN_o)
              << std::endl; // False negative of CO

    std::cerr << "CO.P=" << std::to_string(Prec_o) << ", "; // Precision of CO
    std::cerr << "CO.R=" << std::to_string(Rec_o) << ", ";  // Recall of CO
    std::cerr << "CO.F_0.5=" << std::to_string(FSCORE1_o)
              << ", "; // F0.5 score of CO
    std::cerr << "CO.F_" << std::to_string(scaleP) << "="
              << std::to_string(FSCOREsp_o) << std::endl; // // Fsp score of CO

    std::cerr
        << "----------------------------------------------------------------"
        << std::endl
        << std::endl;
  }
  return "";
}
