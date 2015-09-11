#ifndef JLE_CONTAINER_SUPPORT__H
#define JLE_CONTAINER_SUPPORT__H



/**
 *     \example  ./core/ex_container.cpp
 */


namespace jle {

    namespace internal_for_containers
    {
        int         register_container(bool is_empty);

        void        register_container_size_change(int reg_code);

        int         get_registered_container_last_size_change(int reg_code);


        bool        is_registered_container(int reg_code);

        void        unregister_container(int reg_code);


    };  //  namespace internal_for_containers

};   //  end namespace jle


#endif      //  JLE_CONTAINER_SUPPORT__H
