   // DEMwMaint.cpp file
 /*
 * Author: David Silkworth
 *         (c) 2020-2023 OpenReliability.org
 */




#include "dem.h"
//#include "Element.h"
//#include "DiscreteEvent.h"
//#include "History.h"

 SEXP DEMwMaintwDep(SEXP model_in, SEXP years_in, SEXP margin_in, SEXP show_zeros, SEXP maint_in, SEXP dep_in) {     
	using namespace Rcpp;
	
	Rcpp::List outlist;
	Rcpp::RNGScope Scope;

	
	std::unique_ptr<ElementList> EL(new ElementList(model_in, years_in));
	// EQ is a shared_ptr for storage in History permitting test output of the queue
	std::shared_ptr<EventQueue> EQ(new EventQueue(EL, years_in, maint_in));
	
	//Rcout << EQ->getEvent(0)->getTime() << "  " << EQ->getEvent(1)->getTime() << "  " << EQ->getEvent(2)->getTime() << std::endl;
 	
	// note OLL is now a shared_ptr for storage in History
	std::shared_ptr<OpLineList> OLL(new OpLineList(EL, dep_in));
	std::unique_ptr<History> H(new History(OLL, EQ, margin_in, show_zeros));

	bool endSimulation = false;
	int headspace = 1;
// This is the main loop for the simulation eventually will be a while loop
//for(int i=0; i<40; i++) {	
while(!endSimulation) {
	
	std::shared_ptr<DiscreteEvent> ev = EQ->pullNext();
// before moving the pos index, set the last duration	
	H->setLastDuration(ev->getTime());
	
	
	
	switch(ev->getType()) {			
		case END_SIM:		
			endSimulation = true;	
			break;	
		case FAILURE:		
			OnFailure(ev, EL, EQ, OLL);	
			break;	
		case REPAIR:		
			OnRepair(ev, EL, EQ, OLL);	
			break;	
		case MAINT_START:
	//Rcpp::Rcout << ev->getTime() <<"    opline  "<< ev->getOplineNum()<< "   maint_start" << std::endl;
			OnMaintStart(ev, OLL);
			break;
		case MAINT_END:
	//Rcpp::Rcout << ev->getTime() <<"    opline  "<< ev->getOplineNum()<< "   maint_end" << std::endl;
			OnMaintEnd(ev, EL, EQ, OLL);
			break;			
		default:		
			endSimulation = true;	
			break;	
	}			


	

	headspace = H->update(ev);
	if(headspace==0)  {
		endSimulation = true;
	}
		
}	

	
	Rcpp::List outframe=H->historyList();
	//add headspace as an attribute to output
	outframe.attr("headspace") = headspace;
	Rcout<< "headspace    "<< headspace << std::endl;	
	std::unique_ptr<Summary> S(new Summary(OLL));
	outlist.push_back(S->summaryDF());
	outlist.push_back(outframe);
	outlist.push_back(Rcpp::wrap(H->get__detail()));


	 return(outlist);
}