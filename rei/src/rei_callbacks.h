#ifndef REI_CALLBACKS_H
#define REI_CALLBACKS_H

#include "rei_internal.h"

extern ReiCallbacks reiCallbacks_g;

#define reiDebug(LVL, MSG) reiCallbacks_g.debug(reiCallbacks_g.context, LVL, MSG)

#endif
