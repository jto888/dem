#include "DiscreteEvent.h"


	DiscreteEvent::DiscreteEvent( double time, int type, int eventID, int oplineNum)  {
		_time = time;
		_type = type;
		_eventID = eventID;
		_oplineNum = oplineNum;
	}

	double DiscreteEvent::getTime() {
			return _time;
	}

	int DiscreteEvent::getType(){
		return _type;
	}

	int DiscreteEvent::getEventID(){
		return _eventID;
	}
	
	int DiscreteEvent::getOplineNum(){
		return _oplineNum;
	}