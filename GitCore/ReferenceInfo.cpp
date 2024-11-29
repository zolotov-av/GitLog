#include "ReferenceInfo.h"

ReferenceInfo::ReferenceInfo(const git::reference &ref):
    name(ref.name()),
    short_name(ref.shortName()),
    target(ref.resolve().target()),
    isHead(ref.isHead()),
    isBranch(ref.isBranch()),
    isRemote(ref.isRemote()),
    isTag(ref.isTag())
{
}
