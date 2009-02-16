#ifndef ADV_JAUGER_H
#define ADV_JAUGER_H

#include "AdvMotorController.h"
#include "Jaguar.h"

/* Advanced Jaguar using the features of the Advanced Motor
 * Controller interface.
 */
class AdvJaguar : public Jaguar, public AdvMotorController
{
public:
	AdvJaguar(UINT32 slot, UINT32 channel);
	virtual ~AdvJaguar();
	
	void Set(float value);
	float Get();
};

#endif //ADV_JAGUAR_H
