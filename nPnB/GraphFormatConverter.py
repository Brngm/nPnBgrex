#!/usr/bin/python
# -*- coding: utf-8 -*-
"""
GraphFormatConverter
"""

import os
import sys
from datetime import datetime
from pathlib import Path

import re

import json
import igraph as ig
import codecs

# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# -->nPnB 
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
def GRAPH_json_2_nPnB(InfileGraph_json, OutfileGraph_nPnB):
    """
    """
    path = Path(OutfileGraph_nPnB)
    if path.exists():
        print("The file '%s' already exists"%(OutfileGraph_nPnB))
        return False
    else:
        with open(InfileGraph_json, "r") as f: graph_json = json.load(f)
        if False:
            for k in graph_json:
                print("%s: %s\n"%(k, graph_json[k]))
        
        f=codecs.open(OutfileGraph_nPnB, "w",  encoding='utf8')
        
        # header (id_label)
        f.write("#date%s%s\n"%("\t", datetime.now()))
        f.write("#json_information")
        for x in graph_json["information"]:
            f.write("\t%s"%(x))
        f.write("\n")
        f.write("#name%s%s\n"%("\t", graph_json["name"]))
        f.write("#id_label")
        for X in graph_json["nodes"]:
            f.write("%s%s,%s"%("\t", str(X["id"]), X["label"]))
        f.write("\n")
        for X in graph_json["links"]:
            f.write("%s\t%s\t%s\n"%(str(X["source"]), str(X["target"]), str(X["weight"])))
        f.close()
        return True
        
def GRAPH_igraph_2_nPnB(Graph, OutfileGraph, information="", name="Anonymous", label=None):
    """
    """  
    d="\t"
    
    path = Path(OutfileGraph)
    if path.exists():
        print("The file '%s' already exists"%(OutfileGraph))
        return False
    else:
        f=codecs.open(OutfileGraph, "w",  encoding='utf8')
        
        # date
        f.write("#date\t%s\n" % datetime.now())
        
        # information
        INF="#nPnBinformation:"
        linesplit=information.split("\n")
        for l in linesplit:
            INF=INF+"\t"+l
        f.write("%s\n"%(INF))
        
        # name
        f.write("%s\n"%("#name"+d+name))
        
        # label
        f.write("%s"%("#id_label"))
        if label==None:
            for i in range(Graph.vcount()):
                f.write(d+str(i)+","+str(i))
        else:   
            for i in range(Graph.vcount()):
                f.write(d+str(i)+","+g.vs[i][label])
        f.write("\n")
        
        # edges
        for e in Graph.es():
            f.write("%s%s%s%s%s\n"%(str(e.source), d, str(e.target), d, "1"))
        f.close()
        return True
    
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@      
# -->igraph 
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
def GRAPH_nPnB_2_igraph(InfileGraph):
    """
    """
    d="\t"
    information=""
    nbv=0
    path = Path(InfileGraph)
    if path.exists():
        f=codecs.open(InfileGraph, "r",  encoding='utf8')
        E=[]
        nbv=0
        line = f.readline(); line = line[0:len(line)-1]
        while line :
            # print(line)
            if (not (line=="")):
                if ((line[0]=="#")):
                    linesplit=line.split(":")
                    if linesplit[0]=="#nPnBinformation":
                        for i in range(1, len(linesplit)):
                            information=information+":"+linesplit[i]
                else:
                    linesplit=line.split(d)
                    #print(line)
                    m=max([int(linesplit[0]), int(linesplit[1])])
                    if (nbv<m):
                        nbv=m
                    E.append((int(linesplit[0]), int(linesplit[1]), float(linesplit[2])))
            line = f.readline(); line = line[0:len(line)-1]        
        f.close()

        nbv=nbv+1
        g=ig.Graph(n=nbv,
            edges=[(x,y) for (x,y,z) in E],
            directed=False
        )
        for i in range(g.ecount()):
            g.es[i]["w"]=E[i][2]
        g["nPnBinformation"]=information
        return g
    else:
        print("The file '%s' does not exist"%(InfileGraph))

def GRAPH_json_2_igraph(gjson): 
    """
        json2ig(gjson)
        json format-->igraph format
        need import igraph as ig
    """
    import igraph as ig
    g=ig.Graph( n=len(gjson["nodes"]),
                edges=[(x["source"], x["target"]) for x in gjson["links"]],
                directed=gjson["directed"])
    
    for x in gjson["nodes"]:
        for k in x:
            if (not k=="id"):
                g.vs[x["id"]][k]=x[k]
                
    ide=0
    for x in gjson["links"]:
        for k in x:
            if (not k in ["source", "target"]):
                g.es[ide][k]=x[k]
        ide=ide+1 

    g["name"]=gjson["name"]
    g["information"]=gjson["information"]
    return g

# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# -->json
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
def GRAPH_nPnB_2_json(InfileGraph_nPnB):
    """
    """
    path = Path(InfileGraph_nPnB)
    if (not path.exists()):
        print("The file '%s' does not exist"%(InfileGraph_nPnB))
        return False

    gjson={"information":[],
            "name":"",
            "directed":False,
            "nodes":[],
            "links":[]
           }
    
    pf = open(InfileGraph_nPnB, "r"); LINES=pf.readlines(); pf.close()
    LINES=[line[0:len(line)-1] for line in LINES]
        
    #id_label
    id_labelFinded=False
    DICid_DegLabel={}
    maxid = 0
    for line in LINES:
        if (not (line=="")):
            if ((line[0]=="#")):
                linesplit=line.split("\t")
                if linesplit[0]=="#id_label":
                    NBV=len(linesplit)-1
                    id_labelFinded=True
                    for i in range(1,len(linesplit)):
                        linesplit_comma=linesplit[i].split(",")
                        DICid_DegLabel[linesplit_comma[0]]=[0,linesplit_comma[1]]
                        
                if linesplit[0]=="#name":
                    gjson["name"] = linesplit[1]
                    
                if linesplit[0]=="#json_information":
                    for i in range(1,len(linesplit)):
                       gjson["information"].append(linesplit[i])
            else:
                linesplit=line.split("\t")
                if (int(linesplit[0]) > maxid):
                    maxid = int(linesplit[0])
                if (int(linesplit[1]) > maxid):
                    maxid = int(linesplit[1])
                    
    if not id_labelFinded:
        NBV = maxid + 1
        DICid_DegLabel={str(i):[0, str(i)] for i in range(NBV)}       

    #degrees
    for line in LINES:
        if (not (line=="")):
            if (not (line[0]=="#")):
                linesplit=line.split("\t")
                DICid_DegLabel[linesplit[0]][0]=DICid_DegLabel[linesplit[0]][0] + 1
                DICid_DegLabel[linesplit[1]][0]=DICid_DegLabel[linesplit[1]][0] + 1
                
    # nodes
    for k in DICid_DegLabel:
        gjson["nodes"].append({'id':int(k), 'label':  DICid_DegLabel[k][1], 'deg': DICid_DegLabel[k][0]})

    # edges
    for line in LINES:
        if (not (line=="")):
            if (not (line[0]=="#")):
                linesplit=line.split("\t")
                s=linesplit[0]; t=linesplit[1]; w=linesplit[2];
                gjson["links"].append({'source':int(s), 'target':int(t), 'weight':float(w)})
    return gjson
     
    
def GRAPH_igraph_2_json(g, name=None, inforamtion=None, weight=None, label=None):
    """
    """
    import igraph as ig
    gjson={"information":[],
            "name":"miserables",
            "directed":g.is_directed(),
            "nodes":[],
            "links":[]
           }
    if (name==None):
        gjson["name"]="anonymous"
    else:
        gjson["name"]=name

    if (inforamtion==None):
        gjson["information"]=[]
    else:
        gjson["information"]=inforamtion

    for i in range(g.vcount()):
        if label==None:
            x={"id":i, "label": "%i"%(i), "deg": len(g.neighbors(i))}
        else:
            x={"id":i, "label": g.vs[i][label], "deg": len(g.neighbors(i))}
        gjson["nodes"].append(x)
        
    for i in range(g.ecount()):
        if weight==None:
            x={"source": g.es[i].source, "target": g.es[i].target, "weight": 1}
        else:
            x={"source": g.es[i].source, "target": g.es[i].target, "weight": g.es[i][weight]}
            
        gjson["links"].append(x)
        
    return gjson

# =============================================================================
def write_json_compact_readable(
        data,
        filename="",
        indent=2,
        max_line_length=100,
        sort_keys=False,
        return_str=False
    ):
    """
        def write_json_compact_readable(
            data,
            filename="",
            indent=2,
            max_line_length=100,
            sort_keys=False,
            return_str=False
        )
        Writes readable, compact, robust, and configurable JSON.
        - Supports all JSON depths.
        - Sort_keys option to sort keys.
        - Return_str option to return JSON as a string.
        - Resists overriding list or dict.
    """

    list_type = builtins.list
    dict_type = builtins.dict

    def format_value(value, level):
        space = ' ' * (indent * level)

        # --- dictionary ---
        if isinstance(value, dict_type):
            items = value.items()
            if sort_keys:
                items = sorted(items)
            items = [f'"{k}": {format_value(v, level+1)}' for k, v in items]
            inline = '{' + ', '.join(items) + '}'
            if len(space) + len(inline) > max_line_length:
                multiline = ',\n'.join(
                    ' ' * (indent*(level+1)) + f'"{k}": {format_value(v, level+1)}'
                    for k, v in (sorted(value.items()) if sort_keys else value.items())
                )
                return '{\n' + multiline + f'\n{space}' + '}'
            return inline

        # --- list ---
        elif isinstance(value, list_type):
            if all(isinstance(el, dict_type) for el in value):
                items = [json.dumps(el, separators=(", ", ": ")) for el in value]
                inline = '[ ' + ', '.join(items) + ' ]'
                if len(space) + len(inline) > max_line_length:
                    return '[\n' + ',\n'.join(
                        ' ' * (indent*(level+1)) + json.dumps(el, separators=(", ", ": "))
                        for el in value
                    ) + f'\n{space}]'
                else:
                    return inline
            else:
                items = [format_value(el, level+1) for el in value]
                inline = '[ ' + ', '.join(items) + ' ]'
                if len(space) + len(inline) > max_line_length:
                    return '[\n' + ',\n'.join(
                        ' ' * (indent*(level+1)) + format_value(el, level+1)
                        for el in value
                    ) + f'\n{space}]'
                else:
                    return inline

        # --- simple values ​​---
        else:
            return json.dumps(value, separators=(", ", ": "))

    formatted_json = format_value(data, 0)

    # Write to a file if filename specified
    if filename:
        with open(filename, "w") as f:
            f.write(formatted_json + '\n')

    # Returns the string if requested
    if return_str:
        return formatted_json


# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
# -->Graphviz
# @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@


def GRAPH_Graphviz_2_nPnB(InfileGraph_Graphviz, OutfileGraph_nPnB):
    def esc_label(s):
        return str(s).replace('\\"', '"').replace("\\n", " ").strip()

    def clean_id(s):
        s = str(s).strip().strip(";").strip()
        if len(s) >= 2 and s[0] == '"' and s[-1] == '"':
            s = s[1:-1]
        return s.strip()

    name_to_id = {}
    id_to_label = {}
    edges = {}

    def get_id(name):
        name = clean_id(name)
        if name not in name_to_id:
            i = len(name_to_id)
            name_to_id[name] = i
            id_to_label[i] = name
        return name_to_id[name]

    with open(InfileGraph_Graphviz, "r", encoding="utf-8") as f:
        for raw in f:
            line = raw.strip()
            if not line or line.startswith("//") or line in ("{", "}", "graph {", "digraph {"):
                continue
            if line.startswith("graph ") or line.startswith("digraph "):
                continue

            # Edge: A -- B [weight=2]; or A -> B;
            m = re.match(r'^("?[^"\[\];]+?"?)\s*(--|->)\s*("?[^"\[\];]+?"?)\s*(?:\[(.*?)\])?\s*;?\s*$', line)
            if m:
                u = get_id(m.group(1))
                v = get_id(m.group(3))
                attrs = m.group(4) or ""
                w = "1"
                mw = re.search(r'(?:weight|value|label)\s*=\s*"?([^",\]]+)"?', attrs)
                if mw:
                    w = mw.group(1).strip()
                a, b = sorted((u, v))
                if a != b:
                    edges[(a, b)] = w
                continue

            # Node: A [label="..."];
            m = re.match(r'^("?[^"\[\];]+?"?)\s*\[(.*?)\]\s*;?\s*$', line)
            if m:
                node = clean_id(m.group(1))
                attrs = m.group(2)
                i = get_id(node)
                ml = re.search(r'label\s*=\s*"([^"]*)"', attrs)
                if ml:
                    id_to_label[i] = esc_label(ml.group(1))

    with open(OutfileGraph_nPnB, "w", encoding="utf-8") as f:
        f.write("#date\t%s\n" % datetime.now())
        f.write("#json_information\tConverted from Graphviz\n")
        f.write("#name\t%s\n" % InfileGraph_Graphviz.split("/")[-1].replace(".dot", ""))
        f.write("#id_label")
        for i in sorted(id_to_label):
            f.write("\t%s,%s" % (i, id_to_label[i]))
        f.write("\n")
        for (u, v), w in sorted(edges.items()):
            f.write("%s\t%s\t%s\n" % (u, v, w))


def GRAPH_nPnB_2_Graphviz(InfileGraph_nPnB, OutfileGraph_Graphviz):
    id_to_label = {}
    edges = []

    with open(InfileGraph_nPnB, "r", encoding="utf-8") as f:
        for raw in f:
            line = raw.rstrip("\n")
            if not line:
                continue

            parts = line.split("\t")

            if parts[0] == "#id_label":
                for item in parts[1:]:
                    item = item.strip()
                    if not item:
                        continue
                    pair = item.split(",", 1)
                    if len(pair) == 2:
                        id_to_label[pair[0].strip()] = pair[1].strip()

            elif not line.startswith("#"):
                if len(parts) >= 2:
                    u = parts[0].strip()
                    v = parts[1].strip()
                    w = parts[2].strip() if len(parts) >= 3 else "1"
                    edges.append((u, v, w))
                    id_to_label.setdefault(u, u)
                    id_to_label.setdefault(v, v)

    def esc(s):
        return str(s).replace("\\", "\\\\").replace('"', '\\"')

    def sort_key(x):
        return int(x) if str(x).isdigit() else str(x)

    with open(OutfileGraph_Graphviz, "w", encoding="utf-8") as f:
        f.write("graph G {\n")
        f.write('  node [shape=box];\n\n')

        for node_id in sorted(id_to_label, key=sort_key):
            f.write('  "%s" [label="%s"];\n' % (esc(node_id), esc(id_to_label[node_id])))

        f.write("\n")

        seen = set()
        for u, v, w in edges:
            a, b = sorted((str(u), str(v)))
            if a == b or (a, b) in seen:
                continue
            seen.add((a, b))
            f.write('  "%s" -- "%s" [weight="%s"];\n' % (esc(a), esc(b), esc(w)))

        f.write("}\n")
            
            
            
            
            
            

