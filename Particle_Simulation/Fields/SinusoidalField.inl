/*
* Author: Alexander Neumann
* Date : 23.08.2015
*/

//#include "SinusoidalField.h" // File is inclued by header!
//
//#define _USE_MATH_DEFINES
//#include <cmath> // For M_PI

constexpr auto m_pi = 3.14159265358979323846;
//
////Constructor
//template<typename precision>
//SinusoidalField<precision>::SinusoidalField(const FieldParameters& params)
//	: GeneralField<precision, SinusoidalField<precision>, 3>(), _params(params),
//	_angularfrequency(2 * m_pi *(*params.getFrequencies().begin())),
//	_phase(*params.getPhases().begin()), _offset(*params.getAmplitudes().begin()),
//	_ampDirection(*(++(params.getAmplitudes().begin())))
//{
//	//assert(_angularfrequency > 0), "Sinusoidal Field Frequency should be greater than zero!"));
//	//_period = 2 * M_PI / _angularfrequency;
//};

//Constructor
template<typename precision>
SinusoidalField<precision>::SinusoidalField(const FieldProperties& params)
	: GeneralField<SinusoidalField<precision>>(), _params(params),
	_angularfrequency(2 * m_pi *(*params.getFrequencies().begin())),
	_phase(*params.getPhases().begin()), _ampDirection(*(++(params.getAmplitudes().begin()))), _offset(*params.getAmplitudes().begin())
	
{
	//assert(_angularfrequency > 0), "Sinusoidal Field Frequency should be greater than zero!"));
	//_period = 2 * M_PI / _angularfrequency;
};

//Getter for the field Value; actual function is defined in the constructor
template<typename precision>
inline typename SinusoidalField<precision>::FieldVector SinusoidalField<precision>::getField(const precision time)  const
{
	return (_ampDirection*sin(_angularfrequency*time + _phase)+_offset);
}

 