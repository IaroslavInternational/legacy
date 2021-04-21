#include "SceneDataReader.h"

#include <sstream>
#include <fstream>
#include <filesystem>

#include "json.hpp"

using json = nlohmann::json;
using namespace std::string_literals;

SceneDataReader::SceneDataReader(const char* path)
{
	const auto dataPath = path;

	std::ifstream dataFile(dataPath);
	if (!dataFile.is_open())
	{
		throw ("�� ������ ������� ���� � ������� � �����");
	}

	json j;
	dataFile >> j;

	for (json::iterator m = j.begin(); m != j.end(); ++m)
	{
		auto& d = m.key();

		for (const auto& obj : j.at(d))
		{
			/* ������ ���� ������� */
			
			paths.emplace_back(obj.at("modelsPath"));

			/***********************/

			/* ������ ���� ��������� */

			paths.emplace_back(obj.at("triggersPath"));

			/*************************/

			/* ������ ���� ��������� */

			paths.emplace_back(obj.at("pLightsPath"));

			/*************************/

			/* ������ ���� ����� */

			paths.emplace_back(obj.at("camerasPath"));

			/*************************/
		}
	}
}

SceneDataReader::~SceneDataReader()
{
}

std::vector<std::string>& SceneDataReader::GetPaths()
{
	return paths;
}
