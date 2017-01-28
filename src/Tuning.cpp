#include <algorithm>    
#include <vector>      
#include "Tuning.h"
#include <math.h>

double getChroma(vector<double> scale) {
	double min = scale[0];
	double max = scale[0];
	for(int i=1; i<scale.size(); i++) {
		double diff=scale[i]-scale[i-1];
		if(diff>max) max = diff;
		if(diff<min) min = diff;
	}
	return max-min;
}

Tuning generateMosScale(double gen1Size, double gen2Size, int noteNumber){
	double base = 0;
	double interval = fmod(gen2Size, gen1Size);
	std::vector<double> output(noteNumber);
	for (int i = 0; i< noteNumber; i++){
		output.push_back(fmod(base + (i*interval), gen1Size));
	}
	std::sort (output.begin(), output.end());

	return (Tuning) {
		.gen1Size = gen1Size,
		.gen2Size = gen2Size,
		.scale = output,
		.chroma = getChroma(output)
	};
}

