#ifndef JLE_SIGNAL_SLOT_H
#define JLE_SIGNAL_SLOT_H

///  \example  ./core/ex_signal_slot.cpp


#include <list>
#include <stdexcept>    //  run_time_error exception
#include <tuple>

#include "core/basic_types.hpp"
#include "core/shared_ptr.hpp"



#define JLE_CONNECT_METHOD(__SIGNAL__, __INSTANCE__, __METHOD_NAME__)     \
            (__SIGNAL__).connect(&__INSTANCE__, &std::remove_reference<decltype(__INSTANCE__)>::type::__METHOD_NAME__);

#define JLE_CONNECT_THIS(__SIGNAL__, __METHOD_NAME__)  JLE_CONNECT_METHOD(__SIGNAL__, *this, __METHOD_NAME__)




namespace jle
{


const int SIGNAL_SLOT_MAX_DEEP_EMIT = 20;


//----------------------------------------------------------------------------

//    B a s e C o n n e c t i o n

//----------------------------------------------------------------------------
namespace internal {


class base_connection
        : private jle::non_copyable
{
    bool disconnected;

public:
    base_connection() : disconnected(false) {};

    virtual ~base_connection(){};
    virtual bool isSame(base_connection*)= 0;

    void disconnect(void) {
        disconnected = true;
    };

    bool is_disconnected(void) const { return disconnected; };

};

};  //  namespace internal







//----------------------------------------------------------------------------

//      s i g n a l _ r e c e p t o r

//----------------------------------------------------------------------------


/**
    @brief Any object connected to signals, has to inherit from signal_receptor

    It will track the life in order to properly disconnection

    Example:
    \include ./core/signal_slot.cpp
*/


class signal_receptor : private jle::non_copyable {
    std::list<jle::weak_ptr<internal::base_connection>> list_connections;

public:
    signal_receptor(void) {};

    void internal_register_connection(jle::shared_ptr<internal::base_connection> pconnection) {
        list_connections.push_back(pconnection);
    };

    void un_internal_register_connection(jle::shared_ptr<internal::base_connection> connection) {
        auto it_connection = list_connections.begin();
        while(it_connection != list_connections.end())
        {
            //  it is safe to delete here
            //  we only itereate the list at this point and in destructor
            if ( it_connection->expired() ) {
                it_connection = list_connections.erase(it_connection);
            }
            else {
                auto sp_it = it_connection->lock();
                //  cleaning
                if ( sp_it->is_disconnected()) {
                    it_connection = list_connections.erase(it_connection);
                }
                else if ( sp_it->isSame(connection.get()) ) {
                    sp_it->disconnect();
                    it_connection = list_connections.erase(it_connection);
                }
                else
                    ++it_connection;
            }
        }
    };

    virtual ~signal_receptor(void) {
        try{
            //  say goodbye to singals pointing to us
            auto it_connection = list_connections.begin();
            while(it_connection != list_connections.end())
            {
                if ( it_connection->expired() ) {
                    it_connection = list_connections.erase(it_connection);
                    continue;
                }
                it_connection->lock()->disconnect();
                ++it_connection;
            }
        } catch(...){
            std::cerr << __PRETTY_FUNCTION__ << "(" << __FILE__ << ":" << __LINE__ << ")"  << "exception on destructor"  <<  std::endl;
        }
    };

};



namespace internal {


template<typename... Args>
class base_connectionParam
        : public base_connection
{

public:
    base_connectionParam() : base_connection() {};

    virtual ~base_connectionParam(){};

    virtual void emit(Args...)=0;

    virtual signal_receptor*  get_signal_receptor(void) const  = 0;

};





template<typename TReceiver, typename... Args>
class connection
        : public base_connectionParam<Args...>
{
    TReceiver* pt2object;
    void (TReceiver::*fpt)(Args...);


public:
    connection(TReceiver* _pt2Object, void (TReceiver::*_fpt)(Args...))
        :   pt2object(_pt2Object),
            fpt(_fpt)
        {}

    void emit(Args... args){
        (*pt2object.*fpt)(args...);
    };



    TReceiver* get_receiver(void)  {
        return pt2object;
    };
    void (TReceiver::*get_func_pointer(void)) (Args...)   {
        return fpt;
    };

    bool isSame(base_connection* pbc)
    {
        connection<TReceiver, Args...>* pc = dynamic_cast<connection<TReceiver, Args...>* > (pbc);
        if (pc)
            return this == pc;
        else
            return false;
    }

    signal_receptor*  get_signal_receptor(void) const {
        return pt2object;
    }

};


};      //  namespace internal {



/**
    @brief signal instance to connect and emit

    You can connect to functions methods and even other signals
    It is static, good performance and compiling time checked

    Example:
    \include ./core/signal_slot.cpp
*/

template <typename... Args>
class signal
        :   public signal_receptor   // ,  private non_copyable (implicit)
{
    int processing_emit{0};
    std::list< jle::shared_ptr<internal::base_connectionParam<Args...> > > connections;

    //  connection to funcions (pointer and   is_connected?)
    std::list< std::tuple<void (*)(Args...), bool> >  functConnections;


public:
    signal() = default;

    ~signal() {
        try{
            disconnect_all();
            if (processing_emit>0)
                throw std::runtime_error("~signal<> on emit");
                //    pending to check
        } catch(...){
            std::cerr << __PRETTY_FUNCTION__ << "(" << __FILE__ << ":" << __LINE__ << ")"  << "exception on destructor"  <<  std::endl;
        }
    }

    /**
        @brief it will give the number of emits on scope

        Used internally to check if running on destructor
    */
    int get_processing_emits(void) const {  return  processing_emit; }

    template<typename TReceiver>
    void connect(TReceiver* receiver, void (TReceiver::*fpt)(Args...)) {
        if(connections.size()%5==0)     //  small optimization to avoid clean on ervery connection
        {
            auto it2ptrbase_connection = connections.begin();
            while(it2ptrbase_connection != connections.end())
            {
                if  ((*it2ptrbase_connection)->is_disconnected() == true)
                {
                    if (processing_emit==0)
                    {
                        it2ptrbase_connection = connections.erase(it2ptrbase_connection);
                        continue;   //  avoid ++it2ptrbase_connection
                    }
                }
                ++it2ptrbase_connection;
            }
        }


        jle::shared_ptr<internal::base_connectionParam<Args...> >
                pbc (new internal::connection<TReceiver, Args...>(receiver, fpt));
        connections.push_back(pbc);
        receiver->internal_register_connection(jle::shared_ptr<internal::base_connection>(pbc));
    };

    /**
        @brief disconnect a signal from a method

        If you have connected a signal more than one to a method (allowed), disconnect will remove
        only one of the connections
    */
    template<typename TReceiver>
    bool disconnect(TReceiver* receiver, void (TReceiver::*fpt)(Args...)) {
        auto it2ptrbase_connection = connections.begin();
        while(it2ptrbase_connection != connections.end())
        {
            internal::connection<TReceiver>* pconnection = dynamic_cast<internal::connection<TReceiver>* > (it2ptrbase_connection->get());
            if  (
                    pconnection
                    &&
                    pconnection->get_receiver()    == receiver
                    &&
                    pconnection->get_func_pointer() == fpt
                    &&
                    (*it2ptrbase_connection)->is_disconnected() == false  //  this line reduces performance
                )
            {
                receiver->un_internal_register_connection(
                                jle::shared_ptr<internal::base_connection>(*it2ptrbase_connection)
                            );
                //  WARNING: someone could disconnect from emit context
                //  it's not safe to delete here from list
                if (processing_emit>0)
                    (*it2ptrbase_connection)->disconnect();
                else
                {
                    connections.erase(it2ptrbase_connection);
                }
                return true;
            }
            ++it2ptrbase_connection;
        }
        return false;
    };

    /**
        @brief disconnect all signals

        It will disconnect from functions, methods and other signals
    */
    void disconnect_all(void) {
        //  cleaning
        for(auto&&   it2ptrbase_connection : connections)
        {
            if ( it2ptrbase_connection->is_disconnected() == false) {
                signal_receptor* sr = it2ptrbase_connection->get_signal_receptor();
                sr->un_internal_register_connection(jle::shared_ptr<internal::base_connection>(it2ptrbase_connection));
                if (processing_emit>0)
                    it2ptrbase_connection->disconnect();

            }
        }

        if (processing_emit>0)
        {
            for(auto itconnection2funct : functConnections)
            {
                std::get<1>(itconnection2funct) = false;
            }
        }
        else
        {
            functConnections.clear();
            connections.clear();
        }

    };

    /**
        @brief same as emit

        emit is defined as a macro on Qt toolkit.
        Using notify avoid conflicts with Qt
    */
    int notify(Args... args) {
        return emit(args...);
    };

    /**
        @brief call all connected to the signal

        Exceptions are not trapped. If an exception is thrown
        some slots couldn't receive the signal

        Catching the error here to guaranty all slots are called
        has no cpu cost. But the context information, would be limited
    */
    int emit(Args... args) {
        int count=0;
        try
        {
            ++processing_emit;
            if (processing_emit>jle::SIGNAL_SLOT_MAX_DEEP_EMIT)
                throw std::runtime_error("too deep recursion on emit");

            auto itconnection = connections.begin();
            while(itconnection != connections.end())
            {
                try {
                    //  WARNING: someone could disconnect from emmit context
                    if ( (*itconnection)->is_disconnected() == false) {
                        (*itconnection)->emit(args...);
                        ++count;
                        ++itconnection;
                    }
                    else {
                        itconnection = connections.erase(itconnection);
                        //  Unregister is called automatically
                    }
                } catch(...) {
                    std::cerr << "error on signal::emit()" << std::endl;
                }
            };


                    //  functions are different
            auto itconnection2funct = functConnections.begin();
            while(itconnection2funct != functConnections.end())
            {
                try {
                    if (std::get<1>(*itconnection2funct))
                    {
                        (std::get<0>(*itconnection2funct))(args...);
                        ++count;
                        ++itconnection2funct;
                    }
                    else
                    {
                        itconnection2funct = functConnections.erase(itconnection2funct);
                    }
                } catch(...) {
                    std::cerr << "error on signal::emit()" << std::endl;
                }
            }
        }
        catch (...)
        {
            --processing_emit;
            //  It could happend if we destroy the signal during processing emit
            throw;
        }
        --processing_emit;
        return count;
    };

    /**
        @brief same as emit

        I don't like this option, but it is quite frecuent
    */
    void operator()(Args... args) {
        emit(args...);
    };

    //  syntax shortut to connect with other signals
    template<typename TReceiver>
    void connect(TReceiver* receiver) {
        connect(receiver, &jle::signal<Args...>::operator());
    }


    /**
        @brief connect a signal to a function
    */
    void connect( void (*pt2Function)(Args...) ) {
        functConnections.push_back( std::make_tuple(pt2Function, true));
    }

    /**
        @brief disonnect a signal from a function
    */
    bool disconnect( void (*pt2Function)(Args...) ) {
        auto itconnection2funct = functConnections.begin();
        while(itconnection2funct != functConnections.end())
        {
            if (pt2Function == std::get<0>(*itconnection2funct)) {
                if(processing_emit>0)
                    std::get<1>(*itconnection2funct) = false;
                else
                {
                    functConnections.erase(itconnection2funct);
                }
                return true;
            }
            ++itconnection2funct;
        }
        return false;
    }


};



};  //  end namespace


#endif // JLE_SIGNAL_SLOT_H
