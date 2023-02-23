#ifndef _maintlist_H
#define _maintlist_H

#include "dem.h"
//#include "Element.h"
#include "Maintenance.h"

class MaintList {		
	private:	
		std::vector<std::shared_ptr<class Maintenance>> _maintList;
		
		
		
	public:	
		MaintList(SEXP maint_in);
		int getSize();
		std::shared_ptr<Maintenance> getByIndex(int index);
		std::shared_ptr<Maintenance> getByID(int id);
		
};		

#endif
