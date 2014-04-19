#include "Tools.h"
#include <iostream>

using namespace std;

Tools::Tools(void)
{
}


Tools::~Tools(void)
{
}

void Tools::warning(char* msg) {
	cout << "WARNING: " << msg << endl;
}

void Tools::warning(std::string msg) {
	cout << "WARNING: " << msg << endl;
} 

void Tools::errorAndExit(char* msg, int exitCode) {
	cout << "ERROR: " << msg << endl;
	exit(exitCode);
}

static void errorAndEixt(std::string msg, int exitCode) {
	cout << "ERROR: " << msg << endl;
	exit(exitCode);
}