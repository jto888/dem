   // DEMwMaint.cpp file
 /*
 * Author: David Silkworth
 *         (c) 2020-2023 OpenReliability.org
 */




#include "dem.h"
//#include "Element.h"
//#include "DiscreteEvent.h"
//#include "History.h"

 SEXP DEMwMaint(SEXP model_in, SEXP years_in, SEXP margin_in, SEXP show_zeros, SEXP maint_in) {     
	using namespace Rcpp;
	
	Rcpp::List outlist;
	Rcpp::RNGScope Scope;

	
	std::unique_ptr<ElementList> EL(new ElementList(model_in, years_in));
	// EQ is a shared_ptr for storage in History permitting test output of the queue
	std::shared_ptr<EventQueue> EQ(new EventQueue(EL, years_in, maint_in));
	
	
		// note OLL is now a shared_ptr for storage in History
	//	std::shared_ptr<OpLineList> OLL(new OpLineList(EL));
	//	std::unique_ptr<History> H(new History(OLL, EQ, margin_in, show_zeros));
	//	int Elem0ID = EL->getByIndex(0)->numRands();
	//	Rcout<< "Elem0 numRands  "<< Elem0ID << std::endl;	
	 return Rcpp::wrap(EQ->writeQue());
	//Rcout << EQ->getEvent(0)->getTime() << "  " << EQ->getEvent(1)->getTime() << "  " << EQ->getEvent(2)->getTime() << std::endl;
 }