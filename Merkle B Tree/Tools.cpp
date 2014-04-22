#include "Tools.h"
#include <iostream>
#include <string>
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

void Tools::errorAndEixt(std::string msg, int exitCode) {
	cout << "ERROR: " << msg << endl;
	exit(exitCode);
}

std::string Tools::stringToHex(std::string s) {
	string hex = "";
	for (int i = 0;i != s.size();i++) {
		unsigned char ch = s[i];
		unsigned char more = ch / 16;
		unsigned char less = ch % 16;
		switch(more) {
		case 0:
			hex += "0";
			break;
		case 1:
			hex += "1";
			break;
		case 2:
			hex += "2";
			break;
		case 3:
			hex += "3";
			break;
		case 4:
			hex += "4";
			break;
		case 5:
			hex += "5";
			break;
		case 6:
			hex += "6";
			break;
		case 7:
			hex += "7";
			break;
		case 8:
			hex += "8";
			break;
		case 9:
			hex += "9";
			break;
		case 10:
			hex += "a";
			break;
		case 11:
			hex += "b";
			break;
		case 12:
			hex += "c";
			break;
		case 13:
			hex += "d";
			break;
		case 14:
			hex += "e";
			break;
		case 15:
			hex += "f";
			break;
		default:
			cout << "The program should not be here. Check Tools::stringToHex().\n";
			exit(1);
		}
		switch(less) {
		case 0:
			hex += "0";
			break;
		case 1:
			hex += "1";
			break;
		case 2:
			hex += "2";
			break;
		case 3:
			hex += "3";
			break;
		case 4:
			hex += "4";
			break;
		case 5:
			hex += "5";
			break;
		case 6:
			hex += "6";
			break;
		case 7:
			hex += "7";
			break;
		case 8:
			hex += "8";
			break;
		case 9:
			hex += "9";
			break;
		case 10:
			hex += "a";
			break;
		case 11:
			hex += "b";
			break;
		case 12:
			hex += "c";
			break;
		case 13:
			hex += "d";
			break;
		case 14:
			hex += "e";
			break;
		case 15:
			hex += "f";
			break;
		default:
			cout << "The program should not be here. Check Tools::stringToHex().\n";
			exit(1);
		}
	}
	return hex;
}

string Tools::hexToString(string hex) {
	if (hex.length() == 0 || hex.length() % 2 == 1) {
		cout << "The Program Should NOT Go Here.\n";
	}
	string s;
	s.resize(hex.length()/2);
	int index = 0;
	char more,less;
	while (index < hex.length()) {
		more = hex[index];
		less = hex[index+1];
		index += 2;
		unsigned char ch;
		switch (more) {
		case '0':
			ch = 0;
			break;
		case '1':
			ch = 16;
			break;
		case '2':
			ch = 32;
			break;
		case '3':
			ch = 48;
			break;
		case '4':
			ch = 64;
			break;
		case '5':
			ch = 80;
			break;
		case '6':
			ch = 96;
			break;
		case '7':
			ch = 112;
			break;
		case '8':
			ch = 128;
			break;
		case '9':
			ch = 144;
			break;
		case 'a':
		case 'A':
			ch = 160;
			break;
		case 'b':
		case 'B':
			ch = 176;
			break;
		case 'c':
		case 'C':
			ch = 192;
			break;
		case 'd':
		case 'D':
			ch = 208;
			break;
		case 'e':
		case 'E':
			ch = 224;
			break;
		case 'f':
		case 'F':
			ch = 240;
			break;
		default:
			cout << "The Program Should NOT Go Here!\n";
			exit(1);
		}
		switch (less) {
		case '0':
			ch += 0;
			break;
		case '1':
			ch += 1;
			break;
		case '2':
			ch += 2;
			break;
		case '3':
			ch += 3;
			break;
		case '4':
			ch += 4;
			break;
		case '5':
			ch += 5;
			break;
		case '6':
			ch += 6;
			break;
		case '7':
			ch += 7;
			break;
		case '8':
			ch += 8;
			break;
		case '9':
			ch += 9;
			break;
		case 'a':
		case 'A':
			ch += 10;
			break;
		case 'b':
		case 'B':
			ch += 11;
			break;
		case 'c':
		case 'C':
			ch += 12;
			break;
		case 'd':
		case 'D':
			ch += 13;
			break;
		case 'e':
		case 'E':
			ch += 14;
			break;
		case 'f':
		case 'F':
			ch += 15;
			break;
		default:
			cout << "The Program Should NOT Go Here!\n";
			exit(1);
		}
		s[index/2-1] = ch;
	}
	return s;
}