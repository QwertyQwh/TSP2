Final GA distance: 7081.66
Final Randomized distance: 19472

Important parameters for GA Algorithm:

Fitness function:
Calculate the total distance of the permutation and name it tl.
Apply f(x) = 1/((x/10000)^30) to tl and return f(x) as the fitness value for this permutation.
This way the fitness gap between two permutations with little difference in total_distance would be amplified.


Population = 26

Mutation_rate = 0.25


