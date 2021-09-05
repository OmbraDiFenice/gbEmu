#include <Pch.h>

#include <utils/GLErrorMacros.h>
#include "Synchronization.h"

void SynchronizeOn(const SynchroItem& iSynchroItem) {
    GLCall(glMemoryBarrier(iSynchroItem));
}
