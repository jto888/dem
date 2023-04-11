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
	}
	//set all elements in this OpLine as Dormant (active=0)
	for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {
		//if(OLL->getByNum(opline)->getElems()[i]->getID() != element) {
			//OLL->getByNum(opline)->getElems()[i]->setActive(0);
			OLL->getByNum(opline)->getElems()[i]->setAsDormant();
		//}
	}
}
