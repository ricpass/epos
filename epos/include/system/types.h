// EPOS Internal Type Management System

typedef __SIZE_TYPE__ size_t;

#ifndef __types_h
#define __types_h

inline void * operator new(size_t s, void * a) { return a; }
inline void * operator new[](size_t s, void * a) { return a; }

__BEGIN_SYS

// Dummy class for incomplete architectures and machines 
template<int>
class Dummy;

// Utilities
class Debug;
class Lists;
class Spin;
class Heap;

// System parts
class Build;
class Boot;
class Setup;
class Init;
class System;
class Application;

// Hardware Mediators - CPU
class IA32;

// Hardware Mediators - Time-Stamp Counter
class IA32_TSC;

// Hardware Mediators - Memory Management Unit
class IA32_MMU;

// Hardware Mediators - Machine
class PC;

// Hardware Mediators - Bus
class PC_PCI;

// Hardware Mediators - Interrupt Controller
class PC_IC;

// Hardware Mediators - Timer
class PC_Timer;

// Hardware Mediators - Real-time Clock
class PC_RTC;

// Hardware Mediators - EEPROM
class PC_EEPROM;

// Hardware Mediators - UART
class PC_UART;

// Hardware Mediators - Display
class Serial_Display;
class PC_Display;

// Hardware Mediators - NIC
class PC_Ethernet;
class PCNet32;
class C905;
class E100;

// Abstractions	- Process
class Thread;

// Abstractions	- Synchronization
class Synchronizer;
class Mutex;
class Semaphore;
class Condition;

// Abstractions	- Time
class Clock;
class Alarm;
class Chronometer;

// Abstractions - Network
class Ethernet;


// System Components IDs
// The order in this enumeration defines many things in the system (e.g. init)
typedef unsigned int Type_Id;
enum 
{
    CPU_ID,
    TSC_ID,
    MMU_ID,

    MACHINE_ID,
    PCI_ID,
    IC_ID,
    TIMER_ID,
    RTC_ID,
    EEPROM_ID,
    UART_ID,
    DISPLAY_ID,
    NIC_ID,

    THREAD_ID,

    MUTEX_ID,
    SEMAPHORE_ID,
    CONDITION_ID,

    CLOCK_ID,
    ALARM_ID,
    CHRONOMETER_ID,

    UNKNOWN_TYPE_ID,
    LAST_TYPE_ID = UNKNOWN_TYPE_ID - 1
};

__END_SYS

#endif
