/* Function definitions for the Wheel structure */

#include "wheel.h"

Wheel::Wheel()
{
	Type=UNDEF_WHEEL_TYPE;
}

Wheel::Wheel(int type)
{
	Type=type;
}
