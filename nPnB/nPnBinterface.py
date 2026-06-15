#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
Python interface
"""
import os
import sys
from pathlib import Path
import ctypes
import subprocess
import math
import random as rd
import codecs


# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# metrics
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
def beta2s(beta):
    """
    beta2s(beta): return (2*(math.atan(beta)))/math.pi
    """
    return (2*(math.atan(beta)))/math.pi

def s2beta(s):
    """
    s2beta(s): return (math.tan((math.pi*s)/2))
    """
    return (math.tan((math.pi*s)/2))

def PRF(s, TruePositive, TrueNegative, FalsePositive, FalseNegative):
    """
    PRF(s, TruePositive, TrueNegative, FalsePositive, FalseNegative)
    return (P, R, Fs)
    """
    beta=s2beta(s)
    P=0.0; R=0.0; F=0.0
    if (TruePositive + FalsePositive) > 0:
        P = TruePositive/(TruePositive + FalsePositive)
    if (TruePositive + FalseNegative) > 0:
        R = TruePositive/(TruePositive + FalseNegative)
    if (P + R) > 0:
        F = (1 + beta**2) * ((P * R) / (((beta**2) * P) + R))  
    return (P, R, F)

def TPTNFPFN_Intrinsic(InfileGraph, InfileClust, PO):
    """
    """
    here=str(Path(__file__).resolve().parent)
    #print(here)
    lib = ctypes.CDLL(here+"/lib/libnpnb.so")

    lib.TPTNFPFN_Intrinsic.argtypes = [
        ctypes.c_char_p,                  # InfileGraph
        ctypes.c_char_p,                  # InfileClust
        ctypes.c_char_p,                  # POclust
        ctypes.c_char_p,                  # error_msg
        ctypes.c_int,                     # error_msg_size
        ctypes.POINTER(ctypes.c_double)   # out
    ]
    lib.TPTNFPFN_Intrinsic.restype = ctypes.c_int

    ERROR_BUFFER_SIZE = 1000
    error_buffer = ctypes.create_string_buffer(ERROR_BUFFER_SIZE)
    result_array = (ctypes.c_double * 4)()

    x = str(InfileGraph).encode("utf-8")
    y = str(InfileClust).encode("utf-8")
    POy = str(PO).encode("utf-8")

    code = lib.TPTNFPFN_Intrinsic(
        x,
        y,
        POy,
        error_buffer,
        ERROR_BUFFER_SIZE,
        result_array
    )

    if code != 0:
        return code, error_buffer.value.decode(), None
    else:
        X = list(result_array) 
        return 0, None, {"TP":X[0], "TN":X[1], "FP":X[2], "FN":X[3]} 

def TPTNFPFN_Extrinsic(InfileGold, POgold, InfileClust, POclust, n_vertices):
    """
    """
    here=str(Path(__file__).resolve().parent)
    #print(here)
    lib = ctypes.CDLL(here+"/lib/libnpnb.so")

    lib.TPTNFPFN_Extrinsic.argtypes = [
        ctypes.c_char_p,                  # InfileGold
        ctypes.c_char_p,                  # POgold
        ctypes.c_char_p,                  # InfileClust
        ctypes.c_char_p,                  # POclust
        ctypes.c_int,                     # number of vertices of the support graph 
        ctypes.c_char_p,                  # error_msg
        ctypes.c_int,                     # error_msg_size
        ctypes.POINTER(ctypes.c_double)   # out
    ]
    lib.TPTNFPFN_Extrinsic.restype = ctypes.c_int

    ERROR_BUFFER_SIZE = 1000
    error_buffer = ctypes.create_string_buffer(ERROR_BUFFER_SIZE)
    result_array = (ctypes.c_double * 4)()

    x = str(InfileGold).encode("utf-8")
    POx = str(POgold).encode("utf-8")

    y = str(InfileClust).encode("utf-8")
    POy = str(POclust).encode("utf-8")

    EXIT = lib.TPTNFPFN_Extrinsic(
        x,
        POx,
        y,
        POy,
        n_vertices,
        error_buffer,
        ERROR_BUFFER_SIZE,
        result_array
    )

    if EXIT != 0:
        # print("Error: ", error_buffer.value.decode());
        return EXIT, error_buffer.value.decode(), None
    else:
        # print("Sucess");
        X = list(result_array) 
        return EXIT, None, {"TP":X[0], "TN":X[1], "FP":X[2], "FN":X[3]} 

# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# make clustering
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
def run_main(commande):
    # stdout=PIPE to capture the value of stdout
    # stderr=None so that stderr is displayed directly in the parent terminal
    result = subprocess.run(
        commande,
        stdout=subprocess.PIPE,
        stderr=None, # stderr goes into the parent terminal
        text=True
    )
    X = result.returncode
    Y = result.stdout.strip()
    return X, Y

def BEC(InfileGraph, OutfileClust, sp, so="no", Epsilon=0.01, InfileClustZero="0", rankedEdeges="0", Verbose="0"):
    """
    """
    assert ((sp >= 0) and (sp <=1)), ("sp Must be such (sp >= 0) and (sp <=1)) but sp=%.10f"%(sp))
    assert ((so=="no") or (so >= 0) and (so <=1)), ("so Must be such ((so=='no') or (so >= 0) and (so <=1)) but so=%.10f"%(so))
    assert ((Epsilon >= 0) and (Epsilon <= 1)), ("Epsilon Must be such ((Epsilon >= 0) and (Epsilon <= 1)) but Epsilon=%.10f"%(s))
    # =====================================================================

    here=str(Path(__file__).resolve().parent)

    (EXIT, ERROR)=run_main([here+"/bin/bec",
        # required inputs
        str(sp), # scale of description
        InfileGraph, # InfileGraph path 
        OutfileClust, # OutfileClust path

        # optional INPUTs
        "-o"+str(so), # in [0,1] The stickiness scale, overlap amount for the extension of the partition modules into overlapping modules. (default = "no" do not extend)
        "-e"+str(Epsilon), # in [0,1] The smaller epsilon, the better the quality of the output clustering, but the slower the computing (default: 0.01)
        "-r"+rankedEdeges, # "0" or "1" (default: "0" the edge are not ordered, the program will take care of it)
        "-z"+InfileClustZero, # Initial clustering path (default "0", initial clustering with each vertex in its own module)
        "-v"+Verbose] # Displays progress (default: 0 for silent)
    ) 

    if (EXIT != 0):
        # print("Error: ", ERROR);
        return EXIT, ERROR, None
    else:
        # print("Sucess");
        return EXIT, None, read_file_npnb_clust(OutfileClust)
        
def BEC_short_lived(InfileGraph, sp, so="no", Epsilon=0.01, InfileClustZero="0", rankedEdeges="0", Verbose="0"):
    """
    """
    # temporary files for OutfileClust
    bidonNum = str(int(rd.random()*(100000000000000000000000000))) # random string of digit
    OutfileClust = "short-lived-nPnB-OutfileClust"+bidonNum+".txt" # path for save the short_lived clustering
    
    EXIT, ERROR, X = BEC(InfileGraph, OutfileClust=OutfileClust, sp=sp, so=so,
                               Epsilon=Epsilon, InfileClustZero=InfileClustZero, rankedEdeges=rankedEdeges, Verbose=Verbose)   
    
    # delete temporary files
    retour = os.popen("rm -r "+OutfileClust,'r').read()
    return EXIT, ERROR, X

# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# read clustering
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
def read_file_npnb_clust(InfileClust, verbose=False):
    d="\t"
    OUTPUT={

        "date":"", # computing date
        "bec":"", # command line

        "input_InfileGraph":"", # input file graph
        "input_OutfileClust":"", # output file clustering

        "input_RankedEdeges":"", # input RankedEdeges used for computing the community structure
        "input_ClustZero":"", # input ClustZero used for computing the community structure
        "input_epsilon":"", # input epsilon used for computing the community structure
        
        "input_sp":"", # input description scale used to construct Cp the clustering by partition
        "input_so":"",  # input stickiness scale, overlap amount used for the extension of the partition modules into overlapping modules
        
        "gnbv":None, # number of nodes in the graph on which the community structure were built
        "gnbe":None, # number of edges in the graph on which the community structure were built
        "density":None, # edges density

        "secondsP":None, # Time computing for partition
        "secondsO":None, # Time computing for the extensions

        "Cp":[], # the clustering by partition
        "Ext":[], # the gluant extentions of the modules of Cp: len(Cp) = len(Ext). For all i, Cp[i] ∩ Ext[i] = ∅

        "|C|":None, # the number of modules

        # intrinsic metrics of Cp with respect to the graph on which it was constructed
        "omega_p":None,
        "TP_p":None, "TN_p":None, "FP_p":None, "FN_p":None,
        "P_p":None, "R_p":None, "F0.5_p":None, "Fsp_p":None,

        # Co=[Cp[i]+w[i] for i in range len(Cp)] (Co is a clustering allowing overlaps)
        # intrinsic metrics of Co with respect to the graph on which it was constructed
        "omega_o":None,
        "TP_o":None, "TN_o":None, "FP_o":None, "FN_o":None,
        "P_o":None, "R_o":None, "F0.5_o":None, "Fsp_o":None
    }

    with open(InfileClust, "r", encoding="utf-8") as f:
        for ligne in f:
            line=ligne.strip()
            if verbose:
                print(line)
            if (not (line=="")):
                if (not (line[0]=="#")):
                    linesplit=line.split(d)
                    match linesplit[0]:
                        case "date":
                            OUTPUT["date"] = linesplit[1]
                        case "bec":
                            OUTPUT["bec"] = linesplit[1]
                        case "input_InfileGraph": 
                            OUTPUT["input_InfileGraph"] = linesplit[1]
                        case "input_OutfileClust": 
                            OUTPUT["input_OutfileClust"] = linesplit[1]
                        case "input_RankedEdeges": 
                            OUTPUT["input_RankedEdeges"] = linesplit[1]
                        case "input_ClustZero": 
                            OUTPUT["input_ClustZero"] = linesplit[1]
                        case "input_sp": 
                            OUTPUT["input_sp"] = linesplit[1]
                        case "input_so": 
                            OUTPUT["input_so"] = linesplit[1]
                        case "input_epsilon": 
                            OUTPUT["input_epsilon"] = linesplit[1]

                        case "gnbv":
                            OUTPUT["gnbv"] = int(linesplit[1])
                        case "gnbe":
                            OUTPUT["gnbe"] = int(linesplit[1])
                        case "density":
                            OUTPUT["density"] = float(linesplit[1])

                        case "secondsP":
                            OUTPUT["secondsP"] = float(linesplit[1])
                        case "secondsO":
                            OUTPUT["secondsO"] = float(linesplit[1])

                        case "|C|":
                            OUTPUT["|C|"] = int(linesplit[1])

                        case "omega_p":
                            OUTPUT["omega_p"] = float(linesplit[1])
                        case "TP_p":
                            OUTPUT["TP_p"] = float(linesplit[1])
                        case "TN_p":
                            OUTPUT["TN_p"] = float(linesplit[1])
                        case "FP_p":
                            OUTPUT["FP_p"] = float(linesplit[1])
                        case "FN_p":
                            OUTPUT["FN_p"] = float(linesplit[1])
                        case "P_p":
                            OUTPUT["P_p"] = float(linesplit[1])
                        case "R_p":
                            OUTPUT["R_p"] = float(linesplit[1])
                        case "F0.5_p":
                            OUTPUT["F0.5_p"] = float(linesplit[1])
                        case "Fsp_p":
                            OUTPUT["Fsp_p"] = float(linesplit[1])

                        case "omega_o":
                            OUTPUT["omega_o"] = float(linesplit[1])
                        case "TP_o":
                            OUTPUT["TP_o"] = float(linesplit[1])
                        case "TN_o":
                            OUTPUT["TN_o"] = float(linesplit[1])
                        case "FP_o":
                            OUTPUT["FP_o"] = float(linesplit[1])
                        case "FN_o":
                            OUTPUT["FN_o"] = float(linesplit[1])
                        case "P_o":
                            OUTPUT["P_o"] = float(linesplit[1])
                        case "R_o":
                            OUTPUT["R_o"] = float(linesplit[1])
                        case "F0.5_o":
                            OUTPUT["F0.5_o"] = float(linesplit[1])
                        case "Fsp_o":
                            OUTPUT["Fsp_o"] = float(linesplit[1])

                        case "m": # on module
                            LL = linesplit[1:]  # remove the first token "m"
                            i = LL.index("!")   # find the position of "!"
                            L1= LL[:i]          # before "!"
                            L2 = LL[i+1:]       # after "!"
                            OUTPUT["Cp"].append([int(x) for x in L1])
                            OUTPUT["Ext"].append([int(x) for x in L2])
                        case _:
                            junk=0
    return OUTPUT

# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# intrinsic scales
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
def equivalent_C1_C2(C1, C2, eqon):
    """
    complexity when C1=C2: 7*nbv
    """
    if eqon == "equal": # equivalence is done on real equality C1=C2
        if not (len(C1)==len(C2)):
            return False
        else:
            nbv=sum([len(mod) for mod in C1]) # complexity = nbv
            member1=[0 for i in range(nbv)] # complexity = nbv
            for i in range(len(C1)): # complexity = nbv
                for j in C1[i]:
                    member1[j]=i

            member2=[0 for i in range(nbv)] # complexity = nbv
            for i in range(len(C2)): # complexity = nbv
                for j in C2[i]:
                    member2[j]=i

            for mod in C1: # C1 included in C2 # complexity = nbv
                x=member2[mod[0]]
                for i in mod:
                    if not (member2[i] == x):
                        return False
            for mod in C2: # C2 included in C1 # complexity = nbv
                x=member1[mod[0]]
                for i in mod:
                    if not (member1[i] == x):
                        return False
            return True
    else: # equivalence is not done on real equality C1=C2, but on simple (len(C1)==len(C2)) (it's faster)
        if (len(C1)==len(C2)):
            return True  
        else:
            return False 

def clust_intrinsic(InfileGraph, Epsilon=0.01, step=0.001, eqon="equal", Verbose="0"):
    """
    """
    INTR=[]
    s=0
    while s <= 1:
        print("Searching for intrinsic scales (by step=%.5f) s=%.5f"%(step, s), end="\r")
        EXIT, ERROR, X = BEC_short_lived(InfileGraph, sp=s, so=s,
                                       Epsilon=Epsilon, InfileClustZero="0", rankedEdeges="0", Verbose=Verbose)
        if (EXIT!=0):
            print(ERROR)
            return False
        else:
            if float(X["omega_p"]) == 0:
                if len(INTR) ==0:
                    INTR.append(X)
                else:
                    if not equivalent_C1_C2(X["Cp"], INTR[len(INTR)-1]["Cp"], eqon):
                        INTR.append(X)                    
        s=s+step
    return INTR

def clust_scale(InfileGraph, scale, Epsilon=0.01, Verbose="0"):
    """
    """
    C=[]
    for s in scale:
        print("s=%.5f"%(s), end="\r")
        EXIT, ERROR, X = BEC_short_lived(InfileGraph, sp=s, so=s,
                                       Epsilon=Epsilon, InfileClustZero="0", rankedEdeges="0", Verbose=Verbose)
        if (EXIT!=0):
            print(ERROR)
            return False
        else:
            C.append(X)
    return C
