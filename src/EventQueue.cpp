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
		auto ev = make_shared<DiscreteEvent>(elem->nextFail(), FAILURE, elem->getID(), elem->getOplineNum());
	
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

EventQueue::EventQueue(std::unique_ptr<ElementList>& EL, SEXP years_in, SEXP maint_in) {
					
						
	simhours=Rcpp::as<double>(years_in)*365.0*24.0;	
	eventQue.push_back(std::make_shared<DiscreteEvent>(simhours, 0, 0,0));				
	for(int i=0; i<EL->getSize(); i++)  {				
		//std::shared_ptr<Element> elem = EL->getByIndex(i) ;	
		shared_ptr<Element> elem = EL->getByIndex(i) ;			
	// construct a shared pointer to a DiscreteEvent that might not be used				
				// note that event type 1 is an 'lruFail' event				
		//std::shared_ptr<DiscreteEvent> ev = std::make_shared<DiscreteEvent>(elem->nextFail(), 1, elem->getID());	
		auto ev = make_shared<DiscreteEvent>(elem->nextFail(), FAILURE, elem->getID(), elem->getOplineNum());
	
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
	
	// Read all maintenance events and place them onto the queue							
	Rcpp::IntegerVector maint_v = maint_in;							
	int df_cols = maint_v[0];
	maint_v.erase(maint_v.begin());
	int df_rows = (int) maint_v.size()/df_cols;	
	
	for(int i=0; i<df_rows; i++) {							
		Rcpp::IntegerVector maint_row(df_cols);						
		for(int j=0; j<df_cols; j++) {						
			maint_row[j] = maint_v[i + j*df_rows];					
		}						
	
						
		opline =   maint_row[0];						
		interval =  (double) maint_row[1];						
		duration =   (double) maint_row[2];						
		first_interval = (double) maint_row[3];						
		maint_start = 0.0;						
		maint_end = 0.0;						
								
	
								
								
								
		// this do loop inserts all maintenance events on the queue.						
		do {	
//for(int k=0; k<4; k++) {
	
			if(maint_end < 1.0) {					
				maint_start = first_interval;				
			}else{					
				maint_start = maint_end +  interval;				
			}					//
			maint_end = maint_start +  duration;	
			
	Rcpp::Rcout <<"opline  "<< opline << "    maint_start  " << maint_start << "  " << "maint_end  " <<  maint_end << std::endl;
								
/*								
								
			//DiscreteEvent( double time, int type, int eventID, int oplineNum);					
			auto ev1 = make_shared<DiscreteEvent>(maint_start, MAINT_START, 0, opline);					
			auto ev2 = make_shared<DiscreteEvent>(maint_end, MAINT_END, 0, opline);					
								
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
*/									
		}
		while(maint_end < simhours);					
								
	}  // get the next opline for maintenance							
	
}


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
				case END_SIM:
					type_char = "X";
					break;
				case FAILURE:
					type_char = "F";
					break;
				case REPAIR:
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
	
	
	
	
