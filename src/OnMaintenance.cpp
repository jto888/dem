#include "dem.h"					
					
					
void OnMaintStart(std::shared_ptr<DiscreteEvent>& ev,					
		std::shared_ptr<OpLineList>& OLL) {			
					
					
// in-line Maintenance handling					
					
	//int element = ev->getEventID();				
	int opline = ev->getOplineNum();				
					
	//set all elements in opline to active=0				
	for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {				
		OLL->getByNum(opline)->getElems()[i]->setAsDormant();			
	}				
	//set the inMaint >0 (so that OnRepair can avoid activation of  the opline)				
	OLL->getByNum(opline)->incrementInMaint();				
					
// direct impact Maintenance handling					
					
	//get direct dependents and process if they exist				
	if(OLL->getByNum(opline)->getDirectDependents().size() > 0) {				
		std::vector<int> deps = OLL->getByNum(opline)->getDirectDependents();			
	//for each direct dependent set all elements as dormant				
		for(int j=0; j < (int) deps.size(); j++) {			
					
			//set all elements in opline to active=0 (i.e. passivate)		
			for(int i=0; i <  (int) OLL->getByNum(deps[j])->getElems().size(); i++) {		
				OLL->getByNum(deps[j])->getElems()[i]->setAsDormant();	
			}		
			//increment the inMaint >0 (so that OnRepair can avoid activation of  the opline)		
			OLL->getByNum(deps[j])->incrementInMaint();		
		}			
	}				
					
}

void OnMaintEnd(std::shared_ptr<DiscreteEvent>& ev,				
		std::unique_ptr<ElementList>& EL,			
		std::shared_ptr<EventQueue>& EQ,			
		std::shared_ptr<OpLineList>& OLL) {			
					
					
					
	// eventID would be zero for maintenance and has not bearing here				
	//int maintID = ev->getEventID();				
	int opline = ev->getOplineNum();				
					
	//decriment maint counter on opline				
	OLL->getByNum(opline)->decrementInMaint();				
	int enablerOperating = activateOrRepair(opline, ev, EL, EQ, OLL);				
					
					
	// direct dependent Maintenance handling				
	if(OLL->getByNum(opline)->getDirectDependents().size() > 0) {				
		std::vector<int> deps = OLL->getByNum(opline)->getDirectDependents();			
		for(int j=0; j < (int) deps.size(); j++) {			
			//decrement the inMaint -1 		
			OLL->getByNum(deps[j])->decrementInMaint();		
			if(OLL->getByNum(deps[j])->getInMaint() < 1) {		
			if(enablerOperating == 1) {		
				activateOrRepair(deps[j], ev, EL, EQ, OLL);	
				// this is location for recursive function call	
			}}		
		}			
	}				
					
}					
					
