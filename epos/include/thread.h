// EPOS Thread Abstraction Declarations

#ifndef __thread_h
#define __thread_h

#include <utility/queue.h>
#include <cpu.h>
#include <machine.h>
#include <system/kmalloc.h>

__BEGIN_SYS

class Thread
{
    friend class Init_First;
    friend class Synchronizer_Common;
    friend class Alarm;

protected:
    static const bool preemptive = Traits<Thread>::preemptive;
    static const bool reboot = Traits<System>::reboot;

    static const unsigned int QUANTUM = Traits<Thread>::QUANTUM;
    static const unsigned int STACK_SIZE = Traits<Application>::STACK_SIZE;

    typedef CPU::Log_Addr Log_Addr;
    typedef CPU::Context Context;

public:
    // Thread State
    enum State {
        RUNNING,
        READY,
        SUSPENDED,
        WAITING,
        FINISHING
    };

    // Thread Priority
    typedef unsigned int Priority;
    enum {
        HIGH = 0,
        NORMAL = 15,
        LOW = 31
    };

    // Thread Queue
    typedef Ordered_Queue<Thread, Priority> Queue;

public:
    Thread(int (* entry)(), 
           const State & state = READY, const Priority & priority = NORMAL, unsigned int stack_size = STACK_SIZE)
    : _state(state), _link(this, priority)
    {
        lock();

        _stack = kmalloc(stack_size);
        _context = CPU::init_stack(_stack, stack_size, &implicit_exit, entry);

        common_constructor(entry, stack_size); // implicit unlock
    }

    template<typename T1>
    Thread(int (* entry)(T1 a1), T1 a1,
           const State & state = READY, const Priority & priority = NORMAL, unsigned int stack_size = STACK_SIZE)
    : _state(state), _link(this, priority)
    {
        lock();

        _stack = kmalloc(stack_size);
        _context = CPU::init_stack(_stack, stack_size, &implicit_exit, entry, a1);

        common_constructor(entry, stack_size); // implicit unlock()
    }

    template<typename T1, typename T2>
    Thread(int (* entry)(T1 a1, T2 a2), T1 a1, T2 a2,
           const State & state = READY, const Priority & priority = NORMAL, unsigned int stack_size = STACK_SIZE)
    : _state(state), _link(this, priority)
    {
        lock();

        _stack = kmalloc(stack_size);
        _context = CPU::init_stack(_stack, stack_size, &implicit_exit, entry, a1, a2);

        common_constructor(entry, stack_size); // implicit unlock()
    }

    template<typename T1, typename T2, typename T3>
    Thread(int (* entry)(T1 a1, T2 a2, T3 a3), T1 a1, T2 a2, T3 a3,
           const State & state = READY, const Priority & priority = NORMAL, unsigned int stack_size = STACK_SIZE)
    : _state(state), _link(this, priority)
    {
        lock();

        _stack = kmalloc(stack_size);
        _context = CPU::init_stack(_stack, stack_size, &implicit_exit, entry, a1, a2, a3);

        common_constructor(entry, stack_size); // implicit unlock()
    }

    ~Thread();

    const volatile State & state() const { return _state; }

    const volatile Priority  & priority() const { return _link.rank(); }
    void priority(const Priority & p);

    int join();
    void pass();
    void suspend();
    void resume();

    static Thread * volatile self() { return running(); }
    static void yield();
    static void exit(int status = 0);

protected:
    void common_constructor(Log_Addr entry, unsigned int stack_size);

    static Thread * volatile running() { return _running; }

    static void lock() { CPU::int_disable(); }
    static void unlock() { CPU::int_enable(); }

    static void reschedule();

    static void implicit_exit();

    static void dispatch(Thread * prev, Thread * next) {
        if(prev != next) {
            db<Thread>(TRC) << "Thread::dispatch(prev=" << prev << ",next=" << next << ")" << endl;
            db<Thread>(INF) << "prev={" << prev << ",ctx=" << *prev->_context << "}" << endl;
            db<Thread>(INF) << "next={" << next << ",ctx=" << *next->_context << "}" << endl;

            CPU::switch_context(&prev->_context, next->_context);
        }

        CPU::int_enable();
    }

    static int idle();

private:
    static void init();

protected:
    Log_Addr _stack;
    Context * volatile _context;
    volatile State _state;
    Queue::Element _link;

    static Scheduler_Timer * _timer;

private:
    static Thread * volatile _running;
    static Queue _ready;
    static Queue _suspended;
};

__END_SYS

#endif
