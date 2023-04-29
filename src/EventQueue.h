#ifndef _event_queue_H
#define _event_queue_H

#include "dem.h"
#include <string>
//#include "OpLineList.h"
//#include "Maintenance.h"
//#include "MaintList.h"

class EventQueue {		
	private:	
		//    private final LinkedList<Event> eventQue;
		std::vector<std::shared_ptr<class DiscreteEvent>> eventQue;
		//double years;
		double simhours;
		
		// members required for survival inside maintenance reader loops		
		int opline;						
		double interval;						
		double duration;						
		double first_interval;						
		double maint_start;						
		double maint_end;
		
	public:	
		  EventQueue(std::unique_ptr<ElementList>& EL, SEXP years_in);
		  EventQueue(std::unique_ptr<ElementList>& EL, SEXP years_in, SEXP maint_in);

		
		void insertEvent(std::shared_ptr<DiscreteEvent>& ev);
		std::shared_ptr<DiscreteEvent> pullNext();
		void setLastDuration(std::unique_ptr<class History>& H, double time_now);
		int update(std::shared_ptr<DiscreteEvent>& ev, std::unique_ptr<OpLineList>& OLL);
		int getSize();
		// used for testing only
		std::shared_ptr<DiscreteEvent> getEvent(int i);
		//   public Event getLast();
		//   public void sort();
		std::string writeQue();
};		


#endif	
