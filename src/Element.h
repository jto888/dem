#ifndef _element_H
#define _element_H

#include "dem.h"

		
class Element {		
	private:	
		int id;
		int opline;
		int operable;
		int active;
		Rcpp::NumericVector failRands;
		int fndx;
		int fwrap;
		Rcpp::NumericVector repairRands;
		int rndx;
		int rwrap;

		int fcase;
		double fp1;
		double fp2;
		double fp3;
		int rcase;
		double rp1;
		double rp2;
		double rp3;
		int seed;

		

	public:	
		Element(const std::vector<double> &model_row, int years);	
		double nextFail();	
		double nextRepair();	
		int getID();	
		int getOplineNum();	
			
		int getOperable();	
		int getActive();	
		void setActive(int);	
		void setOperable(int);
		int numRands();
		int numFails();
		double sumRepairs();

		int getfcase();
		double getfp1();
		double getfp2();
		double getfp3();
		int getrcase();
		double getrp1();
		double getrp2();
		double getrp3();
		int getseed();
		int getfwrap();

		Rcpp::NumericVector getFailRands();
		Rcpp::NumericVector getRepairRands();


};
#endif	
