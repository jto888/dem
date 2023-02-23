#ifndef _summary_H
#define _summary_H

#include "dem.h"

class Summary {		
private:
	std::vector<int> OpLine;
	std::vector<int> ElementID;
	std::vector<int> fcase;
	std::vector<double> fp1;
	std::vector<double> fp2;
	std::vector<double> fp3;
	std::vector<int> rcase;
	std::vector<double> rp1;
	std::vector<double> rp2;
	std::vector<double> rp3;
	std::vector<int> seed;
	std::vector<int> num_fails;
	std::vector<double> repair_hrs;
	std::vector<int> rand_size;
	std::vector<int> fwrap;
	std::shared_ptr<OpLineList> _OLL;

public:
	Summary(std::shared_ptr<OpLineList>& OLL);
	Rcpp::List summaryDF();

};

#endif		