/*
 * Declarations for Deme class to evolve a genetic algorithm for the
 * travelling-salesperson problem.  A deme is a population of individuals.
 */

#include "chromosome.hh"
#include "deme.hh"
#include<cassert>

// Generate a Deme of the specified size with all-random chromosomes.
// Also receives a mutation rate in the range [0-1].
Deme::Deme(const Cities* cities_ptr, unsigned pop_size, double mut_rate)
{
	this->mut_rate_ = mut_rate;
	this->cities_ = cities_ptr;
	pop_ = std::vector<Chromosome*>();
	for (auto i = 0; i < pop_size; i++) {
		pop_.push_back(new Chromosome(cities_ptr));
	}
}

// Clean up as necessary
Deme::~Deme()
{
	auto pop_size = pop_.size();
	for (auto i = 0; i < pop_size; i++) {
		delete(pop_[i]);
	}
}

// Evolve a single generation of new chromosomes, as follows:
// We select pop_size/2 pairs of chromosomes (using the select() method below).
// Each chromosome in the pair can be randomly selected for mutation, with
// probability mut_rate, in which case it calls the chromosome mutate() method.
// Then, the pair is recombined once (using the recombine() method) to generate
// a new pair of chromosomes, which are stored in the Deme.
// After we've generated pop_size new chromosomes, we delete all the old ones.
void Deme::compute_next_generation()
{
	auto new_pop = std::vector<Chromosome*>();
	for (int i = 0; i < ceil(pop_.size()/2);i++) {
		Chromosome* c1 = select_parent();
		Chromosome* c2 = select_parent();
		std::uniform_real_distribution<double> dis(0.0, 1);
		double d1 = dis(generator_);
		double d2 = dis(generator_);
		if (d1 < mut_rate_) {
			c1->mutate();
		}
		if (d2 < mut_rate_) {
			c2->mutate();
		}
		auto kids = c1->recombine(c2);
		new_pop.push_back(kids.first);
		new_pop.push_back(kids.second);
	}
	if (pop_.size() != new_pop.size()) {
		new_pop.pop_back();
	}
	pop_ = new_pop;

	

}

// Return a copy of the chromosome with the highest fitness.
const Chromosome* Deme::get_best() const
{
	const Chromosome* best = pop_[0];
	double bestfit = best->get_fitness();
	for (auto i = 1; i < pop_.size(); i++) {
		if (pop_[i]->get_fitness() > bestfit){
			best = pop_[i];
			bestfit = pop_[i]->get_fitness();
			}
	}
	return best->clone();
}

// Randomly select a chromosome in the population based on fitness and
// return a pointer to that chromosome.
Chromosome* Deme::select_parent()
{
	double sum = 0;
	for (auto i = 0; i < pop_.size(); i++) {
		sum += pop_[i]->get_fitness();
	}
	std::uniform_real_distribution<double> dis(0.0, sum);
	double threshhold = dis(generator_);
	double accumulation = 0;
	int i = 0;
	while (accumulation < threshhold) {
		accumulation += pop_[i]->get_fitness();
		i++;
	}
	return pop_[i - 1];
}
