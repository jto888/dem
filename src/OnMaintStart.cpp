#include "dem.h"


void OnMaintStart(std::shared_ptr<DiscreteEvent>& ev,
		//std::unique_ptr<ElementList>& EL,
		//std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL) {


// in-line Maintenance handling			

	//int element = ev->getEventID();
	int opline = ev->getOplineNum();
	
	//set all elements in opline to active=0
	for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {
		OLL->getByNum(opline)->getElems()[i]->setActive(0);		
	}	
	//set the inMaint ==1 (so that OnRepair can avoid activation of  the opline)
	OLL->getByNum(opline)->setInMaint(1);

// direct impact Maintenance handling

	//get direct impacts and process if they exist
	if(!OLL->getByNum(opline)->getDirectImpacts().empty()) {			
		std::vector<DirectImpact> dImpacts = OLL->getByNum(opline)->getDirectImpacts();
	//for each direct impact target set all elements to active=0 and set the inMaint				
		for(int i=0; i< (int) dImpacts.size(); i++)  {		
			int target_num = dImpacts[i].getTarget();	
		// Note probability has no effect on maintenance impact			
			//set all elements in opline to active=0 (i.e. passivate)	
			for(int i=0; i <  (int) OLL->getByNum(target_num)->getElems().size(); i++) {	
				OLL->getByNum(opline)->getElems()[i]->setActive(0);
			}	
			//set the inMaint ==1 (so that OnRepair can avoid activation of  the opline)	
			OLL->getByNum(target_num)->setInMaint(1);	
		}	
	}			
			



		
}
