
#!/usr/bin/python
# -*- coding: utf-8 -*-

"""
    Library for managing graph visualization by assigning vertex colorings
    according to a clustering constructed with nPnB
    according to a description scale (module granularity).
"""
import codecs
import math
import random as rd
import builtins
import json
import os
import sys
import time
from matplotlib import pyplot as plt

sys.path.append("nPnB/")
import nPnBinterface as nPnB

# =============================================================================
def generate_contrasting_colors(INPUT={}): 
    """
    TODO
    """
    if INPUT:
        n=INPUT["n"]; L1=INPUT["L1"]; L2==INPUT["L2"]; resolution=INPUT["resolution"]; seed=INPUT["seed"]
        if seed is not None:
            np.random.seed(seed)

        # Espace RGB discret
        space = np.linspace(0, 255, resolution)
        candidates = np.array(np.meshgrid(space, space, space)).T.reshape(-1, 3)

        # Filtrage par luminosité
        lumin = candidates.sum(axis=1)
        mask = (lumin > L1) & (lumin < L2)
        candidates = candidates[mask]

        if len(candidates) == 0:
            raise ValueError("Aucune couleur ne correspond aux contraintes de luminosité données.")

        # Choix initial aléatoire
        colors = [candidates[np.random.randint(len(candidates))]]

        for _ in range(1, n):
            # Calcul de la distance minimale de chaque candidat aux couleurs choisies
            dists = np.min(
                np.linalg.norm(candidates[:, None, :] - np.array(colors)[None, :, :], axis=2),
                axis=1
            )
            # Choisir la couleur la plus éloignée
            next_color = candidates[np.argmax(dists)]
            colors.append(next_color)

        # Conversion en tuples d'entiers
        L=[list(map(int, c)) for c in colors]
    else:
      if False:
          L=[
              [0, 255, 49], [0, 49, 255], [255, 0, 49], [205, 0, 255], [16, 255, 255], [255, 255, 0], [8, 148, 148], [123, 115, 246], [131, 255, 90], [255, 82, 156], [131, 24, 148], [246, 123, 0], [131, 255, 255], [222, 189, 82], [238, 123, 255], [90, 213, 0], [98, 0, 246], [41, 255, 148], [115, 172, 156], [24, 148, 255], [57, 74, 180], [205, 82, 74], [180, 197, 0], [57, 189, 82], [222, 0, 139], [172, 106, 172], [255, 255, 131], [148, 255, 172], [74, 205, 222], [172, 180, 255], [156, 49, 222], [74, 115, 115], [0, 205, 197], [197, 255, 49], [255, 156, 148], [255, 49, 230], [246, 197, 255], [172, 139, 106], [74, 65, 255], [205, 255, 238], [255, 49, 0], [172, 123, 8], [148, 189, 65], [172, 205, 131], [0, 106, 205], [65, 139, 197], [246, 189, 16], [65, 255, 65], [123, 123, 57],
              [0, 170, 50], [170, 0, 50], [136, 0, 50], [10, 170, 50], [170, 170, 50], [5, 98, 50], [82, 76, 50], [87, 170, 50], [170, 54, 50], [87, 16, 50], [164, 82, 50], [87, 170, 50], [148, 126, 50], [158, 82, 50], [60, 142, 50], [65, 0, 50], [27, 170, 50], [76, 114, 50], [16, 98, 50], [38, 49, 50], [136, 54, 50], [120, 131, 50], [38, 126, 50], [148, 0, 50], [114, 70, 50], [170, 170, 50], [98, 170, 50], [49, 136, 50], [114, 120, 50], [104, 32, 50], [49, 76, 50], [0, 136, 50], [131, 170, 50], [170, 104, 50], [170, 32, 50], [164, 131, 50], [114, 92, 50], [49, 43, 50], [136, 170, 50], [170, 32, 50], [114, 82, 50], [98, 126, 50], [114, 136, 50], [0, 70, 50], [43, 92, 50], [164, 126, 50], [43, 170, 50], [82, 82, 50],
              [50, 170, 32], [50, 32, 170], [50, 0, 170], [50, 170, 170], [50, 170, 0], [50, 98, 98], [50, 76, 164], [50, 170, 60], [50, 54, 104], [50, 16, 98], [50, 82, 0], [50, 170, 170], [50, 126, 54], [50, 82, 170], [50, 142, 0], [50, 0, 164], [50, 170, 98], [50, 114, 104], [50, 98, 170], [50, 49, 120], [50, 54, 49], [50, 131, 0], [50, 126, 54], [50, 0, 92], [50, 70, 114], [50, 170, 87], [50, 170, 114], [50, 136, 148], [50, 120, 170], [50, 32, 148], [50, 76, 76], [50, 136, 131], [50, 170, 32], [50, 104, 98], [50, 32, 153], [50, 131, 170], [50, 92, 70], [50, 43, 170], [50, 170, 158], [50, 82, 5], [50, 126, 43], [50, 136, 87], [50, 70, 136], [50, 92, 131], [50, 126, 10], [50, 170, 43], [50, 82, 38],
              [0, 50, 170], [170, 50, 32], [136, 50, 170], [10, 50, 170], [170, 50, 0], [5, 50, 98], [82, 50, 164], [87, 50, 60], [170, 50, 104], [87, 50, 98], [164, 50, 0], [87, 50, 170], [148, 50, 54], [158, 50, 170], [60, 50, 0], [65, 50, 164], [27, 50, 98], [76, 50, 104], [16, 50, 170], [38, 50, 120], [136, 50, 49], [120, 50, 0], [38, 50, 54], [148, 50, 92], [114, 50, 114], [170, 50, 87], [98, 50, 114], [49, 50, 148], [114, 50, 170], [104, 50, 148], [49, 50, 76], [0, 50, 131], [131, 50, 32], [170, 50, 98], [170, 50, 153], [164, 50, 170], [114, 50, 70], [49, 50, 170], [136, 50, 158], [170, 50, 0], [114, 50, 5], [98, 50, 43], [114, 50, 87], [0, 50, 136], [43, 50, 131], [164, 50, 10], [43, 50, 43], [82, 50, 38]
          ]
      else: # To change the colors if needed: for example, to have more contrast between modules on clusters with few modules
        L=[
              [255, 0, 0], [0, 255, 0], [0, 0, 255],     [255, 255, 0], [255, 0, 255], [0, 255, 255],     [200, 100, 100], [100, 200, 100], [100, 100, 200],
              [255, 82, 156], [131, 24, 148], [246, 123, 0], [131, 255, 255], [222, 189, 82], [238, 123, 255], [90, 213, 0], [98, 0, 246], [41, 255, 148], [115, 172, 156], [24, 148, 255], [57, 74, 180], [205, 82, 74], [180, 197, 0], [57, 189, 82], [222, 0, 139], [172, 106, 172], [255, 255, 131], [148, 255, 172], [74, 205, 222], [172, 180, 255], [156, 49, 222], [74, 115, 115], [0, 205, 197], [197, 255, 49], [255, 156, 148], [255, 49, 230], [246, 197, 255], [172, 139, 106], [74, 65, 255], [205, 255, 238], [255, 49, 0], [172, 123, 8], [148, 189, 65], [172, 205, 131], [0, 106, 205], [65, 139, 197], [246, 189, 16], [65, 255, 65], [123, 123, 57],
              [0, 170, 50], [170, 0, 50], [136, 0, 50], [10, 170, 50], [170, 170, 50], [5, 98, 50], [82, 76, 50], [87, 170, 50], [170, 54, 50], [87, 16, 50], [164, 82, 50], [87, 170, 50], [148, 126, 50], [158, 82, 50], [60, 142, 50], [65, 0, 50], [27, 170, 50], [76, 114, 50], [16, 98, 50], [38, 49, 50], [136, 54, 50], [120, 131, 50], [38, 126, 50], [148, 0, 50], [114, 70, 50], [170, 170, 50], [98, 170, 50], [49, 136, 50], [114, 120, 50], [104, 32, 50], [49, 76, 50], [0, 136, 50], [131, 170, 50], [170, 104, 50], [170, 32, 50], [164, 131, 50], [114, 92, 50], [49, 43, 50], [136, 170, 50], [170, 32, 50], [114, 82, 50], [98, 126, 50], [114, 136, 50], [0, 70, 50], [43, 92, 50], [164, 126, 50], [43, 170, 50], [82, 82, 50],
              [50, 170, 32], [50, 32, 170], [50, 0, 170], [50, 170, 170], [50, 170, 0], [50, 98, 98], [50, 76, 164], [50, 170, 60], [50, 54, 104], [50, 16, 98], [50, 82, 0], [50, 170, 170], [50, 126, 54], [50, 82, 170], [50, 142, 0], [50, 0, 164], [50, 170, 98], [50, 114, 104], [50, 98, 170], [50, 49, 120], [50, 54, 49], [50, 131, 0], [50, 126, 54], [50, 0, 92], [50, 70, 114], [50, 170, 87], [50, 170, 114], [50, 136, 148], [50, 120, 170], [50, 32, 148], [50, 76, 76], [50, 136, 131], [50, 170, 32], [50, 104, 98], [50, 32, 153], [50, 131, 170], [50, 92, 70], [50, 43, 170], [50, 170, 158], [50, 82, 5], [50, 126, 43], [50, 136, 87], [50, 70, 136], [50, 92, 131], [50, 126, 10], [50, 170, 43], [50, 82, 38],
              [0, 50, 170], [170, 50, 32], [136, 50, 170], [10, 50, 170], [170, 50, 0], [5, 50, 98], [82, 50, 164], [87, 50, 60], [170, 50, 104], [87, 50, 98], [164, 50, 0], [87, 50, 170], [148, 50, 54], [158, 50, 170], [60, 50, 0], [65, 50, 164], [27, 50, 98], [76, 50, 104], [16, 50, 170], [38, 50, 120], [136, 50, 49], [120, 50, 0], [38, 50, 54], [148, 50, 92], [114, 50, 114], [170, 50, 87], [98, 50, 114], [49, 50, 148], [114, 50, 170], [104, 50, 148], [49, 50, 76], [0, 50, 131], [131, 50, 32], [170, 50, 98], [170, 50, 153], [164, 50, 170], [114, 50, 70], [49, 50, 170], [136, 50, 158], [170, 50, 0], [114, 50, 5], [98, 50, 43], [114, 50, 87], [0, 50, 136], [43, 50, 131], [164, 50, 10], [43, 50, 43], [82, 50, 38]
          ]
    return L
    
# =============================================================================
def show_colors(colors):
    """
    TODO
    """
    n = len(colors)
    fig, ax = plt.subplots(figsize=(n, 1))
    for i, (r, g, b) in enumerate(colors):
        ax.add_patch(plt.Rectangle((i, 0), 1, 1, color=(r/255, g/255, b/255)))
        ax.text(i + 0.5, -0.2, f"{i+1}", ha='center', va='center', fontsize=10)
    ax.set_xlim(0, n)
    ax.set_ylim(0, 1)
    ax.axis("off")
    plt.tight_layout()
    plt.show()

# =============================================================================
def add_degrees(g):
    """
    add_degrees(g)
    adds the 'deg' field to the json
    """
    nbv=len(g["nodes"]); nbe=len(g["links"])
    deg=[0 for _ in range(nbv)]
    for i in range(nbe):
        deg[g["links"][i]["source"]]=deg[g["links"][i]["source"]]+g["links"][i]["weight"]
        deg[g["links"][i]["target"]]=deg[g["links"][i]["target"]]+g["links"][i]["weight"]
    for i in range(nbv):
        g["nodes"][i]["deg"]=deg[g["nodes"][i]["id"]]

# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


def build_XX0(graph, CS):
    """
    """
    timedeb=time.time()
    nbv=len(graph["nodes"])

    IS0=[]
    for DX in CS:
        sp=float("%.10f"%float(DX["input_sp"]))
        Pp=float(DX["P_p"]); Rp=float(DX["R_p"]); F1p=float(DX["F0.5_p"]); Fpsp=float(DX["Fsp_p"]); OMEGAp=int(DX["omega_p"])
        scoreP='"|Cp(sp=%.4f)|=%i: P=%.2f, R=%.2f, F0.50=%.2f, F%.4f=%.2f, OMEGA=%i"'%(sp,len(DX["Cp"]),Pp,Rp,F1p,sp,Fpsp, OMEGAp)
        
        so=float("%.10f"%float(DX["input_so"]))
        Po=float(DX["P_o"]); Ro=float(DX["R_o"]); F1o=float(DX["F0.5_o"]); Fosp=float(DX["Fsp_o"]); OMEGAo=int(DX["omega_o"])
        scoreO='"|Co(so=%.4f)|=%i: P=%.2f, R=%.2f, F0.50=%.2f, F%.4f=%.2f, OMEGA=%i"'%(so,len(DX["Cp"]),Po,Ro,F1o,sp,Fosp, OMEGAo)       

        member=[[] for _ in range(nbv)] # Cp
        for i in range(len(DX["Cp"])): 
            for j in DX["Cp"][i]:
                member[j].append(i)

        for i in range(len(DX["Cp"])): # Co = Cp + extention
            for j in DX["Ext"][i]:
                member[j].append(i)

        IS0.append({"sp":sp, "N":len(DX["Cp"]), "member":member, "P":DX["P_p"], "R":DX["R_p"], "F1":DX["P_p"], "Fs":DX["Fsp_p"], "OMEGA":DX["omega_p"],
        "scoreP": scoreP, "scoreO": scoreO, "so":sp})

    timefin=time.time()
    return IS0

  
# =============================================================================
def makeJsonGraphGroups(g, C):
    """
    """
    nbv=len(g["nodes"]); nbe=len(g["links"])

    XX=build_XX0(g, C)
      
    # name =================
    CH='  {\n            "name": "%s",\n'%(g["name"])

    # scales
    CH=CH+'              "scales": [\n'
    nbscale=len(XX)
    for i in range(nbscale):

        lastc=("," if i<(nbscale-1) else "")
        CH=CH+'                    {"s":%s, "scoreP": %s, "scoreO": %s}%s\n'%(str(XX[i]["sp"]), XX[i]["scoreP"], XX[i]["scoreO"], lastc)
    CH=CH+'            ],\n'

    # nodes =================
    CH=CH+'            "nodes": [\n'
    for i in range(nbv):
        lastc=("," if i<(nbv-1) else "")
        name=g["nodes"][i]["label"]; name=name.encode('UTF-8'); name=name.decode('UTF-8')

        member=[]
        for ii in range(nbscale):
          member.append(XX[ii]["member"][i])

        CH=CH+'               {"id": %i, "name": "%s", "group": %s}%s\n'%(g["nodes"][i]["id"], name, member,lastc)
    CH=CH+'            ],\n'

    # links =================
    CH=CH+'            "links": [\n'
    for i in range(nbe):
        lastc=("," if i<(nbe-1) else "")
        CH=CH+'               {"id": %i, "source": %i, "target": %i, "value": %.2f}%s\n'%(i, g["links"][i]["source"],
                                                                            g["links"][i]["target"],g["links"][i]["weight"],lastc)
    CH=CH+'            ]\n        }\n'
    return CH

# =============================================================================
# =============================================================================
def make3DHTML(graph, C, OutfileHTML, input_colors=None, template="V.E.1.STABLE.Template.html"):
    """
    Generate a HTML visualization from an external template.

    The template must contain exactly two markers:
        <!-- $$$$ personal data -->
        <!-- $$$$ personal data -->

    The content between these markers is replaced by:
        - graph-json generated by makeJsonGraphGroups(graph, C)
        - colorRGB generated by generate_contrasting_colors(input_colors)
    """
    import os
    import re

    if input_colors is None:
        input_colors = {}

    nl = "\n"

    with open("Graph-Visualization/"+template, "r", encoding="utf-8") as f:
        CH = f.read()

    marker = "<!-- $$$$ personal data -->"
    if CH.count(marker) != 2:
        raise ValueError(
            "Template must contain exactly two '<!-- $$$$ personal data -->' markers. "
            f"Found {CH.count(marker)}."
        )

    personal_data = ""
    personal_data += "    " + marker + nl
    personal_data += "    <script type=\"application/json\" id=\"graph-json\">\n"
    personal_data += "       %s\n" % (makeJsonGraphGroups(graph, C))
    personal_data += "    </script>\n"
    personal_data += "    <script> // basic colors\n"
    personal_data += "    const colorRGB = %s;\n" % (generate_contrasting_colors(input_colors))
    personal_data += "    </script>\n"
    personal_data += "    " + marker

    pattern = re.compile(
        r"\s*<!-- \$\$\$\$ personal data -->[\s\S]*?<!-- \$\$\$\$ personal data -->"
    )

    CH, n = pattern.subn("\n" + personal_data, CH, count=1)
    if n != 1:
        raise ValueError("Could not replace the personal data block in template.")

    outdir = os.path.dirname(os.path.abspath(OutfileHTML))
    if outdir and not os.path.isdir(outdir):
        os.makedirs(outdir, exist_ok=True)

    with open(OutfileHTML, "w", encoding="utf-8") as f:
        f.write(CH)

    return CH

# =============================================================================
def saveChemCH(chem, CH):
    """
        saveChemCH(chem, CH)
        Save the string CH at chem
    """
    f=codecs.open(chem, "w",  encoding='utf8')
    f.write(CH)
    f.close()
    
# =============================================================================
        


