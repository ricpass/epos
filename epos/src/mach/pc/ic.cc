
#include <mach/pc/ic.h>
#include <machine.h>

extern "C" { void _exit(int s); }

__BEGIN_SYS

// Class attributes
APIC::Log_Addr APIC::_base;
PC_IC::Interrupt_Handler PC_IC::_int_vector[PC_IC::INTS];

// Class methods
void PC_IC::int_not(unsigned int i)
{
    db<PC>(WRN) << "\nInt " << i << " occurred, but no handler installed\n";
}

void PC_IC::exc_not(unsigned int i,
        	    Reg32 error, Reg32 eip, Reg32 cs, Reg32 eflags)
{
    db<PC>(WRN) << "\nAn exception has occurred for which no handler"
                << " is installed [err=" << (void *)error
                << ",ctx={cs=" << (void *)cs
                << ",ip=" << (void *)eip
                << ",fl=" << (void *)eflags
                << "}]\n";

    db<PC>(WRN) << "The running thread will now be terminated!" << endl;
    _exit(-1);
}

void PC_IC::exc_pf(unsigned int i,
        	   Reg32 error, Reg32 eip, Reg32 cs, Reg32 eflags)
{  
    db<PC>(WRN) << "\nPage fault [address=" << (void *)CPU::cr2()
        	        << ",err={";
    if(error & (1 << 0))
        db<PC>(WRN) << "P";
    if(error & (1 << 1))
        db<PC>(WRN) << "W";
    if(error & (1 << 2))
        db<PC>(WRN) << "S";
    if(error & (1 << 3))
        db<PC>(WRN) << "R";
    db<PC>(WRN) << "},ctx={cs=" << (void *)cs
                << ",ip=" << (void *)eip
                << ",fl=" << (void *)eflags
                << "}]\n";

    db<PC>(WRN) << "The running thread will now be terminated!" << endl;
    _exit(-1);
}

void PC_IC::exc_gpf(unsigned int i,
        	    Reg32 error, Reg32 eip, Reg32 cs, Reg32 eflags)
{  
    db<PC>(WRN) << "\nGeneral protection fault [err=" << (void *)error
        	        << ",ctx={cs=" << (void *)cs
        	        << ",ip=" << (void *)eip
        	        << ",fl=" << (void *)eflags
        	        << "}]\n";

    db<PC>(WRN) << "The running thread will now be terminated!" << endl;
    _exit(-1);
}

void PC_IC::exc_fpu(unsigned int i,
                    Reg32 error, Reg32 eip, Reg32 cs, Reg32 eflags)
{
    db<PC>(WRN) << "\nFPU fault [err=" << (void *)error
        	        << ",ctx={cs=" << (void *)cs
        	        << ",ip=" << (void *)eip
        	        << ",fl=" << (void *)eflags
        	        << "}]\n";

    db<PC>(WRN) << "The running thread will now be terminated!" << endl;
    _exit(-1);
}

// APIC class methods
void APIC::ipi_init(volatile int * status)
{
//    // Broadcast INIT IPI to all APs excluding self
//    write(ICR0_31, ICR_OTHERS | ICR_LEVEL | ICR_ASSERT | ICR_INIT);
//    while((read(ICR0_31) & ICR_PENDING));

    status[0] = 0;

    // Send INIT IPI to all APs excluding self
    for(unsigned int n = 1; n < Traits<PC>::MAX_CPUS; n++) {
        status[n] = 0;
        write(ICR32_63, n << 24);
        write(ICR0_31, ICR_LEVEL | ICR_ASSERT | ICR_INIT);
        while((read(ICR0_31) & ICR_PENDING));
    }

    i8255::ms_delay(100);
};

void APIC::ipi_start(Log_Addr entry, volatile int * status)
{
//    unsigned int vector = (entry >> 12) & 0xff;
//
//    // Broadcast STARTUP IPI to all APs excluding self twice
//    write(ICR0_31, ICR_OTHERS | ICR_LEVEL | ICR_ASSERT | ICR_STARTUP | vector);
//    while((read(ICR0_31) & ICR_PENDING));
//
//    i8255::ms_delay(10); // ~ 10ms delay
//
//    write(ICR0_31, ICR_OTHERS | ICR_LEVEL | ICR_ASSERT | ICR_STARTUP | vector);
//    while((read(ICR0_31) & ICR_PENDING));
//
//    // Give other CPUs a time to wake up (> 100ms)
//    i8255::ms_delay(100);

    unsigned int vector = (entry >> 12) & 0xff;

    // Send STARTUP IPI to all APs
    for(unsigned int n = 1; n < Traits<PC>::MAX_CPUS; n++) {
        write(ICR32_63, n << 24);
        write(ICR0_31, ICR_LEVEL | ICR_ASSERT | ICR_STARTUP | vector);
        while((read(ICR0_31) & ICR_PENDING));
    }

    // Give them time to wake up (> 100ms)
    i8255::ms_delay(500);

    for(unsigned int n = 1; n < Traits<PC>::MAX_CPUS; n++) {
        if(status[n] == 1) { // CPU is up
            CPU::finc(status[n]);
            i8255::ms_delay(30);
        } else { // send a second STARTUP IPI to the CPU
            write(ICR32_63, n << 24);
            write(ICR0_31, ICR_LEVEL | ICR_ASSERT | ICR_STARTUP | vector);
            i8255::ms_delay(500);
            if(status[n] == 1) // CPU is up
                CPU::finc(status[n]);
            // else CPU was not initialized
        }
    }
}

void APIC::ipi_send(unsigned int cpu, unsigned int interrupt)
{
    write(ICR32_63, (cpu << 24));
    write(ICR0_31, ICR_LEVEL | ICR_ASSERT | ICR_FIXED | interrupt);
    while((read(ICR0_31) & ICR_PENDING));
}

__END_SYS
