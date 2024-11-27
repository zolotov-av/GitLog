#pragma once

#include <QString>
#include <git2.h>

/**
 * @brief Библиотека GitCXX
 * @defgroup GitCXX
 *
 * Простая обертка вокруг библиотеки libgit2 для менеджента
 * ресурсов с помощью RAII и интеграции с приложениями на Qt.
 * Все строковые операции приводятся к QString.
 *
 * В рамках проекта GitTools у меня нет цели покрывать весь API
 * библиотеки libgit2, но если эта библиотека используется
 * где-то еще, то дополнения привествуются.
 *
 * Здесь нет цели спрятать API библиотеки libgit2, напротив
 * желательно оставить возможность работать вместе с libgit2.
 * В рамках проекта GitTools инкапсуляция планируется на другом
 * уровне.
 */

/**
 * @brief Пространство git
 */
namespace git
{

    /**
     * @brief Вернуть последнюю ошибку
     * @return последняя ошибка
     */
    QString lastGitError();

    /**
     * @brief Проверка кода возврата функций libgit2
     * @param error - код возврата
     *
     * Проверяется ли код возврата и, если обнаружена
     * ошибка, то запускает исключение aw::exception
     * с сообщением об ошибке.
     *
     * @note Параметр error не используется для получения
     *   ошибки, в libgit2 нет такого API, вместо этого
     *   текст ошибки читается с помощью git_error_last(),
     *   поэтому проверка должна следовать сразу за зываемой
     *   функцией.
     */
    void check(int error);

    /**
     * @brief Проверка кода возврата функций libgit2
     * @param error - код возврата
     *
     * Проверяется ли код возврата и, если обнаружена
     * ошибка, то в лог выводится предупреждение.
     *
     * @note Параметр error не используется для получения
     *   ошибки, в libgit2 нет такого API, вместо этого
     *   текст ошибки читается с помощью git_error_last(),
     *   поэтому проверка должна следовать сразу за зываемой
     *   функцией.
     */
    void warn(int error);

    class object_id;
    class repository;
    class tree;

}
