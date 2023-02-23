#ifndef _oplinelist_H
#define _oplinelist_H

#include "dem.h"
//#include "Element.h"
#include "OpLine.h"

class OpLineList {		
	private:	
		std::vector<std::shared_ptr<class OpLine>> _oplines;
		
		
		
	public:	
		OpLineList(std::unique_ptr<ElementList>& EL);
		int getSize();
		std::shared_ptr<OpLine> getByIndex(int index);
		std::shared_ptr<OpLine>  getByNum(int num);
		
};		

#endif
