#ifndef JLE_SIGNAL_SLOT_H
#define JLE_SIGNAL_SLOT_H


#include <list>
#include <stdexcept>    //  run_time_error exception
#include <tuple>

#include "support/basic_types.hpp"
#include "support/shared_ptr.hpp"


#define JLE_CONNECT_THIS(__SIGNAL__, __METHOD_NAME__)     (__SIGNAL__).connect(this, &std::remove_reference<decltype(*this)>::type::__METHOD_NAME__);


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

//      S i g n a l R e c e p t o r

//----------------------------------------------------------------------------



class signal_receptor : private jle::non_copyable {
    std::list<jle::weak_ptr<internal::base_connection>> listConnections;

public:
    signal_receptor(void) {};

    void RegisterConnection(jle::shared_ptr<internal::base_connection> pconnection) {
        listConnections.push_back(pconnection);
    };

    void UnRegisterConnection(jle::shared_ptr<internal::base_connection> connection) {
        auto itConnection = listConnections.begin();
        while(itConnection != listConnections.end())
        {
            //  it is safe to delete here
            //  we only itereate the list at this point and in destructor
            if ( itConnection->expired() ) {
                itConnection = listConnections.erase(itConnection);
            }
            else {
                auto sp_it = itConnection->lock();
                //  cleaning
                if ( sp_it->is_disconnected()) {
                    itConnection = listConnections.erase(itConnection);
                }
                else if ( sp_it->isSame(connection.get()) ) {
                    sp_it->disconnect();
                    itConnection = listConnections.erase(itConnection);
                }
                else
                    ++itConnection;
            }
        }
    };

    virtual ~signal_receptor(void) {
        try{
            //  say goodbye to singals pointing to us
            auto itConnection = listConnections.begin();
            while(itConnection != listConnections.end())
            {
                if ( itConnection->expired() ) {
                    itConnection = listConnections.erase(itConnection);
                    continue;
                }
                itConnection->lock()->disconnect();
                ++itConnection;
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

    virtual signal_receptor*  GetSignalReceptor(void) const  = 0;

};





template<typename TReceiver, typename... Args>
class Connection
        : public base_connectionParam<Args...>
{
    TReceiver* pt2Object;
    void (TReceiver::*fpt)(Args...);


public:
    Connection(TReceiver* _pt2Object, void (TReceiver::*_fpt)(Args...))
        :   pt2Object(_pt2Object),
            fpt(_fpt)
        {}

    void emit(Args... args){
        (*pt2Object.*fpt)();
    };



    TReceiver* GetReceiver(void)  {
        return pt2Object;
    };
    void (TReceiver::*GetFuncPointer(void)) (Args...)   {
        return fpt;
    };

    bool isSame(base_connection* pbc)
    {
        Connection<TReceiver>* pc = dynamic_cast<Connection<TReceiver>* > (pbc);
        if (pc)
            return this == pc;
        else
            return false;
    }

    signal_receptor*  GetSignalReceptor(void) const {
        return pt2Object;
    }

};


};      //  namespace internal {



template <typename... Args>
class Signal
        :   public signal_receptor//,public non_copyable (implicit)
{
	int processing_emit;
    std::list< jle::shared_ptr<internal::base_connectionParam<Args...> > > connections;

    //  connection to funcions (pointer and   is_connected?)
    std::list< std::tuple<void (*)(Args...), bool> >  functConnections;


public:
	Signal() : processing_emit(0) {}

	~Signal() {
        try{
            disconnect_all();
            if (processing_emit>0)
                throw std::runtime_error("~Signal<> on emit");
                //	pending to check
        } catch(...){
            std::cerr << __PRETTY_FUNCTION__ << "(" << __FILE__ << ":" << __LINE__ << ")"  << "exception on destructor"  <<  std::endl;
        }
    }

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
                pbc (new internal::Connection<TReceiver>(receiver, fpt));
        connections.push_back(pbc);
        //pbc.DANGEROUS_ThisInstance_NOT_Delete();  //  done by receptor
        receiver->RegisterConnection(jle::shared_ptr<internal::base_connection>(pbc));
    };

    template<typename TReceiver>
    bool disconnect(TReceiver* receiver, void (TReceiver::*fpt)(Args...)) {
        auto it2ptrbase_connection = connections.begin();
        while(it2ptrbase_connection != connections.end())
        //for(auto&& it2ptrbase_connection : connections)
        {
            internal::Connection<TReceiver>* pconnection = dynamic_cast<internal::Connection<TReceiver>* > (it2ptrbase_connection->get());
            if  (
                    pconnection
                    &&
                    pconnection->GetReceiver()    == receiver
                    &&
                    pconnection->GetFuncPointer() == fpt
                    &&
                    (*it2ptrbase_connection)->is_disconnected() == false  //  this line reduces performance
                )
            {
                receiver->UnRegisterConnection(
                                //jle::shared_ptr<base_connection>(*it2ptrbase_connection)
                                //jle::make_shared<base_connection>(*it2ptrbase_connection)
                                //internal::base_connectionParam<Args...>
                                //jle::make_shared<internal::base_connectionParam<Args...>>(*it2ptrbase_connection)
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

    void disconnect_all(void) {
        //  cleaning
        for(auto&&   it2ptrbase_connection : connections)
        {
            if ( it2ptrbase_connection->is_disconnected() == false) {
                signal_receptor* sr = it2ptrbase_connection->GetSignalReceptor();
                sr->UnRegisterConnection(jle::shared_ptr<internal::base_connection>(it2ptrbase_connection));
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

    int notify(Args... args) {
        return emit(args...);
    };

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
			};


					//  functions are different
            auto itconnection2funct = functConnections.begin();
            while(itconnection2funct != functConnections.end())
			{
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

    void operator()(Args... args) {
        emit(args...);
    };

    //  syntax shortut to connect with other signals
    template<typename TReceiver>
    void connect(TReceiver* receiver) {
        connect(receiver, &jle::Signal<Args...>::operator());
    }


    //  connect to functions ----------------------------------------
    void connect( void (*pt2Function)(Args...) ) {
        functConnections.push_back( std::make_tuple(pt2Function, true));
    }

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
