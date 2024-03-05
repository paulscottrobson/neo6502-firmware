/* ------------------------------------------------------------------------- */

/* DO NOT INCLUDE THIS FILE DIRECTLY! ALWAYS INCLUDE fixedpointmath.h only!! */

/*
 * Fixed Point Math Macro Library
 *
 * This file contains the actual implementations of the functions.
 *
 * (C)opyright 2005,2006 by Ivo van Poorten
 * Licensed under version 2 of the GPL
 *
 * $Id: fixedpointmathcode.h 106 2008-03-19 15:13:43Z ivop $
 *
 */

/* ------------------------------------------------------------------------- */

#ifndef __FIXEDPOINTMATHCODE_H__
#define __FIXEDPOINTMATHCODE_H__

/* ------------------------------------------------------------------------- */

/* signed */

typedef int16_t fp8p8_t;
typedef int32_t fp24p8_t;
typedef int32_t fp20p12_t;
typedef int32_t fp16p16_t;
typedef int32_t fp8p24_t;

/* unsigned */

typedef uint16_t ufp8p8_t;
typedef uint32_t ufp24p8_t;
typedef uint32_t ufp20p12_t;
typedef uint32_t ufp16p16_t;
typedef uint32_t ufp8p24_t;

/* ------------------------------------------------------------------------- */

#if defined(__FPM_ENABLE_ALL_LUT__) || defined(__FPM_ENABLE_SQRT_LUT__)
#include "fixedpointmathlut.h"
#endif

/* ------------------------------------------------------------------------- */

#if defined(__INTEL_COMPILER)
#pragma warning(disable:187)    /* '=' where '==' may have been intended */
#endif

/* ------------------------------------------------------------------------- */

/* CONVERSION */

/* int to fixed point */

#define _itofp8p8(x)    ( (int16_t)  (x) << 8 )
#define _itofp24p8(x)   ( (int32_t)  (x) << 8 )
#define _itofp20p12(x)  ( (int32_t)  (x) << 12 )
#define _itofp16p16(x)  ( (int32_t)  (x) << 16 )
#define _itofp8p24(x)   ( (int32_t)  (x) << 24 )
#define _itoufp8p8(x)   ( (uint16_t) (x) << 8 )
#define _itoufp24p8(x)  ( (uint32_t) (x) << 8 )
#define _itoufp20p12(x) ( (uint32_t) (x) << 12 )
#define _itoufp16p16(x) ( (uint32_t) (x) << 16 )
#define _itoufp8p24(x)  ( (uint32_t) (x) << 24 )

/* float to fixed point */

#define _ftofp8p8(x)    ( (fp8p8_t)    ((x) * 256.0) )
#define _ftofp24p8(x)   ( (fp24p8_t)   ((x) * 256.0) )
#define _ftofp20p12(x)  ( (fp20p12_t)  ((x) * 4096.0) )
#define _ftofp16p16(x)  ( (fp16p16_t)  ((x) * 65536.0) )
#define _ftofp8p24(x)   ( (fp8p24_t)   ((x) * 16777216.0) )
#define _ftoufp8p8(x)   ( (ufp8p8_t)   ((x) * 256.0) )
#define _ftoufp24p8(x)  ( (ufp24p8_t)  ((x) * 256.0) )
#define _ftoufp20p12(x) ( (ufp20p12_t) ((x) * 4096.0) )
#define _ftoufp16p16(x) ( (ufp16p16_t) ((x) * 65536.0) )
#define _ftoufp8p24(x)  ( (ufp8p24_t)  ((x) * 16777216.0) )

/* double to fixed point */

#define _dtofp8p8(x)    _ftofp8p8(x)
#define _dtofp24p8(x)   _ftofp24p8(x)
#define _dtofp20p12(x)  _ftofp20p12(x)
#define _dtofp16p16(x)  _ftofp16p16(x)
#define _dtofp8p24(x)   _ftofp8p24(x)
#define _dtoufp8p8(x)   _ftoufp8p8(x)
#define _dtoufp24p8(x)  _ftoufp24p8(x)
#define _dtoufp20p12(x) _ftoufp20p12(x)
#define _dtoufp16p16(x) _ftoufp16p16(x)
#define _dtoufp8p24(x)  _ftoufp8p24(x)

/* fixed point to int */

#define _fp8p8toi(x)    ( (x) >> 8 )
#define _fp24p8toi(x)   ( (x) >> 8 )
#define _fp20p12toi(x)  ( (x) >> 12 )
#define _fp16p16toi(x)  ( (x) >> 16 )
#define _fp8p24toi(x)   ( (x) >> 24 )
#define _ufp8p8toi(x)   _fp8p8toi(x)
#define _ufp24p8toi(x)  _fp24p8toi(x)
#define _ufp20p12toi(x) _fp20p12toi(x)
#define _ufp16p16toi(x) _fp16p16toi(x)
#define _ufp8p24toi(x)  _fp8p24toi(x)

/* fixed point to float */

#define _fp8p8tof(x)    ( (float) (x) / 256.0 )
#define _fp24p8tof(x)   ( (float) (x) / 256.0 )
#define _fp20p12tof(x)  ( (float) (x) / 4096.0 )
#define _fp16p16tof(x)  ( (float) (x) / 65536.0 )
#define _fp8p24tof(x)   ( (float) (x) / 16777216.0 )
#define _ufp8p8tof(x)   _fp8p8tof(x)
#define _ufp24p8tof(x)  _fp24p8tof(x)
#define _ufp20p12tof(x) _fp20p12tof(x)
#define _ufp16p16tof(x) _fp16p16tof(x)
#define _ufp8p24tof(x)  _fp8p24tof(x)

/* fixed point to double */

#define _fp8p8tod(x)    ( (double) (x) / 256.0 )
#define _fp24p8tod(x)   ( (double) (x) / 256.0 )
#define _fp20p12tod(x)  ( (double) (x) / 4096.0 )
#define _fp16p16tod(x)  ( (double) (x) / 65536.0 )
#define _fp8p24tod(x)   ( (double) (x) / 16777216.0 )
#define _ufp8p8tod(x)   _fp8p8tod(x)
#define _ufp24p8tod(x)  _fp24p8tod(x)
#define _ufp20p12tod(x) _fp20p12tod(x)
#define _ufp16p16tod(x) _fp16p16tod(x)
#define _ufp8p24tod(x)  _fp8p24tod(x)

/* fixed point to fixed point */

#define _fp8p8tofp8p8(x)                        (x)
#define _fp8p8tofp24p8(x)         ((fp24p8_t)   (x))
#define _fp8p8tofp20p12(x)      ( ((fp20p12_t)  (x)) <<  4 )
#define _fp8p8tofp16p16(x)      ( ((fp16p16_t)  (x)) <<  8 )
#define _fp8p8tofp8p24(x)       ( ((fp8p24_t)   (x)) << 16 )
#define _ufp8p8toufp8p8(x)                      (x)
#define _ufp8p8toufp24p8(x)       ((ufp24p8_t)  (x))
#define _ufp8p8toufp20p12(x)    ( ((fp20p12_t)  (x)) <<  4 )
#define _ufp8p8toufp16p16(x)    ( ((ufp16p16_t) (x)) <<  8 )
#define _ufp8p8toufp8p24(x)     ( ((ufp8p24_t)  (x)) << 16 )

#define _fp24p8tofp8p8(x)         ((fp8p8_t)    (x))
#define _fp24p8tofp24p8(x)                      (x)
#define _fp24p8tofp20p12(x)     ( ((fp20p12_t)  (x)) << 4 )
#define _fp24p8tofp16p16(x)     ( ((fp16p16_t)  (x)) << 8 )
#define _fp24p8tofp8p24(x)      ( ((fp8p24_t)   (x)) << 16)
#define _ufp24p8toufp8p8(x)       ((ufp8p8_t)   (x))
#define _ufp24p8toufp24p8(x)                    (x)
#define _ufp24p8toufp20p12(x)   ( ((fp20p12_t)  (x)) << 4 )
#define _ufp24p8toufp16p16(x)   ( ((ufp16p16_t) (x)) << 8 )
#define _ufp24p8toufp8p24(x)    ( ((ufp8p24_t)  (x)) << 16)

#define _fp20p12tofp8p8(x)      ( (fp8p8_t)    ((x)  >> 4 ) )
#define _fp20p12tofp24p8(x)     ( (fp24p8_t)   ((x)  >> 4 ) )
#define _fp20p12tofp20p12(x)                    (x)
#define _fp20p12tofp16p16(x)    ( (fp16p16_t)  ((x)  << 4 ) )
#define _fp20p12tofp8p24(x)     ( (fp8p24_t)   ((x)  << 12 ) )
#define _ufp20p12toufp8p8(x)    ( (ufp8p8_t)   ((x)  >> 4 ) )
#define _ufp20p12toufp24p8(x)   ( (ufp24p8_t)  ((x)  >> 4 ) )
#define _ufp20p12tofp20p12(x)                   (x)
#define _fp20p12tofp16p16(x)    ( (fp16p16_t)  ((x)  << 4 ) )
#define _ufp20p12toufp8p24(x)   ( (ufp8p24_t)  ((x)  << 12 ) )

#define _fp16p16tofp8p8(x)      ( (fp8p8_t)    ((x)  >> 8 ) )
#define _fp16p16tofp24p8(x)     ( (fp24p8_t)   ((x)  >> 8 ) )
#define _fp16p16tofp20p12(x)    ( (fp20p12_t)  ((x)  >> 4 ) )
#define _fp16p16tofp16p16(x)                    (x)
#define _fp16p16tofp8p24(x)     ( (fp8p24_t)   ((x)  << 8 ) )
#define _ufp16p16toufp8p8(x)    ( (ufp8p8_t)   ((x)  >> 8 ) )
#define _fp16p16tofp20p12(x)    ( (fp20p12_t)  ((x)  >> 4 ) )
#define _ufp16p16toufp24p8(x)   ( (ufp24p8_t)  ((x)  >> 8 ) )
#define _ufp16p16toufp16p16(x)                  (x)
#define _ufp16p16toufp8p24(x)   ( (ufp8p24_t)  ((x)  << 8 ) )

#define _fp8p24tofp8p8(x)       ( (fp8p8_t)    ((x)  >> 16) )
#define _fp8p24tofp24p8(x)      ( (fp24p8_t)   ((x)  >> 16) )
#define _fp8p24tofp20p12(x)     ( (fp20p12_t)  ((x)  >> 12 ) )
#define _fp8p24tofp16p16(x)     ( (fp16p16_t)  ((x)  >> 8 ) )
#define _fp8p24tofp8p24(x)                      (x)
#define _ufp8p24toufp8p8(x)     ( (ufp8p8_t)   ((x)  >> 16) )
#define _ufp8p24toufp24p8(x)    ( (ufp24p8_t)  ((x)  >> 16) )
#define _ufp8p24toufp20p12(x)   ( (fp20p12_t)  ((x)  >> 12 ) )
#define _ufp8p24toufp16p16(x)   ( (ufp16p16_t) ((x)  >> 8 ) )
#define _ufp8p24toufp8p24(x)                    (x)

/* ------------------------------------------------------------------------- */

/* SIMPLE MATH */

/* multiplication (x*y) */

#define _mulfp8p8(x,y)    ( ( (int32_t)(x) *  (int32_t)(y)) >> 8  )
#define _mulfp24p8(x,y)   ( ( (int64_t)(x) *  (int64_t)(y)) >> 8  )
#define _mulfp20p12(x,y)  ( ( (int64_t)(x) *  (int64_t)(y)) >> 12 )
#define _mulfp16p16(x,y)  ( ( (int64_t)(x) *  (int64_t)(y)) >> 16 )
#define _mulfp8p24(x,y)   ( ( (int64_t)(x) *  (int64_t)(y)) >> 24 )
#define _mulufp8p8(x,y)   ( ((uint32_t)(x) * (uint32_t)(y)) >> 8  )
#define _mulufp24p8(x,y)  ( ((uint64_t)(x) * (uint64_t)(y)) >> 8  )
#define _mulufp20p12(x,y) ( ((uint64_t)(x) * (uint64_t)(y)) >> 12 )
#define _mulufp16p16(x,y) ( ((uint64_t)(x) * (uint64_t)(y)) >> 16 )
#define _mulufp8p24(x,y)  ( ((uint64_t)(x) * (uint64_t)(y)) >> 24 )

/* division (x/y) */

#define _divfp8p8(x,y)    ( ( (int32_t)(x) << 8 ) / (y) )
#define _divfp24p8(x,y)   ( ( (int64_t)(x) << 8 ) / (y) )
#define _divfp20p12(x,y)  ( ( (int64_t)(x) << 12) / (y) )
#define _divfp16p16(x,y)  ( ( (int64_t)(x) << 16) / (y) )
#define _divfp8p24(x,y)   ( ( (int64_t)(x) << 24) / (y) )
#define _divufp8p8(x,y)   ( ((uint32_t)(x) << 8 ) / (y) )
#define _divufp24p8(x,y)  ( ((uint64_t)(x) << 8 ) / (y) )
#define _divufp20p12(x,y) ( ((uint64_t)(x) << 12) / (y) )
#define _divufp16p16(x,y) ( ((uint64_t)(x) << 16) / (y) )
#define _divufp8p24(x,y)  ( ((uint64_t)(x) << 24) / (y) )

/* ------------------------------------------------------------------------- */

/* MORE ADVANCED MATH */

/* ------------------------------------------------------------------------- */

/* unsigned a less|greater than b for 16-bit and 32-bit types
 * result: 0 or 1
 */

#define _ualtb16(a,b)  ((((uint32_t)a-(uint32_t)b)>>31)&1)
#define _ualtb32(a,b)  ((((uint64_t)a-(uint64_t)b)>>63)&1)
#define _uagtb16(a,b)  _ualtb16(b,a)
#define _uagtb32(a,b)  _ualtb32(b,a)

/* >>> Square root <<< */

/* temporary variables */

#define unused __attribute__((unused))
static ufp8p8_t   _ufp8p8tmp unused,   _ufp8p8tmp2 unused,    _ufp8p8tmp3 unused;
static ufp24p8_t  _ufp24p8tmp unused,  _ufp24p8tmp2 unused,   _ufp24p8tmp3 unused;
static ufp20p12_t _ufp20p12tmp unused, _ufp20p12tmp2 unused,  _ufp20p12tmp3 unused;
static ufp16p16_t _ufp16p16tmp unused, _ufp16p16tmp2 unused,  _ufp16p16tmp3 unused;
static ufp8p24_t  _ufp8p24tmp unused,  _ufp8p24tmp2 unused,   _ufp8p24tmp3 unused;
#undef unused

#define _mid(a,b)           (a+((b-a)>>1))

#define _convergeufp8p8(a,b,m,x)    ( \
            ( (m=(_ualtb16(_mulufp8p8(_mid(a,b),_mid(a,b)),x)))||1) &&    \
            ( (a=m*_mid(a,b)+(1-m)*a)||1 ) && ( (b=(1-m)*_mid(a,b)+m*b)||1 ) \
                                    )
#define _convergeufp24p8(a,b,m,x)   ( \
            ( (m=(_ualtb32(_mulufp24p8(_mid(a,b),_mid(a,b)),x)))||1) &&   \
            ( (a=m*_mid(a,b)+(1-m)*a)||1 ) && ( (b=(1-m)*_mid(a,b)+m*b)||1 ) \
                                    )
#define _convergeufp20p12(a,b,m,x)   ( \
            ( (m=(_ualtb32(_mulufp20p12(_mid(a,b),_mid(a,b)),x)))||1) &&   \
            ( (a=m*_mid(a,b)+(1-m)*a)||1 ) && ( (b=(1-m)*_mid(a,b)+m*b)||1 ) \
                                    )
#define _convergeufp16p16(a,b,m,x)   ( \
            ( (m=(_ualtb32(_mulufp16p16(_mid(a,b),_mid(a,b)),x)))||1) &&   \
            ( (a=m*_mid(a,b)+(1-m)*a)||1 ) && ( (b=(1-m)*_mid(a,b)+m*b)||1 ) \
                                    )
#define _convergeufp8p24(a,b,m,x)   ( \
            ( (m=(_ualtb32(_mulufp8p24(_mid(a,b),_mid(a,b)),x)))||1) &&   \
            ( (a=m*_mid(a,b)+(1-m)*a)||1 ) && ( (b=(1-m)*_mid(a,b)+m*b)||1 ) \
                                    )

#define __sqrtufp8p8(a,b,m,x)   ( ((m=_uagtb16(x,(4<<8)))||1) && \
                                  ((a=m*(1<<8))||1) &&               \
                                  ((b=m*(x>>1)+(1-m)*(x<<1))||1)     ) \
                              && _convergeufp8p8(a,b,m,x) \
                              && _convergeufp8p8(a,b,m,x) \
                              && _convergeufp8p8(a,b,m,x) \
                              && _convergeufp8p8(a,b,m,x) \
                              && _convergeufp8p8(a,b,m,x) \
                              && _convergeufp8p8(a,b,m,x) \
                              && _convergeufp8p8(a,b,m,x) \
                              && _convergeufp8p8(a,b,m,x) \
                              && _convergeufp8p8(a,b,m,x) \
                              && _convergeufp8p8(a,b,m,x) \
                              && _convergeufp8p8(a,b,m,x) \
                              && _convergeufp8p8(a,b,m,x) \
                              ? _mid(a,b) : 0

#define __sqrtufp24p8(a,b,m,x)  ( ((m=_uagtb32(x,(4<<8)))||1) && \
                                  ((a=m*(1<<8))||1) &&               \
                                  ((b=m*(x>>1)+(1-m)*(x<<1))||1)     ) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                && _convergeufp24p8(a,b,m,x) \
                                ? _mid(a,b) : 0

#define __sqrtufp20p12(a,b,m,x)  ( ((m=_uagtb32(x,(4<<12)))||1) && \
                                   ((a=m*(1<<12))||1) &&               \
                                   ((b=m*(x>>1)+(1-m)*(x<<1))||1)     ) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                && _convergeufp20p12(a,b,m,x) \
                                ? _mid(a,b) : 0

#define __sqrtufp16p16(a,b,m,x)  ( ((m=_uagtb32(x,(4<<16)))||1) && \
                                   ((a=m*(1<<16))||1) &&               \
                                   ((b=m*(x>>1)+(1-m)*(x<<1))||1)     ) \
                              && _convergeufp16p16(a,b,m,x) \
                              && _convergeufp16p16(a,b,m,x) \
                              && _convergeufp16p16(a,b,m,x) \
                              && _convergeufp16p16(a,b,m,x) \
                              && _convergeufp16p16(a,b,m,x) \
                              && _convergeufp16p16(a,b,m,x) \
                              && _convergeufp16p16(a,b,m,x) \
                              && _convergeufp16p16(a,b,m,x) \
                              && _convergeufp16p16(a,b,m,x) \
                              && _convergeufp16p16(a,b,m,x) \
                              && _convergeufp16p16(a,b,m,x) \
                              && _convergeufp16p16(a,b,m,x) \
                              ? _mid(a,b) : 0

#define __sqrtufp8p24(a,b,m,x)  ( ((m=_uagtb32(x,(4<<24)))||1) && \
                                   ((a=m*(1<<24))||1) &&               \
                                   ((b=m*(x>>1)+(1-m)*(x<<1))||1)     ) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              && _convergeufp8p24(a,b,m,x) \
                              ? _mid(a,b) : 0

#define _sqrtufp8p8(x)      __sqrtufp8p8  (_ufp8p8tmp,   _ufp8p8tmp2,   \
                                           _ufp8p8tmp3,   x)
#define _sqrtufp24p8(x)     __sqrtufp24p8 (_ufp24p8tmp,  _ufp24p8tmp2,  \
                                           _ufp24p8tmp3,  x)
#define _sqrtufp20p12(x)    __sqrtufp20p12(_ufp20p12tmp, _ufp20p12tmp2, \
                                           _ufp20p12tmp3, x)
#define _sqrtufp16p16(x)    __sqrtufp16p16(_ufp16p16tmp, _ufp16p16tmp2, \
                                           _ufp16p16tmp3, x)
#define _sqrtufp8p24(x)     __sqrtufp8p24 (_ufp8p24tmp,  _ufp8p24tmp2,  \
                                           _ufp8p24tmp3,  x)

#define _sqrtfp8p8(x)       (x>0 ? _sqrtufp8p8(x)   : 0)
#define _sqrtfp24p8(x)      (x>0 ? _sqrtufp24p8(x)  : 0)
#define _sqrtfp20p12(x)     (x>0 ? _sqrtufp20p12(x) : 0)
#define _sqrtfp16p16(x)     (x>0 ? _sqrtufp16p16(x) : 0)
#define _sqrtfp8p24(x)      (x>0 ? _sqrtufp8p24(x)  : 0)

#define _sqrtufp8p8lut(x)   ( __genericufp8p8lut  (_ufp8p8sqrtlut_16,      \
                                                   _ufp8p8sqrtlut_256,   x ) )
#define _sqrtufp24p8lut(x)  ( __genericufp24p8lut (_ufp24p8sqrtlut_16,     \
                                                   _ufp24p8sqrtlut_256,    \
                                                   _ufp24p8sqrtlut_64k,    \
                                                   _ufp24p8sqrtlut_16m,  x ) )
#define _sqrtufp16p16lut(x) ( __genericufp16p16lut(_ufp16p16sqrtlut_16,    \
                                                   _ufp16p16sqrtlut_256,   \
                                                   _ufp16p16sqrtlut_64k, x ) )
#define _sqrtufp8p24lut(x)  ( __genericufp8p24lut (_ufp8p24sqrtlut_4,      \
                                                   _ufp8p24sqrtlut_16,     \
                                                   _ufp8p24sqrtlut_256,  x ) )

#define _sqrtfp8p8lut(x)    (x>0 ? _sqrtufp8p8lut(x)   : 0)
#define _sqrtfp24p8lut(x)   (x>0 ? _sqrtufp24p8lut(x)  : 0)
#define _sqrtfp16p16lut(x)  (x>0 ? _sqrtufp16p16lut(x) : 0)
#define _sqrtfp8p24lut(x)   (x>0 ? _sqrtufp8p24lut(x)  : 0)

/* >>> Exponential function <<< */

/* e^x for x>=0 */

/* temporary variables */

#define unused __attribute__((unused))
static uint32_t _fp32_x2 unused,  _fp32_x3 unused,  _fp32_x4 unused,  _fp32_x5 unused,  _fp32_x6 unused;
static uint64_t _fp64_x2 unused,  _fp64_x3 unused,  _fp64_x4 unused,  _fp64_x5 unused,  _fp64_x6 unused,
                _fp64_x7 unused,  _fp64_x8 unused,  _fp64_x9 unused,  _fp64_x10 unused, _fp64_x11 unused,
                _fp64_x12 unused, _fp64_x13 unused, _fp64_x14 unused, _fp64_x15 unused, _fp64_x16 unused;
#undef unused

/* I cheat a little here by passing overflowed ufp8p8's to mul and div,
 * but they get cast to uint32_t anyway, so this gives a bit more precission.
 * However, it does _not_ use 64-bit (u)ints and that's intentional!
 * Precision is reasonable for 0 < x < 3 and starts to degrade after that.
 * x>5 will soon start to overflow!
 *
 * All of them have problems when they reach zero, because some of the terms
 * won't fit in the type (i.e. 0.0001 becomes 0) and the expression is
 * false. That's why I add 1 to each term, so every expression evaluates
 * as true. 1 equals to 1/2^8, 1/2^16 and 1/2^24, depending on the type,
 * so the loss of precision is very limited.
 */

#define _expufp8p8(x)   (_fp32_x2 = _mulufp8p8(x       , x        )+1) && \
                        (_fp32_x3 = _mulufp8p8(_fp32_x2, x        )+1) && \
                        (_fp32_x4 = _mulufp8p8(_fp32_x2, _fp32_x2 )+1) && \
                        (_fp32_x5 = _mulufp8p8(_fp32_x4, x        )+1) && \
                        (_fp32_x6 = _mulufp8p8(_fp32_x4, _fp32_x2 )+1) ? \
                        ( 1<<8 ) + \
                        x + _divufp8p8(_fp32_x2, 2<<8 )   + \
                            _divufp8p8(_fp32_x3, 6<<8 )   + \
                            _divufp8p8(_fp32_x4, 24<<8 )  + \
                            _divufp8p8(_fp32_x5, 120<<8 ) + \
                            _divufp8p8(_fp32_x6, 720<<8 ) \
                        : (1<<8)

/* Same cheat for the next three macro's, but with uint64_t instead */

/* precision degrades after x = 9 or there about and overflows soon if x>15 */

#define _expufp24p8(x)  (_fp64_x2  = _mulufp24p8(x       , x        )+1) && \
                        (_fp64_x3  = _mulufp24p8(_fp64_x2, x        )+1) && \
                        (_fp64_x4  = _mulufp24p8(_fp64_x2, _fp64_x2 )+1) && \
                        (_fp64_x5  = _mulufp24p8(_fp64_x3, _fp64_x2 )+1) && \
                        (_fp64_x6  = _mulufp24p8(_fp64_x3, _fp64_x3 )+1) && \
                        (_fp64_x7  = _mulufp24p8(_fp64_x4, _fp64_x3 )+1) && \
                        (_fp64_x8  = _mulufp24p8(_fp64_x4, _fp64_x4 )+1) && \
                        (_fp64_x9  = _mulufp24p8(_fp64_x5, _fp64_x4 )+1) && \
                        (_fp64_x10 = _mulufp24p8(_fp64_x5, _fp64_x5 )+1) && \
                        (_fp64_x11 = _mulufp24p8(_fp64_x6, _fp64_x5 )+1) && \
                        (_fp64_x12 = _mulufp24p8(_fp64_x6, _fp64_x6 )+1) && \
                        (_fp64_x13 = _mulufp24p8(_fp64_x7, _fp64_x6 )+1) && \
                        (_fp64_x14 = _mulufp24p8(_fp64_x7, _fp64_x7 )+1) && \
                        (_fp64_x15 = _mulufp24p8(_fp64_x8, _fp64_x7 )+1) && \
                        (_fp64_x16 = _mulufp24p8(_fp64_x8, _fp64_x8 )+1)  ? \
                        ( 1<<8 ) + \
                        x + \
                        _divufp24p8(_fp64_x2,  2<<8 )     + \
                        _divufp24p8(_fp64_x3,  6<<8 )     + \
                        _divufp24p8(_fp64_x4,  24<<8 )    + \
                        _divufp24p8(_fp64_x5,  120<<8 )   + \
                        _divufp24p8(_fp64_x6,  720<<8 )   + \
                        _divufp24p8(_fp64_x7,  5040<<8 )  + \
                        _divufp24p8(_fp64_x8,  (uint64_t) 40320<<8 )         +\
                        _divufp24p8(_fp64_x9,  (uint64_t) 362880L<<8 )       +\
                        _divufp24p8(_fp64_x10, (uint64_t) 3628800L<<8 )      +\
                        _divufp24p8(_fp64_x11, (uint64_t) 39916800L<<8 )     +\
                        _divufp24p8(_fp64_x12, (uint64_t) 479001600L<<8 )    +\
                        _divufp24p8(_fp64_x13, (uint64_t) 6227020800LL<<8 )  +\
                        _divufp24p8(_fp64_x14, (uint64_t) 87178291200LL<<8 ) +\
                        _divufp24p8(_fp64_x15, (uint64_t) 1307674368000LL<<8)+\
                        _divufp24p8(_fp64_x16, (uint64_t) 20922789888000LL<<8)\
                        : (1<<8)

/* best precision-to-value ratio since it's well balanced */

#define _expufp16p16(x) (_fp64_x2  = _mulufp16p16(x       , x        )+1) && \
                        (_fp64_x3  = _mulufp16p16(_fp64_x2, x        )+1) && \
                        (_fp64_x4  = _mulufp16p16(_fp64_x2, _fp64_x2 )+1) && \
                        (_fp64_x5  = _mulufp16p16(_fp64_x3, _fp64_x2 )+1) && \
                        (_fp64_x6  = _mulufp16p16(_fp64_x3, _fp64_x3 )+1) && \
                        (_fp64_x7  = _mulufp16p16(_fp64_x4, _fp64_x3 )+1) && \
                        (_fp64_x8  = _mulufp16p16(_fp64_x4, _fp64_x4 )+1) && \
                        (_fp64_x9  = _mulufp16p16(_fp64_x5, _fp64_x4 )+1) && \
                        (_fp64_x10 = _mulufp16p16(_fp64_x5, _fp64_x5 )+1)  ? \
                        ( 1<<16 ) + \
                        x + \
                        _divufp16p16(_fp64_x2,  2<<16 )     + \
                        _divufp16p16(_fp64_x3,  6<<16 )     + \
                        _divufp16p16(_fp64_x4,  24<<16 )    + \
                        _divufp16p16(_fp64_x5,  120<<16 )   + \
                        _divufp16p16(_fp64_x6,  720<<16 )   + \
                        _divufp16p16(_fp64_x7,  5040<<16 )  + \
                        _divufp16p16(_fp64_x8,  (uint64_t) 40320<<16 )    + \
                        _divufp16p16(_fp64_x9,  (uint64_t) 362880L<<16 )  + \
                        _divufp16p16(_fp64_x10, (uint64_t) 3628800L<<16 )   \
                        : (1<<16)

/* upper limit of x lowered compared to expfp8p8, so the precision of
 * the fraction is somewhat better, but it overflows sooner if x increases  */

#define _expufp8p24(x)  (_fp64_x2  = _mulufp8p24(x       , x        )+1) && \
                        (_fp64_x3  = _mulufp8p24(_fp64_x2, x        )+1) && \
                        (_fp64_x4  = _mulufp8p24(_fp64_x2, _fp64_x2 )+1) && \
                        (_fp64_x5  = _mulufp8p24(_fp64_x3, _fp64_x2 )+1) && \
                        (_fp64_x6  = _mulufp8p24(_fp64_x3, _fp64_x3 )+1) && \
                        (_fp64_x7  = _mulufp8p24(_fp64_x4, _fp64_x3 )+1) && \
                        (_fp64_x8  = _mulufp8p24(_fp64_x4, _fp64_x4 )+1)  ? \
                        ( 1<<24 ) + \
                        x + \
                        _divufp8p24(_fp64_x2,  2<<24 )     + \
                        _divufp8p24(_fp64_x3,  6<<24 )     + \
                        _divufp8p24(_fp64_x4,  24<<24 )    + \
                        _divufp8p24(_fp64_x5,  120<<24 )   + \
                        _divufp8p24(_fp64_x6,  720<<24 )   +  \
                        _divufp8p24(_fp64_x7,  5040<<24 )  + \
                        _divufp8p24(_fp64_x8,  (uint64_t) 40320<<24 ) \
                        : (1<<24)

#define _expfp8p8(x)    _expufp8p8(x)
#define _expfp24p8(x)   _expufp24p8(x)
#define _expfp16p16(x)  _expufp16p16(x)
#define _expfp8p24(x)   _expufp8p24(x)

#define _expufp8p8lut(x)   ( __genericufp8p8lut  (_ufp8p8explut_16,      \
                                                  _ufp8p8explut_256,   x ) )
#define _expufp24p8lut(x)  ( __genericufp24p8lut (_ufp24p8explut_16,     \
                                                  _ufp24p8explut_256,    \
                                                  _ufp24p8explut_64k,    \
                                                  _ufp24p8explut_16m,  x ) )
#define _expufp16p16lut(x) ( __genericufp16p16lut(_ufp16p16explut_16,    \
                                                  _ufp16p16explut_256,   \
                                                  _ufp16p16explut_64k, x ) )
#define _expufp8p24lut(x)  ( __genericufp8p24lut (_ufp8p24explut_4,      \
                                                  _ufp8p24explut_16,     \
                                                  _ufp8p24explut_256,  x ) )

#define _expfp8p8lut(x)    (x>0 ? _expufp8p8lut(x)   : 1<<8)
#define _expfp24p8lut(x)   (x>0 ? _expufp24p8lut(x)  : 1<<8)
#define _expfp16p16lut(x)  (x>0 ? _expufp16p16lut(x) : 1<<16)
#define _expfp8p24lut(x)   (x>0 ? _expufp8p24lut(x)  : 1<<24)

/* >>> Natural logarithm <<< */

/* ln x or log_e x */
/* f(x)=ln(x) for x>=1
 */

#define __lnufp8p8(a)   (a>=1<<8) ? ( /* x>= 1 */ \
      (         (a<=1<<9)?     0:\
       (a>>=1)&&(a<=1<<9)? 177:(a>>=1)&&(a<=1<<9)? 355:\
       (a>>=1)&&(a<=1<<9)? 532:(a>>=1)&&(a<=1<<9)? 710:\
       (a>>=1)&&(a<=1<<9)? 887:(a>>=1)&&(a<=1<<9)?1065:\
       (a>>=1)&&(a<=1<<9)?1242:(a>>=1)&&(a<=1<<9)?1420:0)\
      +((a<=(1<<8)+(1<< 7))?0:(a=_mulufp8p8(a,171))?104:104)\
      +((a<=(1<<8)+(1<< 6))?0:(a=_mulufp8p8(a,205))?57:57)\
      +((a<=(1<<8)+(1<< 5))?0:(a=_mulufp8p8(a,228))?30:30)\
      +((a<=(1<<8)+(1<< 4))?0:(a=_mulufp8p8(a,241))?16:16)\
      +((a<=(1<<8)+(1<< 3))?0:(a=_mulufp8p8(a,248))?8:8)\
      +((a<=(1<<8)+(1<< 2))?0:(a=_mulufp8p8(a,252))?4:4)\
      +((a<=(1<<8)+(1<< 1))?0:(a=_mulufp8p8(a,254))?2:2)\
      +((a<=(1<<8)+(1<< 0))?0:(a=_mulufp8p8(a,255))?1:1)\
      ) /* done */ \
      : /* x<1 */ itofp8p8(0)

#define __lnufp24p8(a)   (a>=1<<8) ? ( /* x>= 1 */ \
      (         (a<=1<<9)?   0:\
       (a>>=1)&&(a<=1<<9)? 177:(a>>=1)&&(a<=1<<9)? 355:\
       (a>>=1)&&(a<=1<<9)? 532:(a>>=1)&&(a<=1<<9)? 710:\
       (a>>=1)&&(a<=1<<9)? 887:(a>>=1)&&(a<=1<<9)?1065:\
       (a>>=1)&&(a<=1<<9)?1242:(a>>=1)&&(a<=1<<9)?1420:\
       (a>>=1)&&(a<=1<<9)?1597:(a>>=1)&&(a<=1<<9)?1774:\
       (a>>=1)&&(a<=1<<9)?1952:(a>>=1)&&(a<=1<<9)?2129:\
       (a>>=1)&&(a<=1<<9)?2307:(a>>=1)&&(a<=1<<9)?2484:\
       (a>>=1)&&(a<=1<<9)?2662:(a>>=1)&&(a<=1<<9)?2839:\
       (a>>=1)&&(a<=1<<9)?3017:(a>>=1)&&(a<=1<<9)?3194:\
       (a>>=1)&&(a<=1<<9)?3371:(a>>=1)&&(a<=1<<9)?3549:\
       (a>>=1)&&(a<=1<<9)?3726:(a>>=1)&&(a<=1<<9)?3904:\
       (a>>=1)&&(a<=1<<9)?4081:(a>>=1)&&(a<=1<<9)?4259:0)\
      +((a<=(1<<8)+(1<< 7))?0:(a=_mulufp24p8(a,171))?104:104)\
      +((a<=(1<<8)+(1<< 6))?0:(a=_mulufp24p8(a,205))?57:57)\
      +((a<=(1<<8)+(1<< 5))?0:(a=_mulufp24p8(a,228))?30:30)\
      +((a<=(1<<8)+(1<< 4))?0:(a=_mulufp24p8(a,241))?16:16)\
      +((a<=(1<<8)+(1<< 3))?0:(a=_mulufp24p8(a,248))?8:8)\
      +((a<=(1<<8)+(1<< 2))?0:(a=_mulufp24p8(a,252))?4:4)\
      +((a<=(1<<8)+(1<< 1))?0:(a=_mulufp24p8(a,254))?2:2)\
      +((a<=(1<<8)+(1<< 0))?0:(a=_mulufp24p8(a,255))?1:1)\
      ) /* done */ \
      : /* x<1 */ itofp24p8(0)

#define __lnufp16p16(a)   (a>=1<<16) ? ( /* x>= 1 */ \
      (         (a<=1<<17)?     0:\
       (a>>=1)&&(a<=1<<17)? 45426:(a>>=1)&&(a<=1<<17)? 90852:\
       (a>>=1)&&(a<=1<<17)?136278:(a>>=1)&&(a<=1<<17)?181704:\
       (a>>=1)&&(a<=1<<17)?227130:(a>>=1)&&(a<=1<<17)?272557:\
       (a>>=1)&&(a<=1<<17)?317983:(a>>=1)&&(a<=1<<17)?363409:\
       (a>>=1)&&(a<=1<<17)?408835:(a>>=1)&&(a<=1<<17)?454261:\
       (a>>=1)&&(a<=1<<17)?499687:(a>>=1)&&(a<=1<<17)?545113:\
       (a>>=1)&&(a<=1<<17)?590539:(a>>=1)&&(a<=1<<17)?635965:\
       (a>>=1)&&(a<=1<<17)?681391:(a>>=1)&&(a<=1<<17)?726818:0)\
      +((a<=(1<<16)+(1<<15))?0:(a=_mulufp16p16(a,43691))?26573:26573)\
      +((a<=(1<<16)+(1<<14))?0:(a=_mulufp16p16(a,52429))?14624:14624)\
      +((a<=(1<<16)+(1<<13))?0:(a=_mulufp16p16(a,58254))?7719:7719)\
      +((a<=(1<<16)+(1<<12))?0:(a=_mulufp16p16(a,61681))?3973:3973)\
      +((a<=(1<<16)+(1<<11))?0:(a=_mulufp16p16(a,63550))?2017:2017)\
      +((a<=(1<<16)+(1<<10))?0:(a=_mulufp16p16(a,64528))?1016:1016)\
      +((a<=(1<<16)+(1<< 9))?0:(a=_mulufp16p16(a,65028))?510:510)\
      +((a<=(1<<16)+(1<< 8))?0:(a=_mulufp16p16(a,65281))?256:256)\
      +((a<=(1<<16)+(1<< 7))?0:(a=_mulufp16p16(a,65408))?128:128)\
      +((a<=(1<<16)+(1<< 6))?0:(a=_mulufp16p16(a,65472))?64:64)\
      +((a<=(1<<16)+(1<< 5))?0:(a=_mulufp16p16(a,65504))?32:32)\
      +((a<=(1<<16)+(1<< 4))?0:(a=_mulufp16p16(a,65520))?16:16)\
      +((a<=(1<<16)+(1<< 3))?0:(a=_mulufp16p16(a,65528))?8:8)\
      +((a<=(1<<16)+(1<< 2))?0:(a=_mulufp16p16(a,65532))?4:4)\
      +((a<=(1<<16)+(1<< 1))?0:(a=_mulufp16p16(a,65534))?2:2)\
      +((a<=(1<<16)+(1<< 0))?0:(a=_mulufp16p16(a,65535))?1:1)\
      ) /* done */ \
      : /* x<1 */ itofp16p16(0)

#define __lnufp8p24(a)   (a>=1<<24) ? ( /* x>= 1 */ \
      (         (a<=1<<25)?     0:\
       (a>>=1)&&(a<=1<<25)?11629080:(a>>=1)&&(a<=1<<25)?23258160:\
       (a>>=1)&&(a<=1<<25)?34887240:(a>>=1)&&(a<=1<<25)?46516320:\
       (a>>=1)&&(a<=1<<25)?58145400:(a>>=1)&&(a<=1<<25)?69774480:\
       (a>>=1)&&(a<=1<<25)?81403560:(a>>=1)&&(a<=1<<25)?93032640:0)\
      +((a<=(1<<24)+(1<<23))?0:(a=_mulufp8p24(a,11184811))?6802576:6802576)\
      +((a<=(1<<24)+(1<<22))?0:(a=_mulufp8p24(a,13421773))?3743728:3743728)\
      +((a<=(1<<24)+(1<<21))?0:(a=_mulufp8p24(a,14913081))?1976071:1976071)\
      +((a<=(1<<24)+(1<<20))?0:(a=_mulufp8p24(a,15790321))?1017112:1017112)\
      +((a<=(1<<24)+(1<<19))?0:(a=_mulufp8p24(a,16268816))?516263:516263)\
      +((a<=(1<<24)+(1<<18))?0:(a=_mulufp8p24(a,16519105))?260117:260117)\
      +((a<=(1<<24)+(1<<17))?0:(a=_mulufp8p24(a,16647160))?130563:130563)\
      +((a<=(1<<24)+(1<<16))?0:(a=_mulufp8p24(a,16711935))?65408:65408)\
      +((a<=(1<<24)+(1<<15))?0:(a=_mulufp8p24(a,16744512))?32736:32736)\
      +((a<=(1<<24)+(1<<14))?0:(a=_mulufp8p24(a,16760848))?16376:16376)\
      +((a<=(1<<24)+(1<<13))?0:(a=_mulufp8p24(a,16769028))?8190:8190)\
      +((a<=(1<<24)+(1<<12))?0:(a=_mulufp8p24(a,16773121))?4096:4096)\
      +((a<=(1<<24)+(1<<11))?0:(a=_mulufp8p24(a,16775168))?2048:2048)\
      +((a<=(1<<24)+(1<<10))?0:(a=_mulufp8p24(a,16776192))?1024:1024)\
      +((a<=(1<<24)+(1<< 9))?0:(a=_mulufp8p24(a,16776704))?512:512)\
      +((a<=(1<<24)+(1<< 8))?0:(a=_mulufp8p24(a,16776960))?256:256)\
      +((a<=(1<<24)+(1<< 7))?0:(a=_mulufp8p24(a,16777088))?128:128)\
      +((a<=(1<<24)+(1<< 6))?0:(a=_mulufp8p24(a,16777152))?64:64)\
      +((a<=(1<<24)+(1<< 5))?0:(a=_mulufp8p24(a,16777184))?32:32)\
      +((a<=(1<<24)+(1<< 4))?0:(a=_mulufp8p24(a,16777200))?16:16)\
      +((a<=(1<<24)+(1<< 3))?0:(a=_mulufp8p24(a,16777208))?8:8)\
      +((a<=(1<<24)+(1<< 2))?0:(a=_mulufp8p24(a,16777212))?4:4)\
      +((a<=(1<<24)+(1<< 1))?0:(a=_mulufp8p24(a,16777214))?2:2)\
      +((a<=(1<<24)+(1<< 0))?0:(a=_mulufp8p24(a,16777216))?1:1)\
      ) /* done */ \
      : /* x<1 */ itofp8p24(0)

#define _lnufp8p8(a)    (_ufp8p8tmp=a)   ? (__lnufp8p8(_ufp8p8tmp)) \
                                         : (__lnufp8p8(_ufp8p8tmp))
#define _lnufp24p8(a)   (_ufp24p8tmp=a)  ? (__lnufp24p8(_ufp24p8tmp)) \
                                         : (__lnufp24p8(_ufp24p8tmp))
#define _lnufp16p16(a)  (_ufp16p16tmp=a) ? (__lnufp16p16(_ufp16p16tmp)) \
                                         : (__lnufp16p16(_ufp16p16tmp))
#define _lnufp8p24(a)   (_ufp8p24tmp=a)  ? (__lnufp8p24(_ufp8p24tmp)) \
                                         : (__lnufp8p24(_ufp8p24tmp))

#define _lnfp8p8(a)      _lnufp8p8(a)
#define _lnfp24p8(a)     _lnufp24p8(a)
#define _lnfp16p16(a)    _lnufp16p16(a)
#define _lnfp8p24(a)     _lnufp8p24(a)

#define _lnufp8p8lut(x)   ( __genericufp8p8lut  (_ufp8p8lnlut_16,      \
                                                 _ufp8p8lnlut_256,   x ) )
#define _lnufp24p8lut(x)  ( __genericufp24p8lut (_ufp24p8lnlut_16,     \
                                                 _ufp24p8lnlut_256,    \
                                                 _ufp24p8lnlut_64k,    \
                                                 _ufp24p8lnlut_16m,  x ) )
#define _lnufp16p16lut(x) ( __genericufp16p16lut(_ufp16p16lnlut_16,    \
                                                 _ufp16p16lnlut_256,   \
                                                 _ufp16p16lnlut_64k, x ) )
#define _lnufp8p24lut(x)  ( __genericufp8p24lut (_ufp8p24lnlut_4,      \
                                                 _ufp8p24lnlut_16,     \
                                                 _ufp8p24lnlut_256,  x ) )

#define _lnfp8p8lut(x)    (x>(1<<8)  ? _lnufp8p8lut(x)   : 0)
#define _lnfp24p8lut(x)   (x>(1<<8)  ? _lnufp24p8lut(x)  : 0)
#define _lnfp16p16lut(x)  (x>(1<<16) ? _lnufp16p16lut(x) : 0)
#define _lnfp8p24lut(x)   (x>(1<<24) ? _lnufp8p24lut(x)  : 0)

/* >>> Power function <<< */

/* x^y */

#define __powufp8p8(x,y)   ((_ufp8p8tmp2   = _mulufp8p8 (y, lnufp8p8(x)  )) ?\
                                           expufp8p8  (_ufp8p8tmp2) : 1<<8)
#define __powufp24p8(x,y)  ((_ufp24p8tmp2  = _mulufp24p8(y, lnufp24p8(x)  )) ?\
                                           expufp24p8 (_ufp24p8tmp2) : 1<<8)
#define __powufp16p16(x,y) ((_ufp16p16tmp2 = _mulufp16p16(y, lnufp16p16(x) ))?\
                                           expufp16p16 (_ufp16p16tmp2) : 1<<16)
#define __powufp8p24(x,y)  ((_ufp8p24tmp2  = _mulufp8p24 (y, lnufp8p24(x)  ))?\
                                           expufp8p24  (_ufp8p24tmp2) : 1<<24)

#define _powufp8p8(x,y)   ( x>(1<<8)  ? __powufp8p8(x,y)   : 1<<8 )
#define _powufp24p8(x,y)  ( x>(1<<8)  ? __powufp24p8(x,y)  : 1<<8 )
#define _powufp16p16(x,y) ( x>(1<<16) ? __powufp16p16(x,y) : 1<<16 )
#define _powufp8p24(x,y)  ( x>(1<<24) ? __powufp8p24(x,y)  : 1<<24 )

#define _powfp8p8(x,y)   _powufp8p8(x,y)
#define _powfp24p8(x,y)  _powufp24p8(x,y)
#define _powfp16p16(x,y) _powufp16p16(x,y)
#define _powfp8p24(x,y)  _powufp8p24(x,y)

/* ------------------------------------------------------------------------- */

/* Generic LUT macro's, used for sqrt, exp and ln in LUT mode 
 *
 * Use linear interpolation for values that don't map exactly
 */

#define __genericufp8p8lut(lut16, lut256, x) \
            (x<16<<8)    ? lut16[x>>5] +                              \
                           _mulufp8p8( ((x<<3)&0x0ff),                 \
                                      (lut16[(x>>5)+1] - lut16[x>>5]) \
                                    ) \
                         : lut256[((x>>8)-16)>>1] + \
                           _mulufp8p8( ((x>>1)&0x0ff),       \
                                      (lut256[(((x>>8)-16)>>1)+1] - \
                                       lut256[ ((x>>8)-16)>>1   ])  \
                                    )

#define __genericufp24p8lut(lut16, lut256, lut64k, lut16m, x) \
            (x<16<<8)    ? lut16[x>>5] + \
                           _mulufp24p8( ((x<<3)&0x0ff), \
                                       (lut16[(x>>5)+1] - lut16[x>>5]) \
                                     ) \
          : (x<256<<8)   ? lut256[((x>>8)-16)>>1] +\
                           _mulufp24p8( ((x>>1)&0x0ff), \
                                       (lut256[(((x>>8)-16)>>1)+1] - \
                                        lut256[ ((x>>8)-16)>>1   ])  \
                                     ) \
          : (x<65536<<8) ? lut64k[((x>>8)-256)>>9] + \
                           _mulufp24p8( (((x-(256<<8))>>9)&0x0ff), \
                                       (lut64k[(((x>>8)-256)>>9)+1] - \
                                        lut64k[ ((x>>8)-256)>>9   ])  \
                                     ) \
                         : lut16m[((x>>8)-65536)>>17] + \
                           _mulufp24p8( (((x-(65536<<8))>>17)&0x0ff), \
                                       (lut16m[(((x>>8)-65536)>>17)+1] - \
                                        lut16m[ ((x>>8)-65536)>>17   ])  \
                                     )

#define __genericufp16p16lut(lut16, lut256, lut64k, x) \
            (x<16<<16)   ? lut16[x>>13] + \
                           _mulufp16p16( ((x<<3)&0x0ffff),     \
                                        (lut16[(x>>13)+1] -   \
                                         lut16[(x>>13)  ]) \
                                       ) \
          : (x<256<<16)  ? lut256[((x>>16)-16)>>1] + \
                           _mulufp16p16(  ((x>>1)&0x0ffff), \
                                         (lut256[(((x>>16)-16)>>1)+1] - \
                                          lut256[ ((x>>16)-16)>>1   ]) \
                                       ) \
                         : lut64k[((x>>16)-256)>>9] + \
                           _mulufp16p16(  (((x-(256<<16))>>9)&0x0ffff), \
                                         (lut64k[(((x>>16)-256)>>9)+1] - \
                                          lut64k[ ((x>>16)-256)>>9   ]) \
                                       )

#define __genericufp8p24lut(lut4, lut16, lut256, x) \
            (x<4<<24)    ? lut4[x>>19]  + \
                           _mulufp8p24( ((x<<5)&0x0ffffff),     \
                                       (lut4[(x>>19)+1] -       \
                                        lut4[(x>>19)  ])        \
                                      ) \
          : (x<16<<24)   ? lut16[x>>21] + \
                           _mulufp8p24( ((x<<3)&0x0ffffff),     \
                                       (lut16[(x>>21)+1] -      \
                                        lut16[(x>>21)  ])       \
                                      ) \
                         : lut256[((x>>24)-16)>>1] + \
                           _mulufp8p24( ((x>>1)&0x0ffffff),     \
                                       (lut256[(((x>>24)-16)>>1)+1] - \
                                        lut256[(((x>>24)-16)>>1)  ]) \
                                      )

/* ------------------------------------------------------------------------- */

/* MAKE COMPILER WARNINGS GO AWAY */

/* This makes sure the compiler doesn't warn you about blabla defined but
 * not used. It does add a little code to your program though. If you don't
 * like that and really care about code size (maybe for embedded systems for
 * example), you can define __FPM_LEAVE_WARNINGS__ before including this file,
 * although stripping the binary will remove this code on most systems anyway.
 *
 * Funny though, ICC starts complaining about a function that's never
 * called :-) But ICC complains about anything, like "warning: '=' where
 * '==' may have been intended". MAY HAVE BEEN??? STFU!
 */

/* Not sure if I keep this "function". About every compiler either complains
 * about unused variables or about a function that never gets called,
 * depending on the optimization level and if this function is here or not.
 * Maybe change it into some "init" function that has to be called by
 * the program that uses this macro lib?? Have to think about it.
 */

#define __FPM_LEAVE_WARNINGS___
#ifndef __FPM_LEAVE_WARNINGS___
static void __fixed_point_math_make_compiler_warnings_go_away(void)
{
    _sqrtufp8p8(1);
    _sqrtufp24p8(1);
    _sqrtufp16p16(1);
    _sqrtufp8p24(1);
    _expufp8p8(1);
    _expufp24p8(1);
}
#endif

/* ------------------------------------------------------------------------- */

#endif

/* ------------------------------------------------------------------------- */

