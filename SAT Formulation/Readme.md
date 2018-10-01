# Goal
The goal of this assignment is to take a complex new problem and formulate and solve it as a SAT problem. Formulation as SAT is a valuable skill in AI that will come in handy whenever you are faced with a new problem in NP class. SAT solvers over the years have become quite advanced and are often able to scale to decently sized real-world problems.
# Scenario
You are an investigative agency working on uncovering the hidden connections between the various drug agencies. You have got telephone records of various telephone numbers which are believed to be associated with the mafia. Some external source has suggested that there are K different drug agencies with several people part of multiple agencies. Your goal is to automatically uncover the various drug agencies from the telephone records. To solve this problem (for our assignment), you make a few assumptions.<br>
1. Each person has exactly one phone.
1. All drug agencies are very close-knit: if two people are in the same agency they must have called
each other.
1. People don’t directly call anyone outside their agency.
1. No agency is a strict subsidiary of another agency.<br>

You abstract out the problem by creating an undirected graph G, where each node is a person and an edge between two nodes indicates that they had a phone conversation.
## Problem Statement
Given an undirected graph G, and a number K, output K subgraphs of G (say G1, ... Gk) such that a node/edge in G is present in at least one of Gi. Moreover, all Gis are complete graphs (i.e., all nodes connected to each other). Finally, no Gi is a subgraph of another Gj. Sample cases are shown in [samplegraphs.pdf](http://www.cse.iitd.ac.in/~mausam/courses/col333/autumn2018/A3/samplegraphs.pdf). Note that there are no self loops in G. G only has 1 connected component.<br>

We will use miniSAT, a complete SAT solver for this problem. Your code will read a graph in the given input format. You will then convert the mapping problem into a CNF SAT formula. The encoding time and encoding size should be polynomial in the size of the original graph. Your SAT formula will be the input to miniSAT, which will return with a variable assignment that satisfies the formula (or an answer "no", signifying that the problem is unsatisfiable). You will then take the SAT assignment and convert it into K complete subgraphs. You will output these subgraphs in the given output format. Note that you can make only one call to miniSAT.<br>

You are being provided a problem generator that takes inputs |G| and K, and generates random problems with those parameters.
## Input Format
The first line has three numbers: number of vertices in G, number of edges in G and K.<br><br>
Nodes are represented by positive integers starting from 1. Each subsequent line represents an edge between two nodes. An input file that represents the last example in the samplegraphs.pdf is:<br>
5 8 2<br>
1 2<br>
1 3<br>
1 4<br>
4 5<br>
3 2<br>
4 2<br>
5 3<br>
3 4<br>
## Output Format
Each subgraph will be prefaced with a #i |Gi| indicating that it is the ith subgraph of number of vertices |Gi|. Post that, mention the vertices in one line. For the solution to the above example<br>
#1 4<br>
1 2 3 4<br>
#2 3<br>
3 4 5<br>
If the problem is unsatisfiable output a 0.<br>
## Code
Supply a compile.sh script. Also supply two shell scripts run1.sh, run2.sh.
* Executing the command “./run1.sh test” will take as input a file named test.graph and produce a file test.satinput – the input file for minisat. You can assume that test.graph exists in the present working directory.
* Executing the command “./run2.sh test” will use the generated test.satoutput, test.graph (and any other temporary files produced by run1.sh) and produce a file test.subgraphs – subgraphs in the output format described above. You can assume that test.graph, test.satoutput (and other temp files) exist in the present working directory.
* Script execution will be as followed:
  1. ./run1.sh test
  1. ./minisat test.satinput test.satoutput 
  1. ./run2.sh test
<br>
When we call “./run1.sh test”, you can assume that test.graph exists in the present working directory. When we call “./run2.sh test”, you can assume that test.graph, test.satinput and test.satoutput exist in the present working directory, along with any other temporary files created by “./run1.sh test”.<br>
While we have not given an explicit time limit in the assignment, we may cut off your program if it takes an excruciatingly long amount of time, say more than an hour or so.<br>

## What is being provided?
A problem generator for G and K where G does have K complete subgraphs is being provided. A check function that tests your output is also being provided. It does not check “unsatisfiable” output and only verifies if your solution provides K complete subgraphs. To run the generator use the command “python problemGenerator.py <number of vertices> <K>”, which will generate the input file "test.graph". To test your code use “python checker.py <input graph file> <output subgraphs file>”. It will only work for satisfiable cases.<br>
  
## Useful Resources
1. [The MiniSat page](http://minisat.se/MiniSat.html)
1. [MiniSat User Guide](https://dwheeler.com/essays/minisat-user-guide.html)

