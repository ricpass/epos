// EPOS Configuration Engine

#ifndef __config_h
#define __config_h

//============================================================================
// DEFINITIONS
//============================================================================
#define __SYS_NS            EPOS
#define __BEGIN_SYS         namespace __SYS_NS {
#define __END_SYS           }
#define __USING_SYS         using namespace __SYS_NS;
#define __SYS(X)            ::__SYS_NS::X

#define ASM                 __asm__
#define ASMV                __asm__ __volatile__

#define __HEADER_ARCH(X)	    <arch/ARCH/X.h>
#define __HEADER_MACH(X)	    <mach/MACH/X.h>
#define __HEADER_APPLICATION_T(X)   <../app/X##_traits.h>
#define __HEADER_APPLICATION(X)     __HEADER_APPLICATION_T(X)

//============================================================================
// ARCHITECTURE, MACHINE, AND APPLICATION SELECTION
// This section is generated automatically from makedefs
//============================================================================
#define ARCH ia32
#define __ARCH_TRAITS_H	        __HEADER_ARCH(traits)

#define MACH pc
#define __MACH_TRAITS_H	        __HEADER_MACH(traits)

#define APPLICATION producer_consumer
#define __APPLICATION_TRAITS_H  __HEADER_APPLICATION(APPLICATION)

//============================================================================
// CONFIGURATION
//============================================================================
#include <system/types.h>
#include <system/meta.h>
#include __APPLICATION_TRAITS_H
#include <system/info.h>

//============================================================================
// THINGS EVERBODY NEEDS
//============================================================================
#include <utility/ostream.h>
#include <utility/debug.h>

#endif
