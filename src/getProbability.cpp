#include "dem.h"

double getProbability() {	
	return Rcpp::runif(1)[0] * 100;
}	
