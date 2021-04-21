#pragma once

#include <vector>
#include <string>

class SceneDataReader
{
public:
	SceneDataReader(const char* path);
	~SceneDataReader();

	// 0 - Путь к моделям
	// 1 - Путь к триггерам 
	// 2 - Путь к точечным источникам света
	// 3 - Путь к камерам
	std::vector<std::string>& GetPaths();
private:
	std::vector<std::string> paths;
};

