# Goal
The goal of this assignment is to take a complex new problem and formulate and solve it as search. Formulation as search is an integral skill of AI that will come in handy whenever you are faced with a new problem. Heuristic search will allow you to find optimal solutions. Local search may not find the optimal solution, but is usually able to find good solutions for really large problems.
# Scenario: Optimization of Conference Schedule.
A conference has n papers accepted. Our job is to organize them in a best possible schedule. The schedule has p parallel sessions at a given time. Each session has k papers. And there are a total of t time slots. We can assume that n = t.p.k. For example, in Figure below t=2 ,p=3 and k=4.
|First Header|Second Header| | ------------- | ------------- | | Content Cell  | Content Cell  | | Content Cell  | Content Cell  |

We first define the characteristics of a good schedule. For any good schedule most people should feel no conflict about which session to attend. That is, (1) all papers in one session should be related to a single theme. And (2) all papers in parallel sessions should be as far away as possible to avoid conflict.
\To operationalize this intuition let us assume we are given a function representing the distance between two papers: d(p1,p2), such that d is between 0 and 1. We can similarly define a similarity between two papers s(p1,p2) = 1-d(p1,p2).
Now we can define the goodness of a schedule as follows\
Sum(similarities of all pairs of papers in a session) + C.Sum(distances of all pairs of papers in parallel sessions).
In our example, the goodness will be computed as
s(1,2) + s(1,3) + s(1,4) + s(2,3) + s(2,4) + s(3,4) + s(5,6) +s(5,7)+s(5,8)+s(6,7)+s(6,8)+s(7,8) +.......
+ C[d(1,5)+d(1,6)+...d(1,11)+d(1,12) + d(2,5) ... + d(2,12) + ..... + d(8,12) + d(13,17)+...]
The constant C trades off the importance of semantic coherence of one session versus reducing conflict across parallel sessions.
Our goal is to find a schedule with the maximum goodness.
