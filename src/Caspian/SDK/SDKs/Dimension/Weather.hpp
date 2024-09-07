#include "../../../Memory/Memory.hpp"

class Weather {
public:
	BUILD_ACCESS(this, float, RainLevel, 0x38);
	BUILD_ACCESS(this, float, LightningLevel, 0x48); //wrong or mc doesn't use it
	BUILD_ACCESS(this, float, FogLevel, 0x4C); //definetely not fog level (bds lied)
};