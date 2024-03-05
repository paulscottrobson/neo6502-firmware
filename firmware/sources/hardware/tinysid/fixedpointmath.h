/* ------------------------------------------------------------------------- */

/*
 * Fixed Point Math Macro Library
 *
 * (C)opyright 2005,2006 by Ivo van Poorten
 * Licensed under version 2 of the GPL
 *
 * $Id: fixedpointmath.h 106 2008-03-19 15:13:43Z ivop $
 *
 */

/* ------------------------------------------------------------------------- */

#ifndef __FIXEDPOINTMATH_H__
#define __FIXEDPOINTMATH_H__

/* ------------------------------------------------------------------------- */

#define __FPM_VERSION__         0.31
#define __FPM_VERSION_MAJOR__   0
#define __FPM_VERSION_MINOR__   31
#define __FPM_VERSION_RELEASE__ beta

/* ------------------------------------------------------------------------- */

#include <stdint.h>

#include "fixedpointmathcode.h"

/* ------------------------------------------------------------------------- */

/* int to fixed point */

#define itofp8p8(x)     _itofp8p8(x)
#define itofp24p8(x)    _itofp24p8(x)
#define itofp20p12(x)   _itofp20p12(x)
#define itofp16p16(x)   _itofp16p16(x)
#define itofp8p24(x)    _itofp8p24(x)
#define itoufp8p8(x)    _itoufp8p8(x)
#define itoufp20p12(x)  _itoufp20p12(x)
#define itoufp24p8(x)   _itoufp24p8(x)
#define itoufp16p16(x)  _itoufp16p16(x)
#define itoufp8p24(x)   _itoufp8p24(x)

/* float to fixed point */

#define ftofp8p8(x)     _ftofp8p8(x)
#define ftofp24p8(x)    _ftofp24p8(x)
#define ftofp20p12(x)   _ftofp20p12(x)
#define ftofp16p16(x)   _ftofp16p16(x)
#define ftofp8p24(x)    _ftofp8p24(x)
#define ftoufp8p8(x)    _ftoufp8p8(x)
#define ftoufp24p8(x)   _ftoufp24p8(x)
#define ftoufp20p12(x)  _ftoufp20p12(x)
#define ftoufp16p16(x)  _ftoufp16p16(x)
#define ftoufp8p24(x)   _ftoufp8p24(x)

/* double to fixed point */

#define dtofp8p8(x)     _dtofp8p8(x)
#define dtofp24p8(x)    _dtofp24p8(x)
#define dtofp20p12(x)   _dtofp20p12(x)
#define dtofp16p16(x)   _dtofp16p16(x)
#define dtofp8p24(x)    _dtofp8p24(x)
#define dtoufp8p8(x)    _dtoufp8p8(x)
#define dtoufp24p8(x)   _dtoufp24p8(x)
#define dtoufp20p12(x)  _dtoufp20p12(x)
#define dtoufp16p16(x)  _dtoufp16p16(x)
#define dtoufp8p24(x)   _dtoufp8p24(x)

/* fixed point to int */

#define fp8p8toi(x)     _fp8p8toi(x)
#define fp24p8toi(x)    _fp24p8toi(x)
#define fp20p12toi(x)   _fp20p12toi(x)
#define fp16p16toi(x)   _fp16p16toi(x)
#define fp8p24toi(x)    _fp8p24toi(x)
#define ufp8p8toi(x)    _ufp8p8toi(x)
#define ufp24p8toi(x)   _ufp24p8toi(x)
#define ufp20p12toi(x)  _ufp20p12toi(x)
#define ufp16p16toi(x)  _ufp16p16toi(x)
#define ufp8p24toi(x)   _ufp8p24toi(x)

/* fixed point to float */

#define fp8p8tof(x)     _fp8p8tof(x)
#define fp24p8tof(x)    _fp24p8tof(x)
#define fp20p12tof(x)   _fp20p12tof(x)
#define fp16p16tof(x)   _fp16p16tof(x)
#define fp8p24tof(x)    _fp8p24tof(x)
#define ufp8p8tof(x)    _ufp8p8tof(x)
#define ufp24p8tof(x)   _ufp24p8tof(x)
#define ufp20p12tof(x)  _ufp20p12tof(x)
#define ufp16p16tof(x)  _ufp16p16tof(x)
#define ufp8p24tof(x)   _ufp8p24tof(x)

/* fixed point to double */

#define fp8p8tod(x)     _fp8p8tod(x)
#define fp24p8tod(x)    _fp24p8tod(x)
#define fp20p12tod(x)   _fp20p12tod(x)
#define fp16p16tod(x)   _fp16p16tod(x)
#define fp8p24tod(x)    _fp8p24tod(x)
#define ufp8p8tod(x)    _ufp8p8tod(x)
#define ufp24p8tod(x)   _ufp24p8tod(x)
#define ufp20p12tod(x)  _ufp20p12tod(x)
#define ufp16p16tod(x)  _ufp16p16tod(x)
#define ufp8p24tod(x)   _ufp8p24tod(x)

/* multiplication (x*y) */

#define mulfp8p8(x,y)    _mulfp8p8(x,y)
#define mulfp24p8(x,y)   _mulfp24p8(x,y)
#define mulfp20p12(x,y)  _mulfp20p12(x,y)
#define mulfp16p16(x,y)  _mulfp16p16(x,y)
#define mulfp8p24(x,y)   _mulfp8p24(x,y)
#define mulufp8p8(x,y)   _mulufp8p8(x,y)
#define mulufp24p8(x,y)  _mulufp24p8(x,y)
#define mulufp20p12(x,y) _mulufp20p12(x,y)
#define mulufp16p16(x,y) _mulufp16p16(x,y)
#define mulufp8p24(x,y)  _mulufp8p24(x,y)

/* division (x/y) */

#define divfp8p8(x,y)    _divfp8p8(x,y)
#define divfp24p8(x,y)   _divfp24p8(x,y)
#define divfp20p12(x,y)  _divfp20p12(x,y)
#define divfp16p16(x,y)  _divfp16p16(x,y)
#define divfp8p24(x,y)   _divfp8p24(x,y)
#define divufp8p8(x,y)   _divufp8p8(x,y)
#define divufp24p8(x,y)  _divufp24p8(x,y)
#define divufp20p12(x,y) _divufp20p12(x,y)
#define divufp16p16(x,y) _divufp16p16(x,y)
#define divufp8p24(x,y)  _divufp8p24(x,y)

/* square root */

/* Hmm, might need to move this stuff to ....code and have one more level
 * of _ so here it's just #define bla(x) _bla(x) no matter what, especially
 * when the other functions get LUT support too and/or more functions
 * are added.
 */

#if defined(__FPM_ENABLE_ALL_LUT__) || defined(__FPM_ENABLE_SQRT_LUT__)

#define sqrtufp8p8(x)   _sqrtufp8p8lut(x)
#define sqrtufp24p8(x)  _sqrtufp24p8lut(x)
#define sqrtufp16p16(x) _sqrtufp16p16lut(x)
#define sqrtufp8p24(x)  _sqrtufp8p24lut(x)
#define sqrtfp8p8(x)    _sqrtfp8p8lut(x)
#define sqrtfp24p8(x)   _sqrtfp24p8lut(x)
#define sqrtfp16p16(x)  _sqrtfp16p16lut(x)
#define sqrtfp8p24(x)   _sqrtfp8p24lut(x)

#else

#define sqrtufp8p8(x)   _sqrtufp8p8(x)
#define sqrtufp24p8(x)  _sqrtufp24p8(x)
#define sqrtufp20p12(x) _sqrtufp20p12(x)
#define sqrtufp16p16(x) _sqrtufp16p16(x)
#define sqrtufp8p24(x)  _sqrtufp8p24(x)
#define sqrtfp8p8(x)    _sqrtfp8p8(x)
#define sqrtfp24p8(x)   _sqrtfp24p8(x)
#define sqrtfp20p12(x)  _sqrtfp20p12(x)
#define sqrtfp16p16(x)  _sqrtfp16p16(x)
#define sqrtfp8p24(x)   _sqrtfp8p24(x)

#endif

/* exponential function e^x for x>=0 */

#if defined(__FPM_ENABLE_ALL_LUT__) || defined(__FPM_ENABLE_EXP_LUT__)

#define expufp8p8(x)   _expufp8p8lut(x)
#define expufp24p8(x)  _expufp24p8lut(x)
#define expufp16p16(x) _expufp16p16lut(x)
#define expufp8p24(x)  _expufp8p24lut(x)
#define expfp8p8(x)    _expfp8p8lut(x)
#define expfp24p8(x)   _expfp24p8lut(x)
#define expfp16p16(x)  _expfp16p16lut(x)
#define expfp8p24(x)   _expfp8p24lut(x)

#else

#define expfp8p8(x)     _expfp8p8(x)
#define expfp24p8(x)    _expfp24p8(x)
#define expfp16p16(x)   _expfp16p16(x)
#define expfp8p24(x)    _expfp8p24(x)
#define expufp8p8(x)    _expufp8p8(x)
#define expufp24p8(x)   _expufp24p8(x)
#define expufp16p16(x)  _expufp16p16(x)
#define expufp8p24(x)   _expufp8p24(x)

#endif

/* natural logarithm ln(x) */

#if defined(__FPM_ENABLE_ALL_LUT__) || defined(__FPM_ENABLE_LN_LUT__)

#define lnufp8p8(x)   _lnufp8p8lut(x)
#define lnufp24p8(x)  _lnufp24p8lut(x)
#define lnufp16p16(x) _lnufp16p16lut(x)
#define lnufp8p24(x)  _lnufp8p24lut(x)
#define lnfp8p8(x)    _lnfp8p8lut(x)
#define lnfp24p8(x)   _lnfp24p8lut(x)
#define lnfp16p16(x)  _lnfp16p16lut(x)
#define lnfp8p24(x)   _lnfp8p24lut(x)

#else

#define lnfp8p8(a)      _lnfp8p8(a)
#define lnfp24p8(a)     _lnfp24p8(a)
#define lnfp16p16(a)    _lnfp16p16(a)
#define lnfp8p24(a)     _lnfp8p24(a)
#define lnufp8p8(a)     _lnufp8p8(a)
#define lnufp24p8(a)    _lnufp24p8(a)
#define lnufp16p16(a)   _lnufp16p16(a)
#define lnufp8p24(a)    _lnufp8p24(a)

#endif

/* power function  pow(x,y) = x^y */

#define powfp8p8(x,y)    _powfp8p8(x,y)
#define powfp24p8(x,y)   _powfp24p8(x,y)
#define powfp16p16(x,y)  _powfp16p16(x,y)
#define powfp8p24(x,y)   _powfp8p24(x,y)
#define powufp8p8(x,y)   _powufp8p8(x,y)
#define powufp24p8(x,y)  _powufp24p8(x,y)
#define powufp16p16(x,y) _powufp16p16(x,y)
#define powufp8p24(x,y)  _powufp8p24(x,y)

/* ------------------------------------------------------------------------- */

#endif

/* ------------------------------------------------------------------------- */

