#include "exception.h"

namespace aw
{

    exception::exception(const any_string<std::string> &msg): m_message(msg.value)
    {

    }

    exception::~exception()
    {

    }

    const char* exception::what() const noexcept
    {
        return m_message.c_str();
    }

}

