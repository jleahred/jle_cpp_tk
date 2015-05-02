#include "container_support.h"

#include <map>

#include "alarm.h"


namespace jle {

    namespace internal_for_containers
    {
        bool                                                     __cleaned__      = false;
        int*                                                     __preg_counter__ =0;
        std::map<int, int /*last registered modif on map*/ >*    __pmreg_counter__=0;


		int& get_reg_counter(void) {
			if (__preg_counter__ == 0)
				__preg_counter__ = new int(0);
			return *__preg_counter__;
		}
		std::map<int, int>&  get_map_reg_counter(void) {
            if(__pmreg_counter__ == 0)
                __pmreg_counter__ = new std::map<int, int>;
            return *__pmreg_counter__;
        }


        int      register_container(bool is_empty)
        {
			int& regCounter = get_reg_counter();
            ++regCounter;
            get_map_reg_counter()[regCounter] = (is_empty ? 0 : 1);
            return regCounter;
        }

        void   register_container_size_change(int reg_code)
        {
            if (get_map_reg_counter().find(reg_code) == get_map_reg_counter().end())
                throw jle::alarm(JLE_HERE, "containersupport", "container is not registered", jle::al::priority::error);
            else
                get_map_reg_counter()[reg_code] = get_map_reg_counter()[reg_code]+1;
        }

        int  get_registered_container_last_size_change(int reg_code)
        {
            if (get_map_reg_counter().find(reg_code) == get_map_reg_counter().end())
                throw jle::alarm(JLE_HERE, "containersupport", "container is not registered", jle::al::priority::error);
            else
                return get_map_reg_counter()[reg_code];
        }

        bool     is_registered_container(int reg_code)
        {
            if (get_map_reg_counter().find(reg_code) != get_map_reg_counter().end())
                return true;
            else
                return false;
        }

        void     unregister_container(int reg_code)
        {
            if (__cleaned__)
                return;

            if (get_map_reg_counter().find(reg_code) == get_map_reg_counter().end())
                throw jle::alarm(JLE_HERE, "containersupport", "container is not registered", jle::al::priority::error);
            else
                get_map_reg_counter().erase(reg_code);
        }


    };  //  namespace internal_for_containers

    void __internal_containersupport_nevercall_me____release_on_exit(void)
    {
//        int*                                                     __preg_counter__ =0;
//        std::map<int, int /*last registered modif on map*/ >*    __pmreg_counter__=0;
        internal_for_containers::__cleaned__ = true;
        delete internal_for_containers::__preg_counter__;
        delete internal_for_containers::__pmreg_counter__;
    }


};   //  end namespace jle


