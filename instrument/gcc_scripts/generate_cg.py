import argparse
import re
import hashlib
from typing import Dict, Set
import argparse




# Determine whether a row is a node
def is_node(string):

    pattern = r"NEWFUN .+{.+}"
    match = re.match(pattern, string)
    if match:
        return True
    return False

# Determine whether a row is an edge
def is_edge(string):

    pattern = r".+ -> .+"
    match = re.match(pattern, string)
    if match:
        return True
    return False

# Hash the string and output the first len digits of the hash result
def hash_string(string, len):
    sha256_hash = hashlib.sha256(string.encode()).hexdigest()
    output = sha256_hash[:len]
    return output

# Extract function name from node row
def extract_node_name(string):
    pattern = r"\{(.+?)\}"
    match = re.search(pattern, string)
    if match:
        content = match.group(1)
        return content
    else:
        return None

# Extract the edge start function name and end point function name from the edge row
def extract_edge_name(string):
    pattern = r"(.+) -> (.+)"
    match = re.match(pattern, string)
    if match:
        content1 = match.group(1)
        content2 = match.group(2)
        return content1, content2
    else:
        return None, None

# Organize the cglog file and put the edges with the same starting node under the same node
def merge_edges(cglog):
    nodes: Dict[str, Set[str]] = {}

    with open(cglog, "r") as fp:
        for line in fp.readlines():
            if is_edge(line):
                start_name, end_name = extract_edge_name(line)
                if start_name not in nodes:
                    nodes[start_name] = set()
                nodes[start_name].add(end_name)

    with open(cglog, "w") as fp:
        for key, value in nodes.items():
            node_line = "NEWFUN Node{" + key + "}\n"
            fp.write(node_line)
            for v in value:
                edge_line = key + " -> " + v + "\n"
                fp.write(edge_line)




if __name__ == "__main__":
    #print("hello")
    parser = argparse.ArgumentParser(description=__doc__)

    parser.add_argument("temporary_directory", metavar="temporary-directory",
                        help="Directory where dot files and target files are "
                             "located")

    args = parser.parse_args()

    cglog_path = args.temporary_directory + "/dot-files/callgraph_log.txt"
    cg_path = args.temporary_directory + "/dot-files/callgraph.dot"

    #print(cglog_path)

    nodes = set()
    exist_nodes = set()
    hash_len = 12
    hash_to_node = {}

    merge_edges(cglog_path)

    with open(cglog_path, "r") as cglogfp:
        cgfp = open(cg_path, "w")

        digraph = "digraph \"Call graph: \" {\n"
        label = "\tlabel=\"Call graph: \";\n\n"
        cgfp.write(digraph)
        cgfp.write(label)

        for line in cglogfp.readlines():
            if is_node(line):
                node_name = extract_node_name(line)
                hash = hash_string(node_name, hash_len)
                #print(node_name, ":", hash)
                nodes.add(node_name)
                exist_nodes.add(node_name)

                in_line = "\tNode0x" + hash +" " + "[shape=record, label=\"{" + node_name + "}\"];\n"
                cgfp.write(in_line)


            if is_edge(line):
                start_name, end_name = extract_edge_name(line)
                start_hash = hash_string(start_name, hash_len)
                end_hash = hash_string(end_name, hash_len)
                #print(start_name, "(", start_hash, ")", " -> ", end_name, "(", end_hash, ")")
                nodes.add(start_name)
                nodes.add(end_name)

                in_line = "\tNode0x" + start_hash + " -> " + "Node0x" + end_hash + ";\n"
                cgfp.write(in_line)

        for node in nodes:
            if node not in exist_nodes:
                hash = hash_string(node, hash_len)
                in_line = "\tNode0x" + hash + "[shape=record, label=\"{" + node + "}\"];\n"
                cgfp.write(in_line)

        cgfp.write("}")

        cgfp.close()



