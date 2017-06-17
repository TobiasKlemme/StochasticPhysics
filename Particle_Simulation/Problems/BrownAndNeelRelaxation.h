/* BrownAndNeelRelaxation Class
* Describes the combined neel and brown rotation
* Author: Alexander Neumann
* Date:   23.08.2015
*/

#pragma once

#ifndef _BROWNANDNEELRELAXATION_H_
#define _BROWNANDNEELRELAXATION_H_

#include "SDEFramework/GeneralSDEProblem.h"
#include "Results/SingleSimulationResult.h"

#include "Helpers/ParameterCalculatorBrownAndNeel.h"


namespace Problems
{

	/*For constexpr to be useable in class definition they have to be defined before the class
	* can not put them into the class because the initialisation of the variables is done after
	* the class is completed so it will always give an error that the variables are not defined
	* if they are within the class
	*/
	constexpr static struct BrownAndNeelDimension : public GeneralSDEDimension<6, 3, 6> //thats pretty handy
	{ } BrownAndNeelDimensionVar; //too get the memory space

	template<typename precision, typename aniso>
	class BrownAndNeelRelaxation : public GeneralSDEProblem<BrownAndNeelRelaxation<precision, aniso>>
	{
	public: //Public Typedefs
		typedef BrownAndNeelRelaxation<precision, aniso>														ThisClass;
		typedef	SDEProblem_Traits<ThisClass>																	Traits;
		typedef precision																						Precision;

		typedef typename Traits::Dimension																	    Dimension;
		typedef typename Traits::ProblemSettings															    ProblemSettings;
		typedef typename Traits::UsedProperties																	UsedProperties;
		typedef typename Traits::InitSettings																	InitSettings;
		typedef typename Traits::NecessaryProvider																NecessaryProvider;
		typedef typename Traits::SimulationParameters															SimulationParameters;

		typedef aniso																							Anisotropy;

		typedef typename Traits::StochasticMatrixType															StochasticMatrixType;
		typedef typename Traits::DeterministicVectorType														DeterministicVectorType;
		typedef typename Traits::DependentVectorType															DependentVectorType;
		typedef typename Traits::IndependentVectorType															IndependentVectorType;
		typedef typename Traits::NoiseVectorType																NoiseVectorType;



	private:
		typedef typename Traits::SubProblemMatrix																Matrix3x3;
		typedef typename Traits::SubVector																		Vec3D;

	private:
		//Function pointers to include different cases
		StochasticMatrixType(BrownAndNeelRelaxation<precision, aniso>::* const toStochasticMatrix)(const DependentVectorType& yi) const noexcept = nullptr;
		DeterministicVectorType(BrownAndNeelRelaxation<precision, aniso>::* const toDrift)(const DependentVectorType& yi) const noexcept = nullptr;

		const Helpers::BrownAndNeelMixedParams<Precision> _ParamHelper;

		//TODO: Move those to private section; Refactor code a bit;
	public:
		const UsedProperties		_ParParams;
		const InitSettings          _Init;
		const ProblemSettings		_ProbSet;
		const Anisotropy			_Anisotropy;

	private:

		//The different cases
		inline auto getStratonovichtoItoFull(const DependentVectorType& yi) const noexcept->DeterministicVectorType;
		inline auto getStratonovichtoItoSimplified(const DependentVectorType& yi) const noexcept->DeterministicVectorType;

		inline auto getStochasticMatrixFull(const DependentVectorType& yi) const noexcept->StochasticMatrixType;
		inline auto getStochasticMatrixSimplified(const DependentVectorType& yi) const noexcept->StochasticMatrixType;

	public:
		EIGEN_MAKE_ALIGNED_OPERATOR_NEW;

		//explicit BrownAndNeelRelaxation(const ProblemSettings& ProbSettings, SimulationParameters& Properties);
		explicit BrownAndNeelRelaxation(const ProblemSettings& ProbSettings, const UsedProperties &Properties, const InitSettings& Init);

		inline auto getStochasticMatrix(const DependentVectorType& yi) const noexcept-> StochasticMatrixType;
		inline auto getDrift(const DependentVectorType& yi) const noexcept-> DeterministicVectorType;
		inline auto getDeterministicVector(const DependentVectorType& yi, const IndependentVectorType& xi) const noexcept->DeterministicVectorType;

		inline void afterStepCheck(DependentVectorType& yi) const noexcept;

		inline decltype(auto) getStart() const noexcept;

		inline DependentVectorType getWeighting() const noexcept
		{
			DependentVectorType scale{ DependentVectorType::Ones() };

			scale.template tail<3>() *= _ParParams.getMagneticProperties().getSaturationMoment(); // Neel Direction Vector  

			return scale;
		};
	};

}
#include "BrownAndNeelRelaxation.inl"
#include "Definitions/BrownAndNeel_Definitions.h"


#endif //_BROWNANDNEELRELAXATION_H_