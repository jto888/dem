#include "dem.h"


void OnMaintEnd(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<MaintList>& ML,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL) {


			

	int maintID = ev->getEventID();
	int opline = ev->getOplineNum();
	//double timeNow = ev->getTime();
	
	//set the inMaint==0
	OLL->getByNum(opline)->setInMaint(0);

	//test for any repairs that need to be placed on queue
	// copied over from original code in OnRepair
	
	// if all other elements in this opline are operable, set all to active=1 
			
	int allOthersOperable= 1;			
	for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {			
		///if(OLL->getByNum(opline)->getElems()[i]->getID() != element) {		
			allOthersOperable = allOthersOperable* OLL->getByNum(opline)->getElems()[i]->getOperable();	
		///}		
	}			
	if(allOthersOperable== 1) {			
			//int allOthersActive = 1;		
			for(int i=0; i <  (int) OLL->getByNum(opline)->getElems().size(); i++) {	
				OLL->getByNum(opline)->getElems()[i]->setActive(1);		
			}			
	}			
	else { 	    // since other elements in this opline are failed		
		/// set this element to active=0 (make it passive, it could indeed fail again while still down)		
		///EL->getByID(element)->setActive(0);		
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
	
	
	//insert the nextMaint events in the queue
		// note type 3 is a maint start
		auto new_maint_start = std::make_shared<DiscreteEvent>(ev->getTime()+ML->getByID(maintID)->getInterval(), 3, maintID, opline);
		EQ->insertEvent(new_maint_start);

		// note type 4 is a maint end
		auto new_maint_end = std::make_shared<DiscreteEvent>(ev->getTime()+ML->getByID(maintID)->getInterval()+ML->getByID(maintID)->getDuration(), 4, maintID, opline);
		EQ->insertEvent(new_maint_end);	

	
// direct impact Maintenance handling

	//get direct impacts and process if they exist
	if(!OLL->getByNum(opline)->getDirectImpacts().empty()) {
		std::vector<DirectImpact> dImpacts = OLL->getByNum(opline)->getDirectImpacts();
	//for each direct impact target process the end of maintenance on each
		for(int i=0; i< (int) dImpacts.size(); i++)  {
			int target_num = dImpacts[i].getTarget();
			
		// Must return inMaint to 0	
			OLL->getByNum(target_num)->setInMaint(0);

			// if all other elements in this target are operable, set all to active=1
			int allOthersOperable= 1;			
			for(int i=0; i <  (int) OLL->getByNum(target_num)->getElems().size(); i++) {			
				///if(OLL->getByNum(opline)->getElems()[i]->getID() != element) {		
					allOthersOperable = allOthersOperable* OLL->getByNum(target_num)->getElems()[i]->getOperable();	
				///}		
			}	
			if(allOthersOperable== 1) {			
				//int allOthersActive = 1;		
				for(int i=0; i <  (int) OLL->getByNum(target_num)->getElems().size(); i++) {	
					OLL->getByNum(target_num)->getElems()[i]->setActive(1);		
				}			
			}	
			else { 	    // since other elements in this opline are failed	
	
				// any failed elements in this target opline are now set to active and their repair events are inserted into the que.		
				for(int i=0; i <  (int) OLL->getByNum(target_num)->getElems().size(); i++) {		
					if(OLL->getByNum(target_num)->getElems()[i]->getOperable() == 0) {	
						OLL->getByNum(target_num)->getElems()[i]->setActive(1) ;
						int elem_id = OLL->getByNum(target_num)->getElems()[i]->getID() ;
						int elem_ol= OLL->getByNum(target_num)->getElems()[i]->getOplineNum(); 
						// note type 2 is a repair
						auto new_event = std::make_shared<DiscreteEvent>(ev->getTime()+EL->getByID(elem_id)->nextRepair(), 2, elem_id, elem_ol);
						EQ->insertEvent(new_event);
					}	
				}
			}
		}
	}	// completion of direct impact handling

}	

