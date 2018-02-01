#pragma once

#include <Windows.h>
#include <string>
#include <iostream>

namespace ee
{
	typedef bool(*initSimulation_func)(void);
	typedef void(*stepSimulation_func)(long double, bool);
	typedef void(*processDrawState_func)(void);
	typedef int(*getDrawStateCount_func)(void);
	typedef void(*prepareDrawStateObject_func)(int);
	typedef int(*getDrawState_primitive_func)(void);
	typedef const float*(*getDrawState_posmat_func)(void);
	typedef const float*(*getDrawState_invposmat_func)(void);
	typedef const float*(*getDrawState_data_func)(void);
	typedef int(*getDrawState_dataSize_func)(void);
	typedef const char*(*getDrawState_name_func)(void);

	class SimulationLoader
	{
	public:
		SimulationLoader() :
			m_module(nullptr)
		{
		}

		bool loadModule(const std::string &moduleName)
		{
			m_module = LoadLibrary(moduleName.c_str());
			return m_module != nullptr;
		}

		bool loadFunctions()
		{
			if (m_module == nullptr)
			{
				return false;
			}

			initSimulation				= (initSimulation_func)GetProcAddress(m_module, "initSimulation");
			stepSimulation				= (stepSimulation_func)GetProcAddress(m_module, "stepSimulation");
			processDrawState			= (processDrawState_func)GetProcAddress(m_module, "processDrawState");
			getDrawStateCount			= (getDrawStateCount_func)GetProcAddress(m_module, "getDrawStateCount");
			prepareDrawStateObject		= (prepareDrawStateObject_func)GetProcAddress(m_module, "prepareDrawStateObject");
			getDrawState_primitive		= (getDrawState_primitive_func)GetProcAddress(m_module, "getDrawState_primitive");
			getDrawState_posmat			= (getDrawState_posmat_func)GetProcAddress(m_module, "getDrawState_posmat");
			getDrawState_invposmat		= (getDrawState_invposmat_func)GetProcAddress(m_module, "getDrawState_invposmat");
			getDrawState_data			= (getDrawState_data_func)GetProcAddress(m_module, "getDrawState_data");
			getDrawState_dataSize		= (getDrawState_dataSize_func)GetProcAddress(m_module, "getDrawState_dataSize");
			getDrawState_name			= (getDrawState_name_func)GetProcAddress(m_module, "getDrawState_name");

			return (initSimulation && stepSimulation && processDrawState && getDrawStateCount && prepareDrawStateObject &&
				getDrawState_primitive && getDrawState_posmat && getDrawState_invposmat && getDrawState_data && getDrawState_dataSize && getDrawState_name);
		}

	public:
		initSimulation_func				initSimulation;
		stepSimulation_func				stepSimulation;
		processDrawState_func			processDrawState;
		getDrawStateCount_func			getDrawStateCount;
		prepareDrawStateObject_func		prepareDrawStateObject;
		getDrawState_primitive_func		getDrawState_primitive;
		getDrawState_posmat_func		getDrawState_posmat;
		getDrawState_invposmat_func		getDrawState_invposmat;
		getDrawState_data_func			getDrawState_data;
		getDrawState_dataSize_func		getDrawState_dataSize;
		getDrawState_name_func			getDrawState_name;

	private:
		HMODULE m_module;
	};
}