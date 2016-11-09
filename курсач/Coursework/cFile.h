#pragma once
#include "Coursework.h"
using namespace std;

typedef map<string, vector<string>> Map;
typedef map<int, vector<string>> Res;

class cFile
{
public:
	cFile();
	cFile(string);
	~cFile();
	Map		files;
	Res		result;
	string	retStr();

private:
	void	add(string, string);
	void	folParse(string, string);
	void	fileParse(string, string);
	void	buildIer(Map);
	void	buildIer2(Map, byte);
	bool	testVec(string, byte);
	bool	testLev(string, byte);
	bool	testAll( byte);
	bool	test(string);
	void	vecDel(vector<string>, string);
	bool	testVec2(string, byte);
};