#include <Pch.h>

#include <core/graphics/opengl/Synchronization.h>
#include <utils/GLErrorMacros.h>

void SynchronizeOn(const SynchroItem& iSynchroItem) {
    GLCall(glMemoryBarrier(iSynchroItem));
}
