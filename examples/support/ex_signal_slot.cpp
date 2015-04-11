#include <iostream>

#include "support/signal_slot.hpp"



class  Signal_receptor  :  public  jle::signal_receptor
{
public:
    Signal_receptor() = delete;
    Signal_receptor(int _instance) : instance(_instance)
    {
        signal_resend.connect(this, &Signal_receptor::on_internal_resend);
        JLE_CONNECT_METHOD(signal_resend, *this, on_internal_resend);

        //  connecting with this, proposed syntax  ------------------------
        JLE_CONNECT_THIS(signal_resend, on_internal_resend);
        //  ---------------------------------------------------------------
    }


    void on_void_method(void) {
        std::cout << "on instance " << instance << "  received: (void)"<< std::endl;
        signal_resend.emit();
    }

    void on_number_method(int num) {
        std::cout << "on instance " << instance << "  received: " << num << std::endl;
    }

    void on_number_string_method(int num, const std::string& s) {
        std::cout << "on instance " << instance << "  received: (" << num << ", " << s << ")" << std::endl;
    }

private:
    int instance;
    jle::signal<>  signal_resend;

    void on_internal_resend(void) {
        std::cout << "on instance " << instance << "  internal resend: (void)"<< std::endl;
    }
};


void on_void_function(void)
{
    std::cout << "on function " << "  received: (void)" << std::endl;
}

void on_number_function(int num)
{
    std::cout << "on function " << "  received: " << num << std::endl;
}

void on_number_string_function(int num, const std::string& s)
{
    std::cout << "on function " << "  received: (" << num << ", " << s << ")" << std::endl;
}





int main(void)
{
    //  signal to functions
    {
        //  no parameters
        {
            jle::signal<>  signal;
            signal.connect(on_void_function);
            signal.emit();
            signal.notify();
            signal();
        }

        //  one parameter
        {
            jle::signal<int>  signal;
            signal.connect(on_number_function);
            signal.emit(1);
        }

        //  two parameters
        {
            jle::signal<int, const std::string&>  signal;
            signal.connect(on_number_string_function);
            signal.emit(1, "one");
            signal.emit(2, "two");
        }
    }

    //  signal to method
    {
        {   //  no parameters
            jle::signal<>  signal;
            Signal_receptor  sr1{1};
            Signal_receptor  sr2{2};
            {
                signal.connect(&sr1, &Signal_receptor::on_void_method);
                JLE_CONNECT_METHOD(signal, sr2, on_void_method);

                signal.emit();
            }
        }

        {   //  one parameter
            jle::signal<int>  signal;
            Signal_receptor  sr1{1};
            Signal_receptor  sr2{2};
            {
                signal.connect(&sr1, &Signal_receptor::on_number_method);
                JLE_CONNECT_METHOD(signal, sr2, on_number_method);

                signal.emit(1);
            }
        }

        {   //  two parameters
            jle::signal<int, const std::string&>  signal;
            Signal_receptor  sr1{1};
            Signal_receptor  sr2{2};
            {
                signal.connect(&sr1, &Signal_receptor::on_number_string_method);
                JLE_CONNECT_METHOD(signal, sr2, on_number_string_method);

                signal.emit(1, "one");
                signal.emit(2, "two");
            }
        }
    }

    //  signal to signal
    {
        {   //  no parameters
            jle::signal<>  signala;
            jle::signal<>  signalb;
            {
                signala.connect(&signalb);
                signalb.connect(on_void_function);

                signala.emit();
            }
        }

        {   //  one parameter
            jle::signal<int>  signala;
            jle::signal<int>  signalb;
            {
                signala.connect(&signalb);
                signalb.connect(on_number_function);

                signala.emit(1);
            }
        }

        {   //  no parameters
            jle::signal<int, const std::string&>  signala;
            jle::signal<int, const std::string&>  signalb;
            {
                signala.connect(&signalb);
                signalb.connect(on_number_string_function);

                signala.emit(2, "two");
            }
        }
    }
}
