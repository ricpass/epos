// EPOS Synchronizer Abstractions Common Package

#ifndef __synchronizer_h
#define __synchronizer_h

#include <cpu.h>
#include <thread.h>
#include <utility/list.h>


__BEGIN_SYS

class Synchronizer_Common
{
protected:

public:
    Synchronizer_Common() {}
    
    // Atomic operations
    bool tsl(volatile bool & lock) { return CPU::tsl(lock); }
    int finc(volatile int & number) { return CPU::finc(number); }
    int fdec(volatile int & number) { return CPU::fdec(number); }
		
    // Thread operations
    void begin_atomic() { Thread::lock(); }
    void end_atomic() { Thread::unlock(); }

    void sleep()
    {
	    begin_atomic();

 			Thread* running = Thread::running();

		  _sleeping.insert(&running->_link);
		  running->suspend();
	    
		  end_atomic();
    }
    
    void wakeup() 
    {
    	begin_atomic();
      
			if(!_sleeping.empty())
  			_sleeping.remove()->object()->resume(); // end_atomic
  		else   	
		 		end_atomic(); 
    }
    
    void wakeup_all() 
    {
    	begin_atomic();
     
			while(!_sleeping.empty()) {
  			_sleeping.remove()->object()->resume(); // end_atomic
  			begin_atomic();
  		}
      	
		 end_atomic();
    }
    
    Thread::Queue _sleeping;
};


__END_SYS

#endif
