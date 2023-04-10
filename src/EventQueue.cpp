#include "dem.h"
#include "DiscreteEvent.h"
#include "EventQueue.h"
#include <math.h>
//#include "Maintenance.h"
//#include "MaintList.h"

		using namespace std; 

EventQueue::EventQueue(std::unique_ptr<ElementList>& EL, SEXP years_in) {
					
						
	simhours=Rcpp::as<double>(years_in)*365.0*24.0;	
	eventQue.push_back(std::make_shared<DiscreteEvent>(simhours, 0, 0,0));				
	for(int i=0; i<EL->getSize(); i++)  {				
		//std::shared_ptr<Element> elem = EL->getByIndex(i) ;	
		shared_ptr<Element> elem = EL->getByIndex(i) ;			
	// construct a shared pointer to a DiscreteEvent that might not be used				
				// note that event type 1 is an 'lruFail' event				
		//std::shared_ptr<DiscreteEvent> ev = std::make_shared<DiscreteEvent>(elem->nextFail(), 1, elem->getID());	
		auto ev = make_shared<DiscreteEvent>(elem->nextFail(), 1, elem->getID(), elem->getOplineNum());
	
			// yes, this is duplicated code from insertEvent, but necessary				
			// since eventQue is a std::vector not an EventQue object	
		if (ev->getTime()< simhours) {
			 for (auto it = eventQue.begin(); it != eventQue.end(); it++) { 
				if( (*it)->getTime() > ev->getTime() )  {
					 eventQue.insert(it,ev);
					 break;
				}
			}	
		}

		
	}
}

/*
EventQueue::EventQueue(std::unique_ptr<ElementList>& EL, 
					   std::unique_ptr<MaintList>& ML, 
					   SEXP years_in) {
						
	simhours=Rcpp::as<double>(years_in)*365.0*24.0;	
	eventQue.push_back(std::make_shared<DiscreteEvent>(simhours, 0, 0,0));				
	for(int i=0; i<EL->getSize(); i++)  {				
		//std::shared_ptr<Element> elem = EL->getByIndex(i) ;	
		shared_ptr<Element> elem = EL->getByIndex(i) ;			
	// construct a shared pointer to a DiscreteEvent that might not be used				
				// note that event type 1 is an 'lruFail' event				
		//std::shared_ptr<DiscreteEvent> ev = std::make_shared<DiscreteEvent>(elem->nextFail(), 1, elem->getID());	
		auto ev = make_shared<DiscreteEvent>(elem->nextFail(), 1, elem->getID(), elem->getOplineNum());
	
			// yes, this is duplicated code from insertEvent, but necessary				
			// since eventQue is a std::vector not an EventQue object	
		if (ev->getTime()< simhours) {
			 for (auto it = eventQue.begin(); it != eventQue.end(); it++) { 
				if( (*it)->getTime() > ev->getTime() )  {
					 eventQue.insert(it,ev);
					 break;
				}
			}	
		}
	}

	for(int i=0; i<ML->getSize(); i++)  {		
		shared_ptr<Maintenance> maint = ML->getByIndex(i) ;	
		double maint_start = maint->getFirstInterval();	

// I now feel that when I specified a first interval of zero I meant it.
//		if(maint_start == 0.0) {
//			maint_start = maint->getInterval();
//		}
	

	
		double maint_end = maint_start + maint->getDuration();
		auto ev1 = make_shared<DiscreteEvent>(maint_start, 3, maint->getNum(), maint->getOplineNum());
		auto ev2 = make_shared<DiscreteEvent>(maint_end, 4, maint->getNum(), maint->getOplineNum());
		
		if (ev1->getTime()< simhours) {
			 for (auto it = eventQue.begin(); it != eventQue.end(); it++) { 
				if( (*it)->getTime() > ev1->getTime() )  {
					 eventQue.insert(it,ev1);
					 break;
				}
			}	
		}
		if (ev2->getTime()< simhours) {
			 for (auto it = eventQue.begin(); it != eventQue.end(); it++) { 
				if( (*it)->getTime() > ev2->getTime() )  {
					 eventQue.insert(it,ev2);
					 break;
				}
			}	
		}		
	}
	
}

*/


void EventQueue::insertEvent(shared_ptr<DiscreteEvent>& ev) {
		if (ev->getTime()< simhours) {	
		
			for(int j=0; j< (int) eventQue.size(); j++) {		
				if(eventQue[j]->getTime()>ev->getTime()) {	
					eventQue.insert(eventQue.begin()+j, ev);
					break;
				}	
			}
/*
			 for (auto it = eventQue.begin(); it != eventQue.end(); it++) { 
				if( (*it)->getTime() > ev->getTime() )  {
					 eventQue.insert(it,ev);
					 break;
				}
			}
*/					
		}	

}

	int EventQueue::getSize() {
		return eventQue.size();
	}
	
	shared_ptr<DiscreteEvent> EventQueue::pullNext() {
		shared_ptr<DiscreteEvent> ev = eventQue[0];
		eventQue.erase(eventQue.begin());
		return ev;	
	}
	
	// used for testing only
	std::shared_ptr<DiscreteEvent> EventQueue::getEvent(int i) {
		return eventQue[i];
	}
	
	std::string EventQueue::writeQue() {
		std::string outstring = "";
		for(int i=0; i < (int) eventQue.size(); i++)  {
		std::string type_char = "X";
			switch(eventQue[i]->getType()) {
				case 0:
					type_char = "X";
					break;
				case 1:
					type_char = "F";
					break;
				case 2:
					type_char = "R";
					break;
			//Clock and/or Stores events to follow
				default:
					type_char = "X";
					break;
			}

			int trunc_time = (int) floor(eventQue[i]->getTime()); 
			outstring = outstring + "[" +to_string(eventQue[i]->getEventID())+type_char+"]"+to_string(trunc_time) + " ";
		}
		return outstring;
	}
	
	
	
	
