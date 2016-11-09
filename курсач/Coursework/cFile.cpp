#include <stdafx.h>
#include "cFile.h"
#include <string>
#include <algorithm>

void	cFile::add(string key, string value)
{
	files[key].push_back(value);
}

void	cFile::fileParse(string fullPath, string fileName)
{
	int			i, j;
	char		buff[2550];
	string		res;
	ifstream	fin(fullPath);
	bool		flag = 0;
	
	fin.getline(buff, 50);
	while (strstr(buff, "#include") && !strstr(buff, "//#include"))
	{
		while (strstr(buff, "\""))
		{
			flag = 1;
			i = j = -1;
			j++;
			while (buff[++i] != '\"');
			while (buff[i + (++j)] != '\"');
			res = string(buff);
			add(fileName, res.substr(i + 1, j - 1));
			//files.insert(Map::value_type(fileName, (std::string(buff)).substr(i, j + 1)));
			res += (std::string(buff)).substr(i, j + 1);
			fin.getline(buff, 50);
		}
		while (strstr(buff, "<"))
		{
			flag = 1;
			i = j = -1;
			while (buff[++i] != '<');
			while (buff[i + (++j)] != '>');
			res = string(buff);
			add(fileName, res.substr(i + 1, j - 1));
			//files.insert(Map::value_type(fileName, (std::string(buff)).substr(i, j + 1)));
			res += (std::string(buff)).substr(i, j + 1);
			fin.getline(buff, 50);
		}		
	}
	if (!flag)
		add(fileName, "0");
}

void	cFile::vecDel(vector<string> vec, string d)
{
	for (auto i = vec.begin(); i != vec.end(); ++i)
	{
		if ((*i) == d)
		{
			vec.erase(i);
			i--;
		}
	}
}

void	cFile::buildIer(Map files)
{
	bool flag;
	byte level = 0;

	for (auto i = files.begin(); i != files.end(); ++i)
	{
		/*for (auto it = files.begin(); it != files.end(); ++it)
		{
			flag = 1;
			for (auto itt = files[(*it).first].begin(); itt < files[(*it).first].end(); ++itt)
			{
				flag = 0;
				if ((*i).first != (*it).first && (*i).first != (*itt) && (*itt) != "0")
					flag = 1;
			}
		}*/
		if (test((*i).first))
		{
			result[level].push_back((*i).first);
		}
	}
	
	while (!testAll(level++))
	{
		for (auto it = files.begin(); it != files.end(); ++it)
		{
			if (testVec((*it).first, level) && testLev((*it).first, level))
				result[level].push_back((*it).first);
		}		
	}
}

bool	cFile::test(string first_name)
{
	for (auto it = files.begin(); it != files.end(); ++it)
	{
		for (auto itt = files[(*it).first].begin(); itt < files[(*it).first].end(); ++itt)
		{
			if (first_name != (*it).first && first_name == (*itt) && (*itt) != "0")
				return false;
		}
	}
	return true;
}

//void	cFile::buildIer2(Map files, byte level)
//{
//	for (auto i = files.begin(); i != files.end(); ++i)
//	{
//		for (auto it = files.begin(); it != files.end(); ++it)
//		{
//			for (auto itt = files[(*it).first].begin(); itt < files[(*it).first].end(); itt++)
//			{
//				
//			}
//		}
//	}
//
//}

bool	cFile::testVec(string name, byte level)		// перевірка чи э данний файл в минулих рівнях
{
	//int i = 0;

	/*for (auto first = result[level].begin(); first != result[level].end(); ++first)
	{*/
		for (auto i = 0; i < level; i++)
		{
			for (auto second = result[i].begin(); second != result[i].end(); second++)
			{
				if (name == (*second))
				{
					//result[i].erase(std::remove(result[i].begin(), result[i].end(), *second), result[i].end());
					//vecDel(result[i], *second);
					return false;
				}
			}
		}
	//}
	return true;
}
//
//bool	cFile::testVec2(string name, byte level)		// перевірка чи э данний файл в минулих рівнях
//{
//	//int i = 0;
//
//	/*for (auto first = result[level].begin(); first != result[level].end(); ++first)
//	{*/
//	for (auto i = 0; i < level; i++)
//	{
//		for (auto second = result[i].begin(); second != result[i].end(); second++)
//		{
//			if (name == (*second))
//			{
//				
//				return false;
//			}
//		}
//	}
//	//}
//	return true;
//}

bool	cFile::testAll(byte level) // якщо э хоть один файл  якого немаэ в рывнях, то вертаэ false
{
	for (auto it = files.begin(); it != files.end(); ++it)
	{
		if (testVec((*it).first, level))
			return false;
	}
	return true;
}

bool	cFile::testLev(string firstName, byte level)		// перевыряэ чи э цей файл в залеэностях минулого рывня
{
	for (auto it = result[level - 1].begin(); it != result[level - 1].end(); ++it)
	{
		for (auto second_name = files[(*it)].begin(); second_name != files[(*it)].end(); ++second_name)
		{
			if (firstName == (*second_name))
				return true;
		}
		/*auto second_name = files[(*it)].begin();
		while (second_name != files[(*it)].end())
		{
			if (firstName == (*second_name))
				return true;

		}*/
	}
	return false;
}

string	cFile::retStr()
{
	string res;
	ostringstream convert;
	for (auto it = result.begin(); it != result.end(); ++it)
	{
		
		convert << ((*it).first);
		res += convert.str() + " : ";
		for (auto itt = result[(*it).first].begin(); itt < result[(*it).first].end(); itt++)
		{
			res += (*itt) + " ";
		}
		res += "\n";
	}
	return res;
}

void cFile::folParse(string path, string fileD)
{
	HANDLE hFind = INVALID_HANDLE_VALUE;
	WIN32_FIND_DATA findFiles;
	string fileName, fPath;

	fPath =path + fileD;
	hFind = FindFirstFile(fPath.c_str(), &findFiles);
	do {
		if (!(findFiles.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
		{
			fileName = findFiles.cFileName;
			fileParse(path + "\\" + fileName, fileName);		
		}
	} while (FindNextFile(hFind, &findFiles) != 0);
}
cFile::cFile(string path)
{
	folParse(path, "\\*.cpp");
	folParse(path, "\\*.h");
	buildIer(files);
	return;
}

cFile::~cFile()
{
}