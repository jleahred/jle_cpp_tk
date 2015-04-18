#include <iostream>

#include "support/signal_slot.hpp"
#include "support/test.h"



int g_received_count = 0;
int g_last_rec_num = -1;
std::string g_last_rec_string = "";

void reset_last_received() {
    g_last_rec_num = -1;
    g_last_rec_string = "";
}

#define INITIALIZE(__COUNT__) \
    g_received_count = 0;  \
    int total2receive = __COUNT__;  \
    reset_last_received();

bool received_void(void) {
    if(g_last_rec_num == -1  &&  g_last_rec_string=="")
        return true;
    else
        return false;
}

bool received_num(int num) {
    if(g_last_rec_num == num  &&  g_last_rec_string=="")
        return true;
    else
        return false;
}

bool received_num_string(int num, const std::string& s) {
    if(g_last_rec_num == num  &&  g_last_rec_string==s)
        return true;
    else
        return false;
}






class  Signal_receptor  :  public  jle::signal_receptor
{
public:
    Signal_receptor()=default;

    void on_void_method(void) {
    reset_last_received();
        ++g_received_count;
    }

    void on_number_method(int num) {
        reset_last_received();
        ++g_received_count;
        g_last_rec_num  = num;
    }

    void on_number_string_method(int num, const std::string& s) {
        reset_last_received();
        ++g_received_count;
        g_last_rec_num  = num;
        g_last_rec_string = s;
    }

private:
};


void on_void_function(void)
{
    reset_last_received();
    ++g_received_count;
}

void on_number_function(int num)
{
    reset_last_received();
    ++g_received_count;
    g_last_rec_num  = num;
}

void on_number_string_function(int num, const std::string& s)
{
    reset_last_received();
    ++g_received_count;
    g_last_rec_num  = num;
    g_last_rec_string = s;
}



void signal_to_function(void)
{
    JLE_TEST_INIT

    //  no parameters
    {
        INITIALIZE(1);

        jle::signal<>  signal;
        signal.connect(on_void_function);
        signal.emit();
        JLE_TEST_ASSERT(total2receive == g_received_count);
    }

    //  no parameters multiple emit
    {
        INITIALIZE(3);

        jle::signal<>  signal;
        signal.connect(on_void_function);
        signal.emit();
        signal.emit();
        signal.emit();
        JLE_TEST_ASSERT(total2receive == g_received_count);
    }

    //  no parameters multiple connect same fuction
    {
        INITIALIZE(2);

        jle::signal<>  signal;
        signal.connect(on_void_function);
        signal.connect(on_void_function);
        signal.emit();
        JLE_TEST_ASSERT(total2receive == g_received_count  &&  received_void());
    }

    //  one parameter
    {
        INITIALIZE(1);

        int send = 42;
        jle::signal<int>  signal;
        signal.connect(on_number_function);
        signal.emit(send);
        JLE_TEST_ASSERT(total2receive == g_received_count  &&  received_num(send));
    }

    //  two parameters
    {
        INITIALIZE(1);

        int send_num = 42;
        std::string send_string = "abc";
        jle::signal<int, const std::string&>  signal;
        signal.connect(on_number_string_function);
        signal.emit(send_num, send_string);
        JLE_TEST_ASSERT(total2receive == g_received_count  &&  received_num_string(send_num, send_string));
    }
}

void signal_to_method()
{
    JLE_TEST_INIT

    //  no parameters
    {
        INITIALIZE(1);

        jle::signal<>  signal;
        Signal_receptor sr;
        JLE_CONNECT_METHOD(signal, sr, on_void_method);
        signal.emit();
        JLE_TEST_ASSERT(total2receive == g_received_count);
    }

    //  no parameters multiple emit
    {
        INITIALIZE(3);

        jle::signal<>  signal;
        Signal_receptor sr;
        JLE_CONNECT_METHOD(signal, sr, on_void_method);
        signal.emit();
        signal.emit();
        signal.emit();
        JLE_TEST_ASSERT(total2receive == g_received_count);
    }

    //  no parameters multiple connect same fuction
    {
        INITIALIZE(2);

        jle::signal<>  signal;
        Signal_receptor sr;
        JLE_CONNECT_METHOD(signal, sr, on_void_method);
        JLE_CONNECT_METHOD(signal, sr, on_void_method);
        signal.emit();
        JLE_TEST_ASSERT(total2receive == g_received_count  &&  received_void());
    }

    //  one parameter
    {
        INITIALIZE(1);

        int send = 42;
        jle::signal<int>  signal;
        Signal_receptor sr;
        JLE_CONNECT_METHOD(signal, sr, on_number_method);
        signal.emit(send);
        JLE_TEST_ASSERT(total2receive == g_received_count  &&  received_num(send));
    }

    //  two parameters
    {
        INITIALIZE(1);

        int send_num = 42;
        std::string send_string = "abc";
        jle::signal<int, const std::string&>  signal;
        Signal_receptor sr;
        JLE_CONNECT_METHOD(signal, sr, on_number_string_method);
        signal.emit(send_num, send_string);
        JLE_TEST_ASSERT(total2receive == g_received_count  &&  received_num_string(send_num, send_string));
    }
}

void signal_to_signal()
{
    JLE_TEST_INIT

    //  no parameters
    {
        INITIALIZE(1);

        jle::signal<>  signal1;
        jle::signal<>  signal2;
        signal1.connect(&signal2);
        signal2.connect(on_void_function);
        signal1.emit();
        JLE_TEST_ASSERT(total2receive == g_received_count);
    }

    //  no parameters multiple emit
    {
        INITIALIZE(3);

        jle::signal<>  signal1;
        jle::signal<>  signal2;
        signal1.connect(&signal2);
        signal2.connect(on_void_function);
        signal1.emit();
        signal1.emit();
        signal1.emit();
        JLE_TEST_ASSERT(total2receive == g_received_count);
    }

    //  no parameters multiple connect same fuction
    {
        INITIALIZE(2);

        jle::signal<>  signal1;
        jle::signal<>  signal2;
        signal1.connect(&signal2);
        signal1.connect(&signal2);
        signal2.connect(on_void_function);
        signal1.emit();
        JLE_TEST_ASSERT(total2receive == g_received_count  &&  received_void());
    }

    //  one parameter
    {
        INITIALIZE(1);

        int send = 42;
        jle::signal<int>  signal1;
        jle::signal<int>  signal2;
        signal1.connect(&signal2);
        signal2.connect(on_number_function);
        signal1.emit(send);
        JLE_TEST_ASSERT(total2receive == g_received_count  &&  received_num(send));
    }

    //  two parameters
    {
        INITIALIZE(1);

        int send_num = 42;
        std::string send_string = "abc";

        jle::signal<int, const std::string&>  signal1;
        jle::signal<int, const std::string&>  signal2;

        signal1.connect(&signal2);
        signal2.connect(on_number_string_function);
        signal1.emit(send_num, send_string);
        JLE_TEST_ASSERT(total2receive == g_received_count  &&  received_num_string(send_num, send_string));
    }
}




int main(void)
{
    JLE_TEST_FILE

    signal_to_function();
    signal_to_method();
    signal_to_signal();

    JLE_TEST_REPORT
}
