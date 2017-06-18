/*
* Author: Alexander Neumann
* Date : 23.08.2015
*/

#pragma once

#ifndef _GeneralSDESolver_H_
#define _GeneralSDESolver_H_

#include "../Basic_Library/stdext/std_extensions.h"
//#include <Eigen\Core>

/* GeneralSDESolver Class
*  Represents an interface witch every SDESolver should inherit
*  Template problem is a class of type GeneralSDEProblem
*  Template precision is either float or double
*/

//
namespace SDE_Framework
{
	namespace detail
	{
		template <bool isIto = true>
		struct FixedTimestepSelector
		{
			template<typename Solver,typename DependentVectorType, typename IndependentVectorType>
			BASIC_ALWAYS_INLINE static auto SelectImpl(Solver&& sol, DependentVectorType&& yi, IndependentVectorType&& xi) noexcept
			{
				return sol.getResultNextFixedTimestepIto(yi, xi);
			}
		};

		template <>
		struct FixedTimestepSelector<false>
		{
			template<typename Solver, typename DependentVectorType, typename IndependentVectorType>
			BASIC_ALWAYS_INLINE static auto SelectImpl(Solver&& sol, DependentVectorType&& yi, IndependentVectorType&& xi) noexcept
			{
				return sol.getResultNextFixedTimestepStratonovich(std::forward<DependentVectorType>(yi), std::forward<IndependentVectorType>(xi));
			}
		};
	}

	template<typename solver, typename problem, typename noisefield>
	class GeneralSDESolver
	{
	public:
		using SolverType = solver;
		using ProblemType = problem;

		typedef typename problem::DependentVectorType																   ResultType;
		typedef		     noisefield																					   NoiseField;
	private:
		typedef typename problem::Precision																			   Precision;
		typedef typename problem::Dimension																			   Dimensions;
		typedef typename problem::DependentVectorType																   DependentVectorType;
		typedef typename problem::IndependentVectorType																   IndependentVectorType;
		typedef typename problem::DeterministicVectorType															   DeterministicVectorType;
		typedef typename problem::StochasticMatrixType																   StochasticMatrixType;
	protected:
		GeneralSDESolver() {};
	private:
		DISALLOW_COPY_AND_ASSIGN(GeneralSDESolver)

		SolverType& self() noexcept// Handy Helper to remove the static_casts from the code
		{
			return *static_cast<SolverType * const>(this);
		};
	protected:
		const ProblemType& m_problem;	// Reference to the problem; should not be a problem since this class is always used with a Simulator Class which holds the problem and the solver
		const Precision m_timestep;		//Reference to the timestep since 
		mutable NoiseField m_dWgen;     //Random Vector; Generator

	public:
		constexpr BASIC_ALWAYS_INLINE GeneralSDESolver(const ProblemType &sdeprob, Precision timestep) : m_problem(sdeprob), m_timestep(timestep), m_dWgen(1000000, timestep) {};

		constexpr BASIC_ALWAYS_INLINE auto getResultNextTimestep(const DependentVectorType &yi, const IndependentVectorType &xi) const noexcept -> ResultType
		{
			return self().getResultNextFixedTimestep(yi, xi);
		};

		constexpr BASIC_ALWAYS_INLINE auto getResultNextFixedTimestep(const DependentVectorType &yi, const IndependentVectorType &xi) const noexcept-> ResultType //decltyp(solver().getResultNextFixedTimestep(yi, xi))
		{
			return self().getResultNextFixedTimestep(yi, xi);
		};

		constexpr BASIC_ALWAYS_INLINE const Precision& getTimestep() const noexcept { return m_timestep; };
	};
}
#endif