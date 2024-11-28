#pragma once

#include "common.h"

namespace git
{

    class reference;

    /**
     * @brief Класс представляющий итератор ссылок (git_reference_iterator)
     * @ingroup GitCXX
     */
    class reference_iterator
    {
    private:

        git_reference_iterator *m_iter { nullptr };

    public:

        explicit reference_iterator(git_reference_iterator *iter = nullptr): m_iter{iter} { }
        reference_iterator(const reference_iterator &) = delete;
        reference_iterator(reference_iterator &&other);
        ~reference_iterator() { git_reference_iterator_free(m_iter); }
        
        reference_iterator& operator = (const reference_iterator &) = delete;
        reference_iterator& operator = (reference_iterator &&other);

        /**
         * @brief Вернуть следующую ссылку
         * @return если ссылок больше нет, то вренет Null
         */
        reference next();

        /**
         * @brief Вернуть имя следущей ссылки
         * @return если ссылок больше нет, то вренет Null
         */
        QString nextName();

        git_reference_iterator* data() { return m_iter; }
        const git_reference_iterator* data() const { return m_iter; }
        const git_reference_iterator* constData() const { return m_iter; }

    };

}
