//////////////////////////////////////
//
// Tools.h
// Ke Wang
//
// Some common operations maybe of use
//
//////////////////////////////////////
// Version History:
// 29th, Mar, 2014 -- Created
//////////////////////////////////////

#include <string>
#include <iostream>

class Tools
{
public:
	Tools(void);
	~Tools(void);
	static void warning(char* msg);
	static void warning(std::string msg);
	static void errorAndExit(char* msg, int exitCode =1);
	static void errorAndEixt(std::string msg, int exitCode = 1);
	
	static std::string stringToHex(std::string); // convert a string to a hex string according to the byte
	static std::string hexToString(std::string); // convert back a string presented by hex to original string
};

