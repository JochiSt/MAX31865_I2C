#ifndef GLOBAL_H
#define GLOBAL_H

/// defines to use true and false in a convenient way
#define true    1
#define false   0
#define True    1
#define False   0

/// Macros for easy handling of the digital IO
#define RESET(x)        _XRESET(x)
#define SET(x)          _XSET(x)
#define TOGGLE(x)       _XTOGGLE(x)
#define SET_OUTPUT(x)   _XSETOUTP(x)
#define SET_INPUT(x)    _XSETINP(x)
#define IS_SET(x)       _XISSET(x)

#define PORT(x)         _XPORT(x)
#define DDR(x)          _XDDR(x)
#define PIN(x)          _PIN(x)

/// internal functions to expand the parameters
#define _XRESET(x,y)    PORT(x) &= ~(1<<y)
#define _XSET(x,y)      PORT(x) |=  (1<<y)
#define _XTOGGLE(x,y)   PORT(x) ^=  (1<<y)

#define _XSETOUTP(x,y)  DDR(x) |=  (1<<y)
#define _XSETINP(x,y)   DDR(x) &= ~(1<<y)

#define _XISSET(x,y)    ((PIN(x) & (1<<y)) != 0)

#define _XPORT(x)       PORT ## x
#define _XDDR(x)        DDR ## x
#define _PIN(x)         PIN ## x

#endif
