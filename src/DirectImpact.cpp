
#include "DirectImpact.h"

DirectImpact::DirectImpact(int _target, double  _probability)  {
	target = _target;
	probability = _probability;
}

int DirectImpact::getTarget() {
	return target;
}

double DirectImpact::getProbability() {
	return probability;
}
