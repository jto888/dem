#ifndef _opline_H
#define _opline_H

#include "dem.h"
#include "EventQueue.h"

class OpLine {		
	private:	
		int num;
		int inMaint;
		std::shared_ptr<EventQueue> EQ;
		std::vector<std::shared_ptr<class Element>> elems;
		std::vector<int> direct_dependents;
		std::vector<Rcpp::IntegerVector>  co_enabled_dependents;



	public:	
		OpLine(std::unique_ptr<ElementList> &_elem, int  _num);
		int getStatus();
		std::vector<std::shared_ptr<class Element>> getElems();
		int getNum();
		int getMaintStatus();
		void setInMaint(int);
		std::vector<int> getDirectDependents();
		std::vector<Rcpp::IntegerVector> getCoEnabledDependents();
		//Rcpp::IntegerVector getCoEnabledDependentRow(int row_num);
		void addDirectDependent(int dep);
		void addCoEnabledDependent(Rcpp::IntegerVector co_enabled_row);
		//bool DirectDependentsEmpty();
		//bool CoEnabledDependentsEmpty();





		//std::vector<DirectImpact> getDirectImpacts();
};

#endif	
