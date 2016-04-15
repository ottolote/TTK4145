#pragma once

#include<ostream>
#include<boost/thread/shared_mutex.hpp>

class safeprint
{
    private:
        static boost::shared_mutex cout_lock;
        std::ostream* stream;

    public:
        safeprint&  operator<<(std::ostream& ) {
            cout_lock.lock_shared();
            std::cout << "public safe" << std::endl;
            cout_lock.unlock();
            return *this;
        }
        safeprint&  operator<<(const std::string& msg) {
            cout_lock.lock_shared();
            std::cout << msg;
            cout_lock.unlock();
            //std::cout << "public safe" << std::endl;
            return *this;
        }

};

//safeprint& operator<<(safeprint safe_cout, std::ostream o){
//    safe_cout.lock();
//    std::cout << &o;
//    safe_cout.unlock();
//    return safe_cout*;
//}
