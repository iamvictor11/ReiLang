#ifndef REI_INTERNAL_CALLBACKS_H
#define REI_INTERNAL_CALLBACKS_H

#include "rei/rei.h"

extern ReiCallbacks reiCallbacks_g;

#define reiDebug(LVL, MSG) reiCallbacks_g.debug(reiCallbacks_g.context, LVL, MSG)

#endif
