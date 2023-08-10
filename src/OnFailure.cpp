#include "dem.h"


void OnFailure(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL) {
			
	if(ev->getEventID() == 9992)  {	
		//Rcpp::Rcout<< "probability    "<< getProbability()<< std::endl;
		if(getProbability() > 1.0) {
			handleInlineFailure(ev, EL, EQ, OLL);
		}else{
			//fail the current element as located in EL (operable=0)	
			//EL->getByID(9992)->setOperable(0);
			EL->getByID(9992)->setAsFailed();
		}
	}else{	
						
// For the basic opline model inline handling will suffice:
	handleInlineFailure(ev, EL, EQ, OLL);
	}
// ToDo: Any special/external actions to take on failure event		
			
	if(ev->getEventID() == 9991)  {	
		if(EL->getByID(9992)->getOperable() == 0) {	
			//auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(9992)->nextRepair(), 2, 9992, 2);
			auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(9992)->nextRepair(), REPAIR, 9992, 2);
			EQ->insertEvent(new_event);
		}					
	}
}

void handleInlineFailure(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL) {
			
//InlineFailActions code
	int element = ev->getEventID();
	int opline = ev->getOplineNum();
	//fail the current element as located in EL (operable=0)	
	//EL->getByID(element)->setOperable(0);
	EL->getByID(element)->setAsFailed();
	//If this element is active, insert its next repair on the que.
	if(EL->getByID(element)->getActive() == 1) {
		// note type 2 is a repair
		//auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(element)->nextRepair(), 2, element, opline);
		auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(element)->nextRepair(), REPAIR, element, opline);
		EQ->insertEvent(new_event);
		EL->getByID(element)->setRepairOnQueue(1);
	}
	//set all elements in this OpLine as Dormant			
	for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {			
		OLL->getByNum(opline)->getElems()[i]->setAsDormant();		
	}
				
	// test for dependencies and set elements in those oplines to dormant			
	if(OLL->getByNum(opline)->getDirectDependents().size() > 0) {
		
		std::vector<int> deps = OLL->getByNum(opline)->getDirectDependents();		
		for(int j=0; j < (int) deps.size(); j++) {

			//deps[j] is the dependent opline 	
			for(int i=0; i <  (int) OLL->getByNum(deps[j])->getElems().size(); i++) {	
				OLL->getByNum(deps[j])->getElems()[i]->setAsDormant();
			//this is the place to call recursion					
			}	
		}		
	}			
	// test for co enabled dependents and set to dormancy depending on the co enabler status				
	if(OLL->getByNum(opline)->getCoEnabledDependents().size() > 0) {				
		for(int i=0; i < (int) OLL->getByNum(opline)->getCoEnabledDependents().size(); i++) {			
			Rcpp::IntegerVector codeps = OLL->getByNum(opline)->getCoEnabledDependents()[i];		
			int codeps_up = 0;		
			int codeps_len = (int) codeps.size();		
			for(int j=0; j<(codeps_len -1) ; j++) {		
				if(codeps[j] == 0) break;	
				codeps_up = codeps_up + OLL->getByNum(codeps[j])->getStatus();	
			}		
			if(codeps_up == 0) {		
				int target = codeps[(codeps_len -1)];	
				for(int k=0; k <  (int) OLL->getByNum(target)->getElems().size(); k++) {	
					OLL->getByNum(target)->getElems()[k]->setAsDormant();
			// At this point there could be a recursive call for any dependencies the target opline					
				}	

			}		
		}			
	}				

}


