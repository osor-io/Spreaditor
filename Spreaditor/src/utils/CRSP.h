#pragma once
#include "Debug.h"

/**
Curiously Recurring Template Pattern applyed to Singletons

To use we need to add this class as a friend and make the constructor
and destructor methods private to avoid normal instantiation. Example:

    class T :  CRSP<T> {
    private:
        friend class CRSP<T>;
        T();
        ~T();
    . . . Rest of Class . . .

*/
template <typename T>
class CRSP {
public:
    static T& get() {
        static T s_instance;
        return s_instance;
    }
protected:
    CRSP() {}
    virtual ~CRSP() {}

private:
    CRSP(const CRSP&) = delete;
    CRSP& operator=(const CRSP&) = delete;

    CRSP(const CRSP&&) = delete;
    CRSP& operator=(const CRSP&&) = delete;

};