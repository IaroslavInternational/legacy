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

// ����������� ������� ��� ������
namespace EngineFunctions
{
	// ���������� ����� �������� ��� ��������� � ����� .json
	// objectName - ��� ����������� �������
	// param - �������� ��� ������
	// path - ���� � �����
	// val - ��������
	// Applog - ��������� �� ���
#if IS_ENGINE_MODE
	template<typename T>
	void SetNewValue(std::string objectName, std::string param, T val, std::string path, AppLog* applog)
#else
	template<typename T>
	void SetNewValue(std::string objectName, std::string param, T val, std::string path)
#endif // IS_ENGINE_MODE
	{
		// �������� �����
		std::ifstream dataFile(path);
		if (!dataFile.is_open())
		{
			throw ("�� ������ ������� ���� � �������");
		}

#if IS_ENGINE_MODE
		std::ostringstream ostrlog;
		ostrlog << "��������� [" << param << " : " << std::to_string(val) << "] " << "��� [" << objectName << "]\n";

		applog->AddLog(SYSTEM_LOG, ostrlog.str().c_str());
#endif // IS_ENGINE_MODE

		// ������ �����
		json j;
		dataFile >> j;

		// �������� �����
		dataFile.close();

		for (json::iterator m = j.begin(); m != j.end(); ++m)
		{
			for (auto& obj : j.at(objectName))
			{
				obj[param] = val;
			}
		}

		// ������ � ����
		std::ofstream ostr(path);
		ostr << j.dump();

		// �������� �����
		ostr.close();
	}

	// ��������� ��� ������� � ���� ������ ����� ������
	template <typename T>
	std::string AttachStrings(T str1, T str2) 
	{
		std::ostringstream oss;
		oss << str1 << " " << str2;

		return oss.str();
	}
}