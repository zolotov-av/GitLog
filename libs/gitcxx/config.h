#pragma once

#include "common.h"

namespace git
{

    /**
     * @brief Класс представляющий конфигурацию
     * @ingroup GitCXX
     *
     * Позволяет читать параметры сохраненные в конфигурации git
     */
    class config
    {
    private:

        git_config *m_config { nullptr };

    public:

        config() = default;
        config(const config &) = delete;
        config(config &&other);
        config(git_config *cfg): m_config{cfg} { }

        ~config()
        {
            git_config_free(m_config);
        }

        QString getString(const QString &name);

    };

}
