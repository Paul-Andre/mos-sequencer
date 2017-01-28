#include <string>
#include <iostream>     
#include <algorithm>    
#include <vector>      
#include <iomanip>
#include "Tuning.h"
#include <math.h>


vector<double> generateMosScale(Tuning const &input){
	double base = 0;
	double interval = fmod(input.gen1Size,input.gen2Size);
	std::vector<double> output(input.noteNumber);
	for (int i = 0; i< input.noteNumber; i++){
		output.push_back(fmod(base + (i*interval), input.gen2Size));
	}
	std::sort (output.begin(), output.end());     
	return output;
}

