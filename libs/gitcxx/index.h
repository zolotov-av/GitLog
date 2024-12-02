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

        class const_entry
        {
        private:

            const git_index_entry *m_entry { nullptr };

        public:

            constexpr const_entry(const git_index_entry *ent): m_entry{ent} { }
            const_entry(const const_entry &) = default;
            const_entry(const_entry &&) = default;

            const_entry& operator = (const const_entry &) = default;
            const_entry& operator = (const_entry &&) = default;

            object_id id() const { return object_id{ m_entry->id }; }
            QString path() const { return QString::fromUtf8(m_entry->path); }
            git_index_time ctime() const { return m_entry->ctime; }
            git_index_time mtime() const { return m_entry->mtime; }

            const git_index_entry* data() const { return m_entry; }
            const git_index_entry* constData() const { return m_entry; }
        };

        struct conflict_entry
        {
            const_entry ancestor;
            const_entry our;
            const_entry their;
        };

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

        conflict_entry getConflictEntry(const QString &path);;

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
