   // Basicdem.cpp file
 /*
 * Author: David Silkworth
 *         (c) 2020 OpenReliability.org
 */




#include "dem.h"
//#include "Element.h"
//#include "DiscreteEvent.h"
#include "History.h"

 SEXP Basicdem(SEXP model_in, SEXP years_in, SEXP margin_in) {     
	using namespace Rcpp;
	
	Rcpp::List outlist;
	Rcpp::RNGScope Scope;

	
	std::unique_ptr<ElementList> EL(new ElementList(model_in, years_in));
	// EQ is a shared_ptr for storage in History permitting test output of the queue
	std::shared_ptr<EventQueue> EQ(new EventQueue(EL, years_in));
	// note OLL is now a shared_ptr for storage in History
	std::shared_ptr<OpLineList> OLL(new OpLineList(EL));
	std::unique_ptr<History> H(new History(OLL, EQ, margin_in));
//	int Elem0ID = EL->getByIndex(0)->numRands();
//	Rcout<< "Elem0 numRands  "<< Elem0ID << std::endl;	
 // Rcout << EQ->writeQue();
//Rcout << EQ->getEvent(0)->getTime() << "  " << EQ->getEvent(1)->getTime() << "  " << EQ->getEvent(2)->getTime() << std::endl;
	bool endSimulation = false;
	int headspace = 1;
	
// This is the main loop for the simulation eventually will be a while loop
//for(int i=0; i<40; i++) {	
while(!endSimulation) {
	
	std::shared_ptr<DiscreteEvent> ev = EQ->pullNext();
// before moving the pos index, set the last duration	
	H->setLastDuration(ev->getTime());
	
	
	
	switch(ev->getType()) {
		case 0:
			endSimulation = true;
			break;
		case 1:
			OnFailure(ev, EL, EQ, OLL);
			break;
		case 2:
			OnRepair(ev, EL, EQ, OLL);
			break;
	//Clock and/or Stores events to follow
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


