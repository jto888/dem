
#include "Element.h"
#include <math.h>
	
Element::Element(const std::vector<double> &model_row, int num_years)  {								
	opline =  (int) model_row[0];			
	id = (int) model_row[1];			
	operable = 1;			
	active = 1;			
	fndx = 0;
	fwrap = 0;
	rndx = 0;
	rwrap = 0;
	
		fp1 = model_row[3];
		fp2 = model_row[4];
		fp3 = model_row[5];

		rp1 = model_row[7];
		rp2 = model_row[8];
		rp3 = model_row[9];
		seed = model_row[10];
		
// num_rands is calculated similar to method used in SimulationHistory.cpp				
// double num = (8760/(FP1_Vec[x]+FP3_Vec[x])+0.5)*(1.0+0.25/ModelSize)*SimulationYears[0];				
// However, ModelSize was number of elements in an opline (this is hard to get in dem code)	
// oplsz<-sapply(Model$OpLine, function (X)  length(Model$OpLine[Model$OpLine==X]))
// can add a 12th column to the model in the scaffold code.	
// Worst case this may lead to random values wrapping for single element oplines.
	int oplsz = model_row[11];
	
	double param = (1.0+0.25/oplsz)*num_years*(0.5+8760/(fp1+fp3));
	int num_rands = (int) std::ceil(param);
		
	//double fp3 = model_row[5];
	//double rp3 = model_row[9];
				
	Rcpp::Environment base("package:base");			
	Rcpp::Function SetSeed = base["set.seed"];			
	SetSeed(Rcpp::Named("seed", seed ));			
				
	switch((int)model_row[2])  {			
		case 1:   		
		failRands= Rcpp::rexp(num_rands,1/fp1);
		fcase = 1;
		break; 		
				
		case 2:   		
		failRands= Rcpp::rnorm(num_rands,fp1,fp2);
		fcase = 2;
		break; 		
				
		case 3:  		
		failRands=Rcpp::rweibull(num_rands,fp2,fp1);
		fcase = 3;
		break; 		
				
		default:
		fcase = 0;
		failRands= Rcpp::rexp(num_rands,1/fp1);		
	}

// this is my first lambda function. Note how the 'this' had to be captured as [&]	
	if(fp3 > 0) {			
		std::for_each(failRands.begin(),failRands.end(), [&](double& d) { d+=fp3;});		
	}			
				
	switch((int) model_row[6])  {			
		case 2: 		
		repairRands = Rcpp::rnorm(num_rands,rp1,rp2);
		rcase = 2;
		break;		
		case 3:		
		repairRands = Rcpp::rweibull(num_rands,rp2,rp1);
		rcase = 3;
		break;		
		case 4: 		
		repairRands = Rcpp::rlnorm(num_rands,rp1,rp2);
		rcase = 4;
		break;		
		default:		
		repairRands = Rcpp::rexp(num_rands,1/rp1);
		rcase = 0;
		break; 		
	}
// 	Note how the this had to be captured as [&]
// Before making rp3 a class member it was captured as [rp3]
	if(rp3 > 0) {			
		std::for_each(repairRands.begin(),repairRands.end(), [&](double& d) { d+=rp3;});		
	}			
				
}				
		
double Element::nextFail() {		
	double randval=failRands[fndx];	
	fndx++;	
	if(fndx==failRands.size()) {
		fndx=0;
		fwrap++;
	}
	return randval;	
}

double Element::nextRepair() {		
	double randval=repairRands[rndx];	
	rndx++;	
	if(rndx==repairRands.size()) {
		rndx=0;
		rwrap++;
	}
	return randval;	
}

int Element::getID() {
	return id;
}

int Element::getOplineNum() {
	return opline;
}

int Element::getOperable() {
	return operable;
}

int Element::getActive() {
	return active;
}

void Element::setActive(int activeState) {
	active = activeState;
}

void Element::setAsActive()  {
	active = 1;
}

void Element::setAsDormant() {
	active = 0;
|	
	
void Element::setOperable(int operableState) {
	operable = operableState;
}
 
void Element::setAsOperable() {
	operable = 1;
}

void Element::setAsFailed() {
	operable = 0;
}

int Element::numRands() {
	return failRands.size();
}

int Element::numFails() {
	//note fndx starts at 1, when there have been no fails
	int num_fails = fndx-1;
	//after a wrap fndx is 0, so the num fails would start at -1 after a wrap
	if(fwrap >0) num_fails = num_fails + fwrap * failRands.size() + 1;
	return num_fails;	
}

double Element::sumRepairs() {
	// this seems to work for case where rndx = 0, without special code.
	double sum_repairs = std::accumulate(repairRands.begin(), repairRands.begin() + rndx, 0.0);
	if(rwrap > 0) sum_repairs = sum_repairs + rwrap * std::accumulate(repairRands.begin(), repairRands.end(), 0.0);
	return sum_repairs;
}

int Element::getfcase() {
	return fcase;
}

double Element::getfp1() {
	return fp1;
}

double Element::getfp2() {
	return fp2;
}

double Element::getfp3() {
	return fp3;
}

int Element::getrcase() {
	return rcase;
}

double Element::getrp1() {
	return rp1;
}

double Element::getrp2() {
	return rp2;
}

double Element::getrp3() {
	return rp3;
}

int Element::getseed() {
	return seed;
}

int Element::getfwrap() {
	return fwrap;
}	

Rcpp::NumericVector Element::getFailRands() {
	return failRands;
}

Rcpp::NumericVector Element::getRepairRands() {
	return repairRands;
}