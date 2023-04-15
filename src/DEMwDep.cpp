   // DEMwEep.cpp file
 /*
 * Author: David Silkworth
 *         (c) 2023 OpenReliability.org
 */
 
 
 
 
 #include "dem.h"			
//#include "Element.h"			
//#include "DiscreteEvent.h"			
//#include "History.h"			
			
 SEXP DEMwDep(SEXP model_in, SEXP years_in, SEXP margin_in, SEXP show_zeros, SEXP dep_in) {     			
	using namespace Rcpp;		
			
	Rcpp::List outlist;		
	Rcpp::RNGScope Scope;		
			
			
	std::unique_ptr<ElementList> EL(new ElementList(model_in, years_in));		
			
			
	// note OLL is now a shared_ptr for storage in History		
	std::shared_ptr<OpLineList> OLL(new OpLineList(EL, dep_in));		
			
			
	return OLL->getDepRow(10);		
			
			
}			
			
