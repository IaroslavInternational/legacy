#pragma once

#include <vector>
#include <string>

class SceneDataReader
{
public:
	SceneDataReader(const char* path);
	~SceneDataReader();

	// 0 - ���� � ������� / 1 - ���� � ��������� / 2 - ���� � �������� ���������� �����
	std::vector<std::string>& GetPaths();
private:
	std::vector<std::string> paths;
};

