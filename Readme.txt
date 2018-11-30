Objective:
Study and Implementation of PageRank Algorithm and improvement of execution time.
Implementation of PageRank in real time application where ranks of team/individuals playing sports is calculated.

We Used Power Iteration Method for Page Rank calculations.
   Power Iteration :
    • Initialize: R0 = [1/N,…,1/N]
    • Iterate: Rank at k+1 iteration = M  * Rank at Kth iteration
    • Stop when | Rk+1 – Rk | < Convergence Value (0.0001)
where M is Adjacency matrix calculated as
     Mij = d*(1 / outdegree(j)) + (1-d) / N
     R0 is Initial Rank vector initialised as given.
     Rk+1 Rank at k+1th iteration
     Rk Rank at kth iteration


Sequential Implementation:
Using power matrix by traditional matrix multiplication.

Parallel Implementation:
The computation intensive areas of Sequential Page Rank Algorithm are identified and converted to parallel using OpenMP so that PageRank algorithm is made efficient in terms of Execution time.
using Pragma Directives in OpenMP



Input for Pagerank.cpp and ParallelPagerank.cpp:  
    1. Number of nodes for which rank is to be calculated
    2. No of links/edges
    3. Source and Destination of the links in range[0,nodes-1].

Output:
Ranks of all the links in order of ranks.

Input for Pagerankapp.cpp:
    1. Type of game - 1.Team sport(cricket,football)  2.Individual games(Race,swimming)
    2. (i). For Team Sport,
                a. No of Teams
                b. No of Tournaments
                c. No of matches in each Tournament
                d. Winner and Loser Team Id for each Match 
       (ii).For Individual Sport,
                a. No of Participants
                b. No of Races  
                c. Race Completion order for each race

Output:
(i) . For Team Sport,
       The Rank,Score of Teams is displayed in order of Ranks after completion of each tournament.
(ii) . For Individual Game,
        The Ranks ,Score of  Teams is displayed in order of Ranks after completion of each race.
          
