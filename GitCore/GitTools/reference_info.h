#pragma once

#include <gitcxx/reference.h>

namespace git
{

    class reference_info
    {
    public:

        QString name;
        QString short_name;
        object_id target;
        bool isHead { false };
        bool isBranch { false };
        bool isRemote { false };
        bool isTag { false };

        reference_info(const reference &ref):
            name(ref.name()),
            short_name(ref.shortName()),
            target(ref.resolve().target()),
            isHead(ref.isHead()),
            isBranch(ref.isBranch()),
            isRemote(ref.isRemote()),
            isTag(ref.isTag())
        {
        }

        reference_info(const reference_info &) = default;
        reference_info(reference_info &&) = default;

        reference_info& operator = (const reference_info &) = default;
        reference_info& operator = (reference_info &&) = default;

    };

}
