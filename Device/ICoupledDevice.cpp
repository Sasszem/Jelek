#include "ICoupledDevice.h"

ICoupledDevice::ICoupledDevice(unsigned id, unsigned plus, unsigned minus, unsigned otherId): IDevice(id, plus, minus), coupledId(otherId)
{
}
