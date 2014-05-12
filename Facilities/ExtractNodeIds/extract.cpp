#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

int main() {
	fstream file("cal.csv",ios::in);
	string s;
	int index = -1;
	vector<int> results;
	while (getline(file,s)){
		index++;
		if (index == 8)
			index = 0;
		if (index == 0)
			results.push_back(atoi(s.c_str()));
	}
	fstream output("results.txt",ios::out|ios::trunc);
	for (int i = 0;i != results.size();i++) {
		output << results[i] << endl;
	}
	return 0;
}