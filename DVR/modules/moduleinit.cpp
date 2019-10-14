#include "moduleinit.h"
#include "platformregister.h"


void module_init()
{
    PlatformRegister::getPlatformRegister();
}
