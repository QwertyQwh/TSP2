/*
 * Implementation for Chromosome class
 */

#include <algorithm>
#include <cassert>
#include <random>
#include <set>
#include "chromosome.hh"

//////////////////////////////////////////////////////////////////////////////
// Generate a completely random permutation from a list of cities
Chromosome::Chromosome(const Cities* cities_ptr)
  : cities_ptr_(cities_ptr),
    order_(random_permutation(cities_ptr->size())),
    generator_(rand())
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Clean up as necessary
Chromosome::~Chromosome()
{
  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Perform a single mutation on this chromosome
void
Chromosome::mutate()
{
	std::uniform_int_distribution<int> dis(0,order_.size()-1);
	int first = dis(generator_);
	int second = dis(generator_);
	while (first == second) {
		first = dis(generator_);
		second = dis(generator_);
	}
	std::iter_swap(order_.begin() + first, order_.begin() + second);

  assert(is_valid());
}

//////////////////////////////////////////////////////////////////////////////
// Return a pair of offsprings by recombining with another chromosome
// Note: this method allocates memory for the new offsprings
std::pair<Chromosome*, Chromosome*>
Chromosome::recombine(const Chromosome* other)
{
  assert(is_valid());
  assert(other->is_valid());
  std::uniform_int_distribution<int> dis(0, order_.size());
  int first = dis(generator_);
  int second = dis(generator_);
  while (first >= second) {
	  first = dis(generator_);
	  second = dis(generator_);
  }
  return std::pair<Chromosome*,Chromosome*> (create_crossover_child(this, other, first, second),create_crossover_child(other, this,first, second));

}

//////////////////////////////////////////////////////////////////////////////
// For an ordered set of parents, return a child using the ordered crossover.
// The child will have the same values as p1 in the range [b,e),
// and all the other values in the same order as in p2.
Chromosome*
Chromosome::create_crossover_child(const Chromosome* p1, const Chromosome* p2,
                                   unsigned b, unsigned e) const
{
  Chromosome* child = p1->clone();

  // We iterate over both parents separately, copying from parent1 if the
  // value is within [b,e) and from parent2 otherwise
  unsigned i = 0, j = 0;

  for ( ; i < p1->order_.size() && j < p2->order_.size(); ++i) {
    if (i >= b && i < e) {
      child->order_[i] = p1->order_[i];
    }
    else { // Increment j as long as its value is in the [b,e) range of p1
      while (p1->is_in_range(p2->order_[j], b, e)) {
        ++j;
      }
      assert(j < p2->order_.size());
      child->order_[i] = p2->order_[j];
      j++;
    }
  }

  assert(child->is_valid());
  return child;
}

// Return a positive fitness value, with higher numbers representing
// fitter solutions (shorter total-city traversal path).
double
Chromosome::get_fitness() const
{
	return 1/pow(cities_ptr_->total_path_distance(order_)/10000,30);
}

// A chromsome is valid if it has no repeated values in its permutation,
// as well as no indices above the range (length) of the chromosome.
// We implement this check with a sort, which is a bit inefficient, but simple
bool
Chromosome::is_valid() const
{
	//check whether the elements are unique
	std::set<unsigned int> s (order_.begin(), order_.end());
	Cities::permutation_t vec;
	vec.assign(s.begin(), s.end());
	if (vec.size() != order_.size()) {
		return false;
	}
	//check whether the elements are within range
	for (auto i = 0; i < order_.size();i++) {
		if (order_[i] > order_.size()) {
			return false;
		}
	}
}

// Find whether a certain value appears in a given range of the chromosome.
// Returns true if value is within the specified the range specified
// [begin, end) and false otherwise.
bool
Chromosome::is_in_range(unsigned value, unsigned begin, unsigned end) const
{
	for (auto i = begin; i < end; i++) {
		if (order_[i] == value) {
			return true;
		}
	}
	return false;
}
