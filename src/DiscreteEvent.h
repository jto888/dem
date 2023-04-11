#ifndef _discrete_event_H
#define _discrete_event_H

#include "dem.h"

		
class DiscreteEvent {		
	private:	
		double _time;
		int _type;   //these event types are declared as const in dem.h
		int _eventID;  // This will hold the Element.ID for now, later with clocks it will be ClockID
		int _oplineNum; // Will be set to zero for Clocks and other types, but having this should
						//drop need for the Element List object going forward

	public:	
		DiscreteEvent( double time, int type, int eventID, int oplineNum);
		double getTime();
		int getType();
		int getEventID();
		int getOplineNum();

		// not to be used, these items are only set on construction
		// void setTime(double time);
		// setType(int type);
		//void setEventID(int typeID);

};		


#endif	
