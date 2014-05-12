// calculate average VO size for eack k
//
// Ke Wang

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char** argv) {
	// read in csv file
	fstream inputFileStream(argv[1],ios::in);
	
	vector<long> averageVOSize;
	averageVOSize.resize(8);

	// deal with each line with k increacing
	string line;
	int index = -1;
	while (getline(inputFileStream,line)) {
		if (line == "")
			break;
		
		index++;
		if (index == 8)
			index = 0;
		
		istringstream iss(line);
		long value;
		iss >> value;
		
		averageVOSize[index] += value;
		
	}
	
	for (int i = 0;i != 8;i++) {
		averageVOSize[i] = averageVOSize[i] / 100;
		cout << averageVOSize[i] << endl;
	}

	return 0;
}