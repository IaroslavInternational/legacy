#pragma once

#include "EngineConverter.h"

#if IS_ENGINE_MODE
#include "AppLog.h"
#endif // IS_ENGINE_MODE

#include <sstream>
#include <fstream>
#include <filesystem>
#include "json.hpp"

using json = nlohmann::json;
using namespace std::string_literals;

namespace EngineFunctions
{
	// Установить значение 
#if IS_ENGINE_MODE
	template<typename T>
	void SetNewValue(std::string objectName, std::string param, T val, std::string path, AppLog* applog)
#else
	template<typename T>
	void SetNewValue(std::string objectName, std::string param, T val, std::string path)
#endif // IS_ENGINE_MODE
	{
		// Открытие файла
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("Не удаётся открыть файл с данными");
		}

#if IS_ENGINE_MODE
		std::ostringstream ostrlog;
		ostrlog << "Установка [" << param << " : " << std::to_string(val) << "] " << "для [" << objectName << "]\n";

		applog->AddLog(SYSTEM_LOG, ostrlog.str().c_str());
#endif // IS_ENGINE_MODE

		// Чтение файла
		json j;
		dataFile >> j;

		// Закрытие файла
		dataFile.close();

		for (json::iterator m = j.begin(); m != j.end(); ++m)
		{
			for (auto& obj : j.at(objectName))
			{
				obj[param] = val;
			}
		}

		// Запись в файл
		std::ofstream ostr(path);
		ostr << j.dump();

		// Закрытие файла
		ostr.close();
	}
}