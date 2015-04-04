#include <iostream>

//#include "support/alarm.h"
#include "support/signal_slot.hpp"
//#include "support/date_time.h"



class class_receptor_do_nothing : public jle::SignalReceptor
{

public:
    void do_nothing(void) {};
};



void receptor_do_nothing (void)
{

}


int contador = 0;
void count (void)
{
    ++contador;
}



const long REPETITIONS = 10000000L;


int main (int /*argc*/, char** /*argv*/)
{
    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "BUCLE suma entero " << std::endl << std::endl;
    {
	jle::DateTime  start =  jle::dtNowLocal();
	int acc=0;
	for (int i=0; i<REPETITIONS; ++i)
	    acc +=i;
	jle::DateTime end = jle::dtNowLocal();
	std:: cout << acc << std::endl;
	std::cout << "__NR__:  " << "time: " << end-start << std::endl;
	std::cout << "__NR__:  " << "calls/millisecond: " << REPETITIONS / jle::ARE_YOU_SURE_YOU_WANT_GetMillisecsDay(end-start) << std::endl;
    }



    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "signal -> función (emit)" << std::endl << std::endl;
    {
	jle::Signal<>   svoid;

	svoid.connect( receptor_do_nothing );

	jle::DateTime  start =  jle::dtNowLocal();
	for (int i=0; i<REPETITIONS; ++i)
	    svoid.emit();

	jle::DateTime end = jle::dtNowLocal();

	std::cout << "__NR__:  " << "time: " << end-start << std::endl;
	std::cout << "__NR__:  " << "calls/millisecond: " << REPETITIONS / jle::ARE_YOU_SURE_YOU_WANT_GetMillisecsDay(end-start) << std::endl;
    }

    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "signal -> función (no emit)" << std::endl << std::endl;
    {
	jle::Signal<>   svoid;

	svoid.connect( receptor_do_nothing );

	jle::DateTime  start =  jle::dtNowLocal();
	for (int i=0; i<REPETITIONS; ++i)
	    svoid();

	jle::DateTime end = jle::dtNowLocal();

	std::cout << "__NR__:  " << "time: " << end-start << std::endl;
	std::cout << "__NR__:  " << "calls/millisecond: " << REPETITIONS / jle::ARE_YOU_SURE_YOU_WANT_GetMillisecsDay(end-start) << std::endl;
    }

    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "signal -> método  (emit)" << std::endl << std::endl;
    {
	jle::Signal<>   svoid;
	class_receptor_do_nothing class_do_nothing;

	svoid.connect(&class_do_nothing, &class_receptor_do_nothing::do_nothing );

	jle::DateTime  start =  jle::dtNowLocal();
	for (int i=0; i<REPETITIONS; ++i)
	    svoid.emit();

	jle::DateTime end = jle::dtNowLocal();

	std::cout << "__NR__:  " << "time: " << end-start << std::endl;
	std::cout << "__NR__:  " << "calls/millisecond: " << REPETITIONS / jle::ARE_YOU_SURE_YOU_WANT_GetMillisecsDay(end-start) << std::endl;
    }

    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "llamada directa función " << std::endl << std::endl;
    {
	jle::DateTime  start =  jle::dtNowLocal();
	for (int i=0; i<REPETITIONS; ++i)
	    count();

	jle::DateTime end = jle::dtNowLocal();

	std::cout << contador << std::endl;
	contador = 0;

	std::cout << "time: " << end-start << std::endl;
	if (jle::ARE_YOU_SURE_YOU_WANT_GetMillisecsDay(end-start) == 0)
	    std::cout << "too fast..." << std::endl;
	else
	    std::cout << "__NR__:  " << "calls/millisecond: " << REPETITIONS / jle::ARE_YOU_SURE_YOU_WANT_GetMillisecsDay(end-start) << std::endl;
    }

    std::cout << "---------------------------------------------------------------------" << std::endl;
    std::cout << "signal -> función " << std::endl << std::endl;
    {
	jle::Signal<>   svoid;

	svoid.connect( count );

	jle::DateTime  start =  jle::dtNowLocal();
	for (int i=0; i<REPETITIONS; ++i)
	    svoid();

	jle::DateTime end = jle::dtNowLocal();

	std::cout << contador << std::endl;
	contador = 0;

	std::cout << "__NR__:  " << "time: " << end-start << std::endl;
	std::cout << "__NR__:  " << "calls/millisecond: " << REPETITIONS / jle::ARE_YOU_SURE_YOU_WANT_GetMillisecsDay(end-start) << std::endl;
    }

    return 0;
}


void jle::AlarmMsg(const jle::Alarm& alarm)
{
    std::cout << alarm << std::endl;
}
