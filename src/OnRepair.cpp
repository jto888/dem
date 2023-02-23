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
EL->getByID(element)->setOperable(1);
//// This was a problem. Always must set next fail upon repair			
//if this element is active, insert next fail on the que.			
///if(EL->getByID(element)->getActive() == 1) {	
	// note type 1 is fail		
	auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(element)->nextFail(), 1, element, opline);		
	EQ->insertEvent(new_event);		
///}			
// if all other elements in this opline are operable, set all to active=1 
///(unless in maintenance)			
int allOthersOperable= 1;			
for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {			
	if(OLL->getByNum(opline)->getElems()[i]->getID() != element) {		
		allOthersOperable = allOthersOperable* OLL->getByNum(opline)->getElems()[i]->getOperable();	
	}		
}			
if(allOthersOperable== 1) {	
	/// only set opline elements to active if not in maintenance
	/// for the inMaint == 1 case this action is taken OnMaintEnd
	if(OLL->getByNum(opline)->getMaintStatus() == 0)  {		
		//int allOthersActive = 1;		
		for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {	
			OLL->getByNum(opline)->getElems()[i]->setActive(1);		
		}
	}		
}			
else { 	    // since other elements in this opline are failed		
	// set this element to active=0 (make it passive, it could indeed fail again while still down)		
	EL->getByID(element)->setActive(0);		
 // any failed elements in this opline are now set to active and their repair events are inserted into the que.			
	for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {		
		if(OLL->getByNum(opline)->getElems()[i]->getOperable() == 0) {	
			OLL->getByNum(opline)->getElems()[i]->setActive(1) ;
			int elem_id = OLL->getByNum(opline)->getElems()[i]->getID() ;
			int elem_ol= OLL->getByNum(opline)->getElems()[i]->getOplineNum(); 
			// note type 2 is a repair
			auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(elem_id)->nextRepair(), 2, elem_id, elem_ol);
			EQ->insertEvent(new_event);
		}	
	}		
}			
			
}
