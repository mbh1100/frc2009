#ifndef ADV_VICTOR_H
#define ADV_VICTOR_H

#include "AdvMotorController.h"
#include "Victor.h"

/* Advanced Victor using the features of the Advanced Motor
 * Controller interface.
 */
class AdvVictor : public Victor, public AdvMotorController
{
public:
	AdvVictor(UINT32 slot, UINT32 channel);
	virtual ~AdvVictor();
	
	void Set(float value);
	float Get();
};

#endif //ADV_VICTOR_H
