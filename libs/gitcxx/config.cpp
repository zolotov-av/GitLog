#include "config.h"

namespace git
{

    config::config(config &&other): m_config{ other.m_config }
    {
        other.m_config = nullptr;
    }

    QString config::getString(const QString &name)
    {
        if ( m_config == nullptr )
            return { };


        const auto utf8_name = name.toUtf8();
        git_config_entry *entry;
        check( git_config_get_entry(&entry, m_config, utf8_name.constData()) );

        QString value = QString::fromUtf8(entry->value);

        git_config_entry_free(entry);

        return std::move(value);
    }

}
