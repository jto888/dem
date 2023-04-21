#ifndef _failure_repair_H
#define _failure_repair_H

#include "dem.h"


void OnFailure(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL);
		
void handleInlineFailure(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL);
void OnRepair(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL);
		
void handleInlineRepair(std::shared_ptr<DiscreteEvent>& ev,
		std::unique_ptr<ElementList>& EL,
		std::shared_ptr<EventQueue>& EQ,
		std::shared_ptr<OpLineList>& OLL);

void recursive_dependency_activator(
		std::shared_ptr<OpLineList>& OLL, 
		int this_opline);

void recursive_dependency_passivator(
		//std::unique_ptr<ElementList>& EL,
		std::shared_ptr<OpLineList>& OLL, 
		int this_opline);
		
		
#endif	