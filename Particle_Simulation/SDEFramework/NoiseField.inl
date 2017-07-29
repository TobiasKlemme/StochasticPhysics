/*
* Author: Alexander Neumann
* Date : 23.08.2015
*/

#pragma once

//#include "NoiseField.h" //File is included by header !

#include <utility>
#include "stdext/is_detected.h"

#ifdef USE_PCG_RANDOM
#include <pcg_extras.hpp>
#include <pcg_random.hpp>
#endif
namespace pcg_helper
{
	//Function type for member save
	template<class pcggens>
	using find_pcg_period_pow2_t = decltype(std::declval<pcggens&>().period_pow2);

	//Checks if Archive has a prologue member function
	template<typename GenToTest>
	class has_pcg_period_pow2 : public stdext::is_detected<find_pcg_period_pow2_t, GenToTest> {};
	template<typename GenToTest>
	static constexpr bool has_pcg_period_pow2_v = has_pcg_period_pow2<GenToTest>::value;
}

template<typename Generator>
Generator createSeededGenerator()
{
	if constexpr (pcg_helper::has_pcg_period_pow2_v<Generator>)
	{
		// Seed with a real random value, if available
		pcg_extras::seed_seq_from<std::random_device> seq;
		return Generator{ seq };
	}
	else
	{
		std::random_device rd;
		std::array<std::random_device::result_type, Generator::state_size> seed_data;
		std::generate(seed_data.begin(), seed_data.end(), [&]() {return rd(); });
		std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
		return Generator{ seq };
	}
}

template<typename prec, int dim, typename generator, typename NormalDistribution>
inline NoiseField<prec, dim, generator, NormalDistribution>::NoiseField(const std::size_t& NumberOfInit, const Precision& timestep)
{
	// Create Random Number Generators
	for(auto& gen : m_generators)
	{
		gen = createSeededGenerator<generator>();
		m_distribution = NormalDistribution{ 0, sqrt(timestep) };
	};
	initGenerators(NumberOfInit);
};

template<typename prec, int dim, typename generator, typename NormalDistribution>
BASIC_ALWAYS_INLINE auto NoiseField<prec, dim, generator, NormalDistribution>::getField() -> FieldVector
{

	//std::array<prec, dim> values;
	//auto rand = [](auto& gen, auto& dist) { return dist(gen); };
	//std::transform(m_generators.begin(), m_generators.end(), m_distributions.begin(), values.begin(), rand);

	//FieldVector tmp = Eigen::Map<FieldVector>(values.data());
	FieldVector tmp;
	std::size_t counter{ 0 };
	for (auto& gen : m_generators)
	{
		tmp(counter, 0) = m_distribution(gen);
		++counter;
	}
	//auto iter = m_generators.begin();
	//for (int i = dim; i--; ++iter)
	//	tmp(i, 0) = m_distribution(*iter);
	return tmp; //Fastest code so far; does not help to introduce a class member tmp
};

//template<typename prec, typename generator>
//inline auto NoiseField<prec, 6, generator>::getField()
//{
//	FieldVector tmp;
//	tmp(0, 0) = m_distribution(m_generators[0]);
//	tmp(1, 0) = m_distribution(m_generators[1]);
//	tmp(2, 0) = m_distribution(m_generators[2]);
//	tmp(3, 0) = m_distribution(m_generators[3]);
//	tmp(4, 0) = m_distribution(m_generators[4]);
//	tmp(5, 0) = m_distribution(m_generators[5]);
//	return tmp; //Fastest code so far; does not help to introduce a class member tmp
//};

template<typename prec, int dim, typename generator, typename NormalDistribution>
inline void NoiseField<prec, dim, generator, NormalDistribution>::initGenerators(const std::size_t& NumberOfInit)
{
	//std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::nanoseconds> field_init_begin, field_init_finished;
	//field_init_begin = std::chrono::high_resolution_clock::now();
	for(auto &gen : this->m_generators)
	{
		this->initGenerator(gen, NumberOfInit);
	};
	//field_init_finished = std::chrono::high_resolution_clock::now();
	//std::cout << "It took " << (field_init_finished - field_init_begin).count() / 1E6 << " ms to initialize the Generators for the NoiseField " << std::endl;

};

template<typename prec, int dim, typename generator, typename NormalDistribution>
inline void NoiseField<prec, dim, generator, NormalDistribution>::initGenerator(generator& gen, const std::size_t& NumberOfInit)
{
	gen.discard(NumberOfInit);
};

