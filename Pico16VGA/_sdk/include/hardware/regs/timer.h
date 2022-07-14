/**
 * Copyright (c) 2021 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
// =============================================================================
// Register block : TIMER
// Version        : 1
// Bus type       : apb
// Description    : Controls time and alarms
//                  time is a 64 bit value indicating the time in usec since
//                  power-on
//                  timeh is the top 32 bits of time & timel is the bottom 32
//                  bits
//                  to change time write to timelw before timehw
//                  to read time read from timelr before timehr
//                  An alarm is set by setting alarm_enable and writing to the
//                  corresponding alarm register
//                  When an alarm is pending, the corresponding alarm_running
//                  signal will be high
//                  An alarm can be cancelled before it has finished by clearing
//                  the alarm_enable
//                  When an alarm fires, the corresponding alarm_irq is set and
//                  alarm_running is cleared
//                  To clear the interrupt write a 1 to the corresponding
//                  alarm_irq
// =============================================================================
#ifndef HARDWARE_REGS_TIMER_DEFINED
#define HARDWARE_REGS_TIMER_DEFINED
// =============================================================================
// Register    : TIMER_TIMEHW
// Description : Write to bits 63:32 of time
//               always write timelw before timehw
#define TIMER_TIMEHW_OFFSET 0x00000000
#define TIMER_TIMEHW_BITS   0xffffffff
#define TIMER_TIMEHW_RESET  0x00000000
#define TIMER_TIMEHW_MSB    31
#define TIMER_TIMEHW_LSB    0
#define TIMER_TIMEHW_ACCESS "WF"
// =============================================================================
// Register    : TIMER_TIMELW
// Description : Write to bits 31:0 of time
//               writes do not get copied to time until timehw is written
#define TIMER_TIMELW_OFFSET 0x00000004
#define TIMER_TIMELW_BITS   0xffffffff
#define TIMER_TIMELW_RESET  0x00000000
#define TIMER_TIMELW_MSB    31
#define TIMER_TIMELW_LSB    0
#define TIMER_TIMELW_ACCESS "WF"
// =============================================================================
// Register    : TIMER_TIMEHR
// Description : Read from bits 63:32 of time
//               always read timelr before timehr
#define TIMER_TIMEHR_OFFSET 0x00000008
#define TIMER_TIMEHR_BITS   0xffffffff
#define TIMER_TIMEHR_RESET  0x00000000
#define TIMER_TIMEHR_MSB    31
#define TIMER_TIMEHR_LSB    0
#define TIMER_TIMEHR_ACCESS "RO"
// =============================================================================
// Register    : TIMER_TIMELR
// Description : Read from bits 31:0 of time
#define TIMER_TIMELR_OFFSET 0x0000000c
#define TIMER_TIMELR_BITS   0xffffffff
#define TIMER_TIMELR_RESET  0x00000000
#define TIMER_TIMELR_MSB    31
#define TIMER_TIMELR_LSB    0
#define TIMER_TIMELR_ACCESS "RO"
// =============================================================================
// Register    : TIMER_ALARM0
// Description : Arm alarm 0, and configure the time it will fire.
//               Once armed, the alarm fires when TIMER_ALARM0 == TIMELR.
//               The alarm will disarm itself once it fires, and can
//               be disarmed early using the ARMED status register.
#define TIMER_ALARM0_OFFSET 0x00000010
#define TIMER_ALARM0_BITS   0xffffffff
#define TIMER_ALARM0_RESET  0x00000000
#define TIMER_ALARM0_MSB    31
#define TIMER_ALARM0_LSB    0
#define TIMER_ALARM0_ACCESS "RW"
// =============================================================================
// Register    : TIMER_ALARM1
// Description : Arm alarm 1, and configure the time it will fire.
//               Once armed, the alarm fires when TIMER_ALARM1 == TIMELR.
//               The alarm will disarm itself once it fires, and can
//               be disarmed early using the ARMED status register.
#define TIMER_ALARM1_OFFSET 0x00000014
#define TIMER_ALARM1_BITS   0xffffffff
#define TIMER_ALARM1_RESET  0x00000000
#define TIMER_ALARM1_MSB    31
#define TIMER_ALARM1_LSB    0
#define TIMER_ALARM1_ACCESS "RW"
// =============================================================================
// Register    : TIMER_ALARM2
// Description : Arm alarm 2, and configure the time it will fire.
//               Once armed, the alarm fires when TIMER_ALARM2 == TIMELR.
//               The alarm will disarm itself once it fires, and can
//               be disarmed early using the ARMED status register.
#define TIMER_ALARM2_OFFSET 0x00000018
#define TIMER_ALARM2_BITS   0xffffffff
#define TIMER_ALARM2_RESET  0x00000000
#define TIMER_ALARM2_MSB    31
#define TIMER_ALARM2_LSB    0
#define TIMER_ALARM2_ACCESS "RW"
// =============================================================================
// Register    : TIMER_ALARM3
// Description : Arm alarm 3, and configure the time it will fire.
//               Once armed, the alarm fires when TIMER_ALARM3 == TIMELR.
//               The alarm will disarm itself once it fires, and can
//               be disarmed early using the ARMED status register.
#define TIMER_ALARM3_OFFSET 0x0000001c
#define TIMER_ALARM3_BITS   0xffffffff
#define TIMER_ALARM3_RESET  0x00000000
#define TIMER_ALARM3_MSB    31
#define TIMER_ALARM3_LSB    0
#define TIMER_ALARM3_ACCESS "RW"
// =============================================================================
// Register    : TIMER_ARMED
// Description : Indicates the armed/disarmed status of each alarm.
//               A write to the corresponding ALARMx register arms the alarm.
//               Alarms automatically disarm upon firing, but writing ones here
//               will disarm immediately without waiting to fire.
#define TIMER_ARMED_OFFSET 0x00000020
#define TIMER_ARMED_BITS   0x0000000f
#define TIMER_ARMED_RESET  0x00000000
#define TIMER_ARMED_MSB    3
#define TIMER_ARMED_LSB    0
#define TIMER_ARMED_ACCESS "WC"
// =============================================================================
// Register    : TIMER_TIMERAWH
// Description : Raw read from bits 63:32 of time (no side effects)
#define TIMER_TIMERAWH_OFFSET 0x00000024
#define TIMER_TIMERAWH_BITS   0xffffffff
#define TIMER_TIMERAWH_RESET  0x00000000
#define TIMER_TIMERAWH_MSB    31
#define TIMER_TIMERAWH_LSB    0
#define TIMER_TIMERAWH_ACCESS "RO"
// =============================================================================
// Register    : TIMER_TIMERAWL
// Description : Raw read from bits 31:0 of time (no side effects)
#define TIMER_TIMERAWL_OFFSET 0x00000028
#define TIMER_TIMERAWL_BITS   0xffffffff
#define TIMER_TIMERAWL_RESET  0x00000000
#define TIMER_TIMERAWL_MSB    31
#define TIMER_TIMERAWL_LSB    0
#define TIMER_TIMERAWL_ACCESS "RO"
// =============================================================================
// Register    : TIMER_DBGPAUSE
// Description : Set bits high to enable pause when the corresponding debug
//               ports are active
#define TIMER_DBGPAUSE_OFFSET 0x0000002c
#define TIMER_DBGPAUSE_BITS   0x00000006
#define TIMER_DBGPAUSE_RESET  0x00000007
// -----------------------------------------------------------------------------
// Field       : TIMER_DBGPAUSE_DBG1
// Description : Pause when processor 1 is in debug mode
#define TIMER_DBGPAUSE_DBG1_RESET  0x1
#define TIMER_DBGPAUSE_DBG1_BITS   0x00000004
#define TIMER_DBGPAUSE_DBG1_MSB    2
#define TIMER_DBGPAUSE_DBG1_LSB    2
#define TIMER_DBGPAUSE_DBG1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_DBGPAUSE_DBG0
// Description : Pause when processor 0 is in debug mode
#define TIMER_DBGPAUSE_DBG0_RESET  0x1
#define TIMER_DBGPAUSE_DBG0_BITS   0x00000002
#define TIMER_DBGPAUSE_DBG0_MSB    1
#define TIMER_DBGPAUSE_DBG0_LSB    1
#define TIMER_DBGPAUSE_DBG0_ACCESS "RW"
// =============================================================================
// Register    : TIMER_PAUSE
// Description : Set high to pause the timer
#define TIMER_PAUSE_OFFSET 0x00000030
#define TIMER_PAUSE_BITS   0x00000001
#define TIMER_PAUSE_RESET  0x00000000
#define TIMER_PAUSE_MSB    0
#define TIMER_PAUSE_LSB    0
#define TIMER_PAUSE_ACCESS "RW"
// =============================================================================
// Register    : TIMER_INTR
// Description : Raw Interrupts
#define TIMER_INTR_OFFSET 0x00000034
#define TIMER_INTR_BITS   0x0000000f
#define TIMER_INTR_RESET  0x00000000
// -----------------------------------------------------------------------------
// Field       : TIMER_INTR_ALARM_3
// Description : None
#define TIMER_INTR_ALARM_3_RESET  0x0
#define TIMER_INTR_ALARM_3_BITS   0x00000008
#define TIMER_INTR_ALARM_3_MSB    3
#define TIMER_INTR_ALARM_3_LSB    3
#define TIMER_INTR_ALARM_3_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTR_ALARM_2
// Description : None
#define TIMER_INTR_ALARM_2_RESET  0x0
#define TIMER_INTR_ALARM_2_BITS   0x00000004
#define TIMER_INTR_ALARM_2_MSB    2
#define TIMER_INTR_ALARM_2_LSB    2
#define TIMER_INTR_ALARM_2_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTR_ALARM_1
// Description : None
#define TIMER_INTR_ALARM_1_RESET  0x0
#define TIMER_INTR_ALARM_1_BITS   0x00000002
#define TIMER_INTR_ALARM_1_MSB    1
#define TIMER_INTR_ALARM_1_LSB    1
#define TIMER_INTR_ALARM_1_ACCESS "WC"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTR_ALARM_0
// Description : None
#define TIMER_INTR_ALARM_0_RESET  0x0
#define TIMER_INTR_ALARM_0_BITS   0x00000001
#define TIMER_INTR_ALARM_0_MSB    0
#define TIMER_INTR_ALARM_0_LSB    0
#define TIMER_INTR_ALARM_0_ACCESS "WC"
// =============================================================================
// Register    : TIMER_INTE
// Description : Interrupt Enable
#define TIMER_INTE_OFFSET 0x00000038
#define TIMER_INTE_BITS   0x0000000f
#define TIMER_INTE_RESET  0x00000000
// -----------------------------------------------------------------------------
// Field       : TIMER_INTE_ALARM_3
// Description : None
#define TIMER_INTE_ALARM_3_RESET  0x0
#define TIMER_INTE_ALARM_3_BITS   0x00000008
#define TIMER_INTE_ALARM_3_MSB    3
#define TIMER_INTE_ALARM_3_LSB    3
#define TIMER_INTE_ALARM_3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTE_ALARM_2
// Description : None
#define TIMER_INTE_ALARM_2_RESET  0x0
#define TIMER_INTE_ALARM_2_BITS   0x00000004
#define TIMER_INTE_ALARM_2_MSB    2
#define TIMER_INTE_ALARM_2_LSB    2
#define TIMER_INTE_ALARM_2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTE_ALARM_1
// Description : None
#define TIMER_INTE_ALARM_1_RESET  0x0
#define TIMER_INTE_ALARM_1_BITS   0x00000002
#define TIMER_INTE_ALARM_1_MSB    1
#define TIMER_INTE_ALARM_1_LSB    1
#define TIMER_INTE_ALARM_1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTE_ALARM_0
// Description : None
#define TIMER_INTE_ALARM_0_RESET  0x0
#define TIMER_INTE_ALARM_0_BITS   0x00000001
#define TIMER_INTE_ALARM_0_MSB    0
#define TIMER_INTE_ALARM_0_LSB    0
#define TIMER_INTE_ALARM_0_ACCESS "RW"
// =============================================================================
// Register    : TIMER_INTF
// Description : Interrupt Force
#define TIMER_INTF_OFFSET 0x0000003c
#define TIMER_INTF_BITS   0x0000000f
#define TIMER_INTF_RESET  0x00000000
// -----------------------------------------------------------------------------
// Field       : TIMER_INTF_ALARM_3
// Description : None
#define TIMER_INTF_ALARM_3_RESET  0x0
#define TIMER_INTF_ALARM_3_BITS   0x00000008
#define TIMER_INTF_ALARM_3_MSB    3
#define TIMER_INTF_ALARM_3_LSB    3
#define TIMER_INTF_ALARM_3_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTF_ALARM_2
// Description : None
#define TIMER_INTF_ALARM_2_RESET  0x0
#define TIMER_INTF_ALARM_2_BITS   0x00000004
#define TIMER_INTF_ALARM_2_MSB    2
#define TIMER_INTF_ALARM_2_LSB    2
#define TIMER_INTF_ALARM_2_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTF_ALARM_1
// Description : None
#define TIMER_INTF_ALARM_1_RESET  0x0
#define TIMER_INTF_ALARM_1_BITS   0x00000002
#define TIMER_INTF_ALARM_1_MSB    1
#define TIMER_INTF_ALARM_1_LSB    1
#define TIMER_INTF_ALARM_1_ACCESS "RW"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTF_ALARM_0
// Description : None
#define TIMER_INTF_ALARM_0_RESET  0x0
#define TIMER_INTF_ALARM_0_BITS   0x00000001
#define TIMER_INTF_ALARM_0_MSB    0
#define TIMER_INTF_ALARM_0_LSB    0
#define TIMER_INTF_ALARM_0_ACCESS "RW"
// =============================================================================
// Register    : TIMER_INTS
// Description : Interrupt status after masking & forcing
#define TIMER_INTS_OFFSET 0x00000040
#define TIMER_INTS_BITS   0x0000000f
#define TIMER_INTS_RESET  0x00000000
// -----------------------------------------------------------------------------
// Field       : TIMER_INTS_ALARM_3
// Description : None
#define TIMER_INTS_ALARM_3_RESET  0x0
#define TIMER_INTS_ALARM_3_BITS   0x00000008
#define TIMER_INTS_ALARM_3_MSB    3
#define TIMER_INTS_ALARM_3_LSB    3
#define TIMER_INTS_ALARM_3_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTS_ALARM_2
// Description : None
#define TIMER_INTS_ALARM_2_RESET  0x0
#define TIMER_INTS_ALARM_2_BITS   0x00000004
#define TIMER_INTS_ALARM_2_MSB    2
#define TIMER_INTS_ALARM_2_LSB    2
#define TIMER_INTS_ALARM_2_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTS_ALARM_1
// Description : None
#define TIMER_INTS_ALARM_1_RESET  0x0
#define TIMER_INTS_ALARM_1_BITS   0x00000002
#define TIMER_INTS_ALARM_1_MSB    1
#define TIMER_INTS_ALARM_1_LSB    1
#define TIMER_INTS_ALARM_1_ACCESS "RO"
// -----------------------------------------------------------------------------
// Field       : TIMER_INTS_ALARM_0
// Description : None
#define TIMER_INTS_ALARM_0_RESET  0x0
#define TIMER_INTS_ALARM_0_BITS   0x00000001
#define TIMER_INTS_ALARM_0_MSB    0
#define TIMER_INTS_ALARM_0_LSB    0
#define TIMER_INTS_ALARM_0_ACCESS "RO"
// =============================================================================
#endif // HARDWARE_REGS_TIMER_DEFINED
