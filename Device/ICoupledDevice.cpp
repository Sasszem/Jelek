#include "ICoupledDevice.h"

using Analyzer::Device::ICoupledDevice;
using Analyzer::Device::IDevice;

ICoupledDevice::ICoupledDevice(unsigned id, unsigned plus, unsigned minus, unsigned otherId): IDevice(id, plus, minus), coupledId(otherId)
{
}
