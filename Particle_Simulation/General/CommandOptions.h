///-------------------------------------------------------------------------------------------------
// file:	CommandOptions.h
//
// summary:	Declares the command options class
///-------------------------------------------------------------------------------------------------
#pragma once

#ifndef _COMMANDOPTIONS_H_
#define _COMMANDOPTIONS_H_

#include "Setup.h"

//The Template does nothing! Look at CPP to see specializations
template<typename App>
class CommandOptions
{
private:

protected:
	CommandOptions() {};
	~CommandOptions() {};
public:
	using Application = App;
	using InputArchive = typename Application::StartInputArchive;
	using OutputArchive = typename Application::StartOutputArchive;

	static void registerOptions() {};
	static void analyseCommandParameters(const int /*argc*/, char** /*argv*/) {};
	static InputArchive getInputArchive();
	//Application getApplication() { return Application{ Application::Parameters{} }; };
};


/*
Kommandozeilenparameter-Kombinationen:
- keiner: "Example_Simulation_Settings.ini" ohne Multivoxel (bei Fehlen von -sysmatrix wird keine Bsp.-Systemmatrix erstellt und direkt angewandt)
- -parfile: Parameterfile ohne Multivoxel
- -sysmatrix: "Example_Simulation_Settings.ini" mit Multivoxel
- -parfile -sysmatrix: Parameterfile mit Multivoxel
*/

#include <string>

#include "basics/StartOptions.h"

#include "Simulator/SimulationManager.h" // Should include everything!

template<>
class CommandOptions<SimulationApplication::SimulationManager<PREC>>
{
public:
	using Application = SimulationApplication::SimulationManager<PREC>;
	using InputArchive = Application::StartInputArchive;
	using OutputArchive = Application::StartOutputArchive;

	//Statische variablen in einer rein statischen Klasse k�nnen gerne public sein. keine notwendigkeit f�r getter und setter!
	static bool useSystemMatrix;

private:
	static StartOptions _StartOptions;
	static std::unique_ptr<InputArchive> _pCFG_Input;
	static std::unique_ptr<InputArchive> _pCFG_InputSysMat;

	// Simulation Parameter Option
	static void SimulationParametersLoad(std::string filename);
	static void SimulationParametersCreate();
	static void SimulationParametersRegister();
	
	//SystemMatrix Parameters
	static void SystemMatrixParametersLoad(std::string filename);
	static void SystemMatrixParametersCreate();
	static void SystemMatrixParametersRegister();

	//Help Option
	static void HelpLoad(std::string);
	static void RegisterHelp();

	//Registers all Options
	static void RegisterAll();

protected:
	//Make sure we do not create a instance of this class somewhere
	CommandOptions() = default;
	~CommandOptions() = default;
public:

	static void registerOptions()
	{
		RegisterAll();
	};
	static void analyseCommandParameters(const int argc, char** argv)
	{
		_StartOptions.analyzeStartArguments(argc, argv);
	};
	static InputArchive getInputArchive()
	{
		return std::move(*_pCFG_Input);
	};
	static InputArchive getInputSystemMatrixArchive()
	{
		return std::move(*_pCFG_InputSysMat);
	};
};

#endif //_COMMANDOPTIONS_H_