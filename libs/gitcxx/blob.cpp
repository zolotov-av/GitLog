#include "blob.h"
#include "repository.h"
#include "exception.h"
#include <limits>

namespace git
{

    blob::blob(blob &&other): m_blob{ other.m_blob }
    {
        other.m_blob = nullptr;
    }

    blob::blob(repository &repo, const object_id &oid): blob{ repo.lookupBlob(oid) }
    {

    }

    blob& blob::operator = (blob &&other)
    {
        git_blob_free(m_blob);
        m_blob = other.m_blob;
        other.m_blob = nullptr;
        return *this;
    }

    QByteArray blob::rawContent() const
    {
        const uint64_t size = rawSize();
        if ( size <= 0 )
            return QByteArray{ };

        if ( size >= std::numeric_limits<QByteArray::size_type>::max() )
            throw exception("Data size too large for QByteArray");

        const auto data = reinterpret_cast<const char*>(git_blob_rawcontent(m_blob));
        if ( data == nullptr )
            throw exception(lastGitError());

        return QByteArray{data, QByteArray::size_type(size)};
    }

}
