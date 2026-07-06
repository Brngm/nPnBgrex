// nPnBmain.cpp
#include "include.h"
#include "handy_types.h"
#include "struct.h"
#include "tool1.h"
#include "tool2.h"
#include "tool3.h"

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
void info() {
  static std::string s;
  std::ostringstream A;
  A << "bec v2.2 -- bec is graph clustering program that finds the "
       "modules of a network with a hand on "
       "the description scale of the modules within the nPnB framework.\n";
  A << "Copyright (C) 2026 Bruno Gaume.\n\n";
  s = A.str();
  printf("%s", s.c_str());
}

void help() {
  // info();
  static std::string s;
  std::ostringstream A;
  A << "usage: "
       "bec [scaleP] [input_graph_file] [output_clustering_file] [options]\n\n";

  A << "scaleP:                 In [0,1], the description scale for clustering by partition.\n";
  A << "input_graph_file:       Read the network from this file.\n";
  A << "output_clustering_file: Write the clustering to this file.\n";

  A << "\noptions:\n";

  A << "   -ox\tset  scaleO to x in [0,1]. "
       "The stickiness scale, overlap amount for the extension of the "
       "partition modules into overlapping modules. "
       "(default = 'no' do not extend to save time).\n";

  A << "   -ex\tset  epsilon to x in [0,1]. "
       "The smaller epsilon, the better the quality of the output clustering, "
       "but the slower the computing. "
       "(default = 0.01).\n";

  A << "   -rx\tset  rankedEdeges to x. "
       "(default = '0', the edges are not ranked in the input graph, the program will take care of it).\n";

  A << "   -zx\tset  clustZero to x. "
       "Read the INITIAL PARTITION to amend from the x file "
       "(default = '0', the programm will take care of it: each node in its own module)\n";

  A << "   -vx\tset  verbose to x. "
       "Display the progress. "
       "(default = '0', silent)\n\n";

  A << "example:\n"
       "bec 0.49 GraphExample.txt clustOutput.txt -o0.1 -v1\n\n";
  
  s = A.str();
  printf("%s", s.c_str());
}

// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
int main(int argc, char **argv) {
  /*
  */

  if ((argc < 4)) {
    printf("ERROR calling: '");
    for (int i = 0; i < argc - 1; i++) {
      printf("%s ", argv[i]);
    }
    printf("%s'\n", argv[argc - 1]);
    help();
    return 1;
  }

  // INPUTS REQUIRED
  const char *scaleP = argv[1]; // scale of description for the nodes partition
  const char *InfileGraph = argv[2];  // The path of the input graph
  const char *OutfileClust = argv[3]; // The path of the output clustering

  // OPTIONAL INPUTS
  // scale of description to extend the partition to a clustering that allows overlapping (default: "no" do not extend)
  const char *scaleO = "no";

  // The initial clustering path (default "0", initial clustering with each vertex in its own module)
  const char *ClustZero = "0";

  // "0" or "1" (default: "0" the edge are not ranked in the graph, the program will take care of it)
  const char *RankedEdeges = "0";

  // The smaller epsilon, the better the quality of the output clustering, but the slower the computing (default: "0.01")
  const char *Epsilon = "0.01";

  // Displays progress (default: "0" for silent)
  const char *Verbose = "0";

  for (int i = 4; i < argc; i++) {
    if (argv[i][0] == '-') {
      switch (argv[i][1]) {

      case 'o':
        scaleO = argv[i] + 2;
        break;

      case 'z':
        ClustZero = argv[i] + 2;
        break;

      case 'r':
        RankedEdeges = argv[i] + 2;
        if (strcmp(RankedEdeges, "0") != 0) {RankedEdeges = "1";}
        break;

      case 'e': // Epsilon
        Epsilon = argv[i] + 2;
        break;

      case 'v': // Verbose
        Verbose = argv[i] + 2;
        if (strcmp(Verbose, "0") != 0) {Verbose = "1";}
        break;

      default:
        printf("ERROR calling: '");
        for (int i = 0; i < argc - 1; i++) {
          printf("%s ", argv[i]);
        }
        printf("%s'\n", argv[argc - 1]);
        help();
        return 1;
        break;
      }
    } else {
      printf("ERROR calling: '");
      for (int i = 0; i < argc - 1; i++) {
        printf("%s ", argv[i]);
      }
      printf("%s'\n", argv[argc - 1]);
      help();
      return 1;
    }
  }

  if ((strcmp(Verbose, "0") != 0)) {
    std::cerr << std::endl << std::endl;
    std::cerr << "----------------------------------------------------------------" << std::endl;
    std::cerr << "bec";
    std::cerr << " " << scaleP;
    std::cerr << " " << InfileGraph;
    std::cerr << " " << OutfileClust;
    std::cerr << " -o" << scaleO;
    std::cerr << " -e" << Epsilon;
    std::cerr << " -r" << RankedEdeges;
    std::cerr << " -z" << ClustZero << std::endl;
  }

  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  const char *ERROR = BEC(InfileGraph, OutfileClust, ClustZero, RankedEdeges, scaleP, scaleO, Epsilon, Verbose);
  // @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
  if (!(ERROR != nullptr && ERROR[0] == '\0')) {
    printf("%s\n", ERROR);
    help();
    return 1;
  }
  return 0;
}
