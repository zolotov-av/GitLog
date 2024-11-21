#pragma once

#include "common.h"

namespace git
{

    /**
     * @brief Класс представляющий объект blob
     * @ingroup GitCXX
     *
     * Используется для чтения данных из сохраненных "файлов"
     */
    class blob
    {
    private:

        git_blob *m_blob { nullptr };

    public:

        blob() = default;
        blob(const blob &) = delete;
        blob(blob &&other);

        blob(git_blob *v): m_blob(v)
        {
        }

        blob(repository &repo, const object_id &oid);

        ~blob()
        {
            git_blob_free(m_blob);
        }

        blob& operator = (const blob &) = delete;
        blob& operator = (blob &&other);

        git_object_size_t rawSize() const
        {
            return git_blob_rawsize(m_blob);
        }

        QByteArray rawContent() const;

    };

}
