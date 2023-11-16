# RPI_PICO_TimerInterrupt Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/RPI_PICO_TimerInterrupt.svg?)](https://www.ardu-badge.com/RPI_PICO_TimerInterrupt)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/RPI_PICO_TimerInterrupt.svg)](https://github.com/khoih-prog/RPI_PICO_TimerInterrupt/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/RPI_PICO_TimerInterrupt/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/RPI_PICO_TimerInterrupt.svg)](http://github.com/khoih-prog/RPI_PICO_TimerInterrupt/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.3.1](#releases-v131)
  * [Releases v1.3.0](#releases-v130)
  * [Releases v1.2.0](#releases-v120)
  * [Releases v1.1.1](#releases-v111)
  * [Releases v1.1.0](#releases-v110)
  * [Releases v1.0.1](#releases-v101)
  * [Initial Releases v1.0.0](#initial-releases-v100)


---
---

## Changelog

### Releases v1.3.1

1. Using `float` instead of `ulong` for better interval accuracy

### Releases v1.3.0

1. Fix severe bug affecting time between the starts. Check [Enable fixed timing between timer calls (vs fixed time btw. end of timer call and next call as implemented) #3](https://github.com/khoih-prog/RPI_PICO_TimerInterrupt/issues/3)


### Releases v1.2.0

1. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories
2. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project.
3. Optimize library code by using `reference-passing` instead of `value-passing`
4. Update all examples

### Releases v1.1.1

1. Fix platform in `library.json`

### Releases v1.1.0

1. Add support to new boards (**ADAFRUIT_ITSYBITSY_RP2040, ADAFRUIT_QTPY_RP2040, ADAFRUIT_STEMMAFRIEND_RP2040, ADAFRUIT_TRINKEYQT_RP2040, ADAFRUIT_MACROPAD_RP2040, SPARKFUN_PROMICRO_RP2040, etc.**) using the arduino-pico core
2. Add `ISR_16_Timers_Array_Complex` examples.
3. Fix examples' bug

### Releases v1.0.1

1. Update README and Packages' Patches to match core arduino-pico core v1.4.0

### Initial Releases v1.0.0

1. Initial coding to support RP2040-based boards such as **RASPBERRY_PI_PICO**, etc. using [**Earle Philhower's arduino-pico** core](https://github.com/earlephilhower/arduino-pico)



