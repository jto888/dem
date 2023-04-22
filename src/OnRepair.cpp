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

// This was a problem. Always must set next fail upon repair			
	// note type 1 is fail		
	//auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(element)->nextFail(), 1, element, opline);		
	auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(element)->nextFail(), FAILURE, element, opline);
	EQ->insertEvent(new_event);	
	
// if all other elements in this opline are operable, set all to active=1 						
///(unless in maintenance)						
if(OLL->getByNum(opline)->getMaintStatus() == 0)  {						
	//if(OLLgetByNum(opline)->activateOrRepair() == 1) {					
	if(activateOrRepair(opline, ev, EL, EQ, OLL) == 1 ) {					
		int num_ddeps = OLL->getByNum(opline)->getDirectDependents().size();				
		if(num_ddeps >0) {				
			for(int i=0; i<num_ddeps; i++) {			
				int dep_opline = OLL->getByNum(opline)->getDirectDependents()[i];		
				if(OLL->getByNum(dep_opline)->getMaintStatus() == 0)  {		
					activateOrRepair(dep_opline, ev, EL, EQ, OLL);	
				}		
			}			
		}				
	}					
}						
	
}	
/*			
// if all other elements in this opline are operable, set all to active=1 
///(unless in maintenance)
if(OLL->getByNum(opline)->getMaintStatus() == 0)  {				
	int allOthersOperable= 1;			
	for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {			
		if(OLL->getByNum(opline)->getElems()[i]->getID() != element) {		
			allOthersOperable = allOthersOperable* OLL->getByNum(opline)->getElems()[i]->getOperable();	
		}		
	}			
	if(allOthersOperable== 1) {			
		for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {	
			OLL->getByNum(opline)->getElems()[i]->setAsActive();		
		}		
	}			
	else { 	    // since at least one other element in this opline is failed		
		// assure this element is dormant, it could indeed fail again while still down)	
			/// this step should be unnecessary, since active-failed state is not utilized.
		EL->getByID(element)->setAsDormant();		
	  // the first element in this opline found to be failed will now have its  repair event is inserted into the que.		
		for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {		
			if(OLL->getByNum(opline)->getElems()[i]->getOperable() == 0) {	
				//OLL->getByNum(opline)->getElems()[i]->setActive(1) ;
				int elem_id = OLL->getByNum(opline)->getElems()[i]->getID() ;
				int elem_ol= OLL->getByNum(opline)->getElems()[i]->getOplineNum(); 
				// note type 2 is a repair
				//auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(elem_id)->nextRepair(), 2, elem_id, elem_ol);
				auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(elem_id)->nextRepair(), REPAIR, elem_id, elem_ol);
				EQ->insertEvent(new_event);
				break;   ///this break assures only one repair at a time
			}	
		}		
	}			
}			
}
*/

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
	  // the first element in this opline found to be failed will now have its  repair event is inserted into the que.						
		for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {					
			if(OLL->getByNum(opline)->getElems()[i]->getOperable() == 0) {				
				//OLL->getByNum(opline)->getElems()[i]->setActive(1) ;			
				int elem_id = OLL->getByNum(opline)->getElems()[i]->getID() ;			
				int elem_ol= OLL->getByNum(opline)->getElems()[i]->getOplineNum(); 			
				auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(elem_id)->nextRepair(), REPAIR, elem_id, elem_ol);			
				EQ->insertEvent(new_event);			
				break;   ///this break assures only one repair at a time			
			}				
		}					
	}						
							
	return allOthersOperable;						
}							
