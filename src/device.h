#ifndef __device_h__
#define __device_h__

extern "C" {
// device libs
#include <MDR32F9Qx_port.h>
#include <MDR32F9Qx_rst_clk.h>

// imports for IntelliSense only (can be removed)
#include "MDR32Fx.h"

// custom libs
#include "mdrlibs/ext_bus_cntrl.h"
#include "mdrlibs/port.h"
#include "mdrlibs/rst_clk.h"
#include "mdrlibs/timer.h"
}

#define BASE_PORTA 0x400A8000
#define BASE_PORTB 0x400B0000
#define BASE_PORTC 0x400B8000
#define BASE_PORTD 0x400C0000
#define BASE_PORTE 0x400C8000

#define PORTA ((_port *)BASE_PORTA)
#define PORTB ((_port *)BASE_PORTB)
#define PORTC ((_port *)BASE_PORTC)
#define PORTD ((_port *)BASE_PORTD)
#define PORTE ((_port *)BASE_PORTE)

#define BASE_RST_CLK 0x40020000
#define RST_CLK ((_rst_clk *)BASE_RST_CLK)

#define BASE_EXT_BUS_CNTRL 0x400F0000
#define EXT_BUS_CNTRL ((_ext_bus_cntrl *)BASE_EXT_BUS_CNTRL)

#define PORTD_BB \
  ((uint32_t *)((0x42000000 + (MDR_PORTD_BASE - 0x40000000) * 32)))
#define PORTC_BB \
  ((uint32_t *)((0x42000000 + (MDR_PORTC_BASE - 0x40000000) * 32)))

#endif
