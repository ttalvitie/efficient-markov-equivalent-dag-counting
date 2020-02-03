# Efficient Markov Equivalent DAG Counting
The code for the AAAI'20 paper "An Efficient Algorithm for Counting Markov Equivalent DAGs" (Robert Ganian, Thekla Hamm, Topi Talvitie).

Based on the [code for the AAAI'19 paper "Counting and Sampling Markov Equivalent Directed Acyclic Graphs"](https://github.com/ttalvitie/count-mao).

## Usage
Compile with `make` (requires the GMP library).

Generate UCCGs with `./gen_uccg.release`. Arguments: number of vertices and number of edges. Writes UCCG as an adjacency matrix to standard output.

Count the Markov equivalent directed acyclic graphs for a given essential graph using `./count_mao.release`. Argument: the name of the method. Reads the graph as an adjacency matrix from the standard input and writes the count to standard output. Supported methods:
- "He and Yu 2016"
- "Dynamic Programming"
- "Dominating Vertex Elimination Dynamic Programming" (the new method)
- "Symmetry Reduction Tree Decomposition DP"

Generate a visualization of a graph as a DOT file using `./graph_to_dot.release`. Reads the graph as an adjacency matrix from the standard input and writes the DOT file to standard output.

## Example
```
# Count the MAOs of a randomly generated sparse UCCG (100 vertices, 200 edges)
./gen_uccg.release 100 200 | ./count_mao.release "Symmetry Reduction Tree Decomposition DP"

# Count the MAOs of a randomly generated dense UCCG (100 vertices, 800 edges)
./gen_uccg.release 100 800 | ./count_mao.release "Dominating Vertex Elimination Dynamic Programming"

# Visualize a randomly generated UCCG (requires dot)
./gen_uccg.release 10 20 | ./graph_to_dot.release | dot -Tx11
```
