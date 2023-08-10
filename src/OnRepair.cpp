#include "dem.h"


void OnRepair(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL) {
			
// For the basic opline model inline handling will suffice:
handleInlineRepair(ev, EL, EQ, OLL);

// ToDo: Any special/external actions to take on repair event		
			
			
			
			
			
			
			
}

void handleInlineRepair(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL) {
			
// InlineRepairActions code			
int element = ev->getEventID();			
int opline = ev->getOplineNum();
// set this element to operable
//EL->getByID(element)->setOperable(1);
EL->getByID(element)->setAsOperable();
EL->getByID(element)->setRepairOnQueue(0);

// This was a problem. Always must set next fail upon repair			
	// note type 1 is fail		
	//auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(element)->nextFail(), 1, element, opline);		
	auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(element)->nextFail(), FAILURE, element, opline);
	EQ->insertEvent(new_event);	
	
	// if all other elements in this opline are operable, set all to active=1 						
	///(unless in maintenance)						
	if(OLL->getByNum(opline)->getInMaint() == 0)  {						
		//if(OLLgetByNum(opline)->activateOrRepair() == 1) {					
		if(activateOrRepair(opline, ev, EL, EQ, OLL) == 1 ) {					
			int num_ddeps = OLL->getByNum(opline)->getDirectDependents().size();				
			if(num_ddeps >0) {				
				for(int i=0; i<num_ddeps; i++) {			
					int dep_opline = OLL->getByNum(opline)->getDirectDependents()[i];		
					if(OLL->getByNum(dep_opline)->getInMaint() == 0)  {		
						activateOrRepair(dep_opline, ev, EL, EQ, OLL);	
					}		
				}			
			}				
		}
		// This repair may enable a co-enabled dependency			
		if(OLL->getByNum(opline)->getCoEnabledDependents().size() > 0) {			
			// loop through target OpLines and activate or repair, if dormant		
			for(int i=0; i < (int) OLL->getByNum(opline)->getCoEnabledDependents().size(); i++) {		
				Rcpp::IntegerVector codeps = OLL->getByNum(opline)->getCoEnabledDependents()[i];	
				int codeps_len = (int) codeps.size();	
				int target = codeps[(codeps_len -1)];	
				if(OLL->getByNum(target)->getInMaint() == 0) {
				// I really did not want to repair an element that already had repair in queue
				// but this test does not do that
				//if(OLL->getByNum(target)->getStatus() == 0) {	
					activateOrRepair(target, ev, EL, EQ, OLL);
				//}
				}
			}		
		}
	}	
}	


int activateOrRepair(int opline, 
	std::shared_ptr<DiscreteEvent>& ev,						
	std::unique_ptr<ElementList>& EL,						
	std::shared_ptr<EventQueue>& EQ,						
	std::shared_ptr<OpLineList>& OLL) {						
							
	int allOthersOperable= 1;						
 	for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {						
		//if(OLL->getByNum(opline)->getElems()[i]->getID() != element) {					
			allOthersOperable = allOthersOperable* OLL->getByNum(opline)->getElems()[i]->getOperable();				
		//}					
	}						
	if(allOthersOperable== 1) {						
		for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {					
			OLL->getByNum(opline)->getElems()[i]->setAsActive();				
		}					
	}						
	else { 	    // since at least one other element in this opline is failed
	// All elements in this opline found to be failed will now have thier repair event inserted into the queue if needed.	
		for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {					
			if(OLL->getByNum(opline)->getElems()[i]->getOperable() == 0) {				
				//OLL->getByNum(opline)->getElems()[i]->setActive(1) ;			
				int elem_id = OLL->getByNum(opline)->getElems()[i]->getID() ;			
				int elem_ol= OLL->getByNum(opline)->getElems()[i]->getOplineNum(); 
				if(OLL->getByNum(opline)->getElems()[i]->getRepairOnQueue() == 0) {				
					auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(elem_id)->nextRepair(), REPAIR, elem_id, elem_ol);			
					EQ->insertEvent(new_event);	
					OLL->getByNum(opline)->getElems()[i]->setRepairOnQueue(1);					
					//  break;   ///this break attempted to assure only one repair at a time
				}	
			}				
		}					
	}						
							
	return allOthersOperable;						
}							
