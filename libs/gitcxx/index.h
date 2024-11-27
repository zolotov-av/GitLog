#pragma once

#include "common.h"
#include "oid.h"

namespace git
{

    /**
     * @brief Класс представляющий индекс
     * @ingroup GitCXX
     *
     * Позволяет читать проиндексированные данные
     */
    class index
    {
    private:

        git_index *m_index { nullptr };

    public:

        constexpr index() = default;
        index(const index &) = delete;
        index(index &&other): m_index(other.m_index)
        {
            other.m_index = nullptr;
        }
        index(git_index *idx): m_index(idx)
        {

        }

        ~index()
        {
            git_index_free(m_index);
        }

        index& operator = (const index &) = delete;
        index& operator = (index &&other);

        object_id oidByPath(const QString &path);

        /**
         * @brief Добавить файл в индекс
         * @param path путь к файлу (относительно рабочего каталога)
         */
        void addByPath(const QString &path);

        /**
         * @brief Добавить файл/каталог в индекс
         * @param path путь к файлу (относительно рабочего каталога)
         */
        void addAll(const QString &path, unsigned flags = GIT_INDEX_ADD_DEFAULT);

        /**
         * @brief Удалить файл из индекса
         * @param path путь к файлу (относительно рабочего каталога)
         */
        void removeByPath(const QString &path);

        /**
         * @brief Записать индекс на диск
         */
        void write();

        void readTree(const tree &tree);

        /**
         * @brief Записать индекс в виде дерева
         * @return OID записанного дерева
         */
        object_id writeTree();

        git_index* data() { return m_index; }

    };

}
