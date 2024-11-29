#pragma once

#include <gitcxx/reference.h>

class ReferenceInfo
{
public:

    QString name { };
    QString short_name { };
    git::object_id target { };
    bool isHead { false };
    bool isBranch { false };
    bool isRemote { false };
    bool isTag { false };

    ReferenceInfo() { };
    ReferenceInfo(const git::reference &ref);
    ReferenceInfo(const ReferenceInfo &) = default;
    ReferenceInfo(ReferenceInfo &&) = default;
    ~ReferenceInfo() { }

    ReferenceInfo& operator = (const ReferenceInfo &) = default;
    ReferenceInfo& operator = (ReferenceInfo &&) = default;

};
