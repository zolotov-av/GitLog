#include "thread.h"
#include "ThreadData.h"
#include <QThreadStorage>
#include <pthread.h>

namespace aw
{

    static QThreadStorage<ThreadData*> thread_data { };


    bool isCurrentThreadRegistered()
    {
        return thread_data.localData() != nullptr;
    }

    const char* currentThreadName()
    {
        const auto td = thread_data.localData();

        if ( td )
            return td->name();

        return "unknown";
    }

    void setCurrentThreadName(const any_string<std::string>& name)
    {
        pthread_setname_np(pthread_self(), name.value.c_str());

        auto td = thread_data.localData();
        if ( td != nullptr )
        {
            //std_trace::log("vt::register_thread(%s) already registered: %s\n", name, td->name());
            td->setName(name);
            return;
        }

        thread_data.setLocalData( new ThreadData(name) );
    }

}
