#include "../../../Memory/Memory.hpp"

class Weather {
public:
	BUILD_ACCESS(this, float, RainLevel, 0x38);
	BUILD_ACCESS(this, float, LightningLevelOld, 0x40);
	BUILD_ACCESS(this, float, LightningLevelNew, 0x44);
	BUILD_ACCESS(this, float, FogLevel, 0x4C); //definetely not fog level (bds lied)
};