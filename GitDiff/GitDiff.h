#pragma once

#include <QQmlApplicationEngine>
#include <GitTools/DiffModel.h>

/**
 * @brief Окно GitDiff
 */
class GitDiff: public QQmlApplicationEngine
{
    Q_OBJECT
    Q_PROPERTY(QString oldFileName READ oldFileName NOTIFY diffChanged FINAL)
    Q_PROPERTY(QString newFileName READ newFileName NOTIFY diffChanged FINAL)
    Q_PROPERTY(QAbstractItemModel* lineModel READ lineModel CONSTANT FINAL)

private:

    QString m_old_file_name { };
    QString m_new_file_name { };
    DiffModel m_line_model { this };

public:

    explicit GitDiff(QObject *parent = nullptr);
    GitDiff(const GitDiff &) = delete;
    GitDiff(GitDiff &&) = delete;
    ~GitDiff();

    GitDiff& operator = (const GitDiff &) = delete;
    GitDiff& operator = (GitDiff &&) = delete;

    const QString& oldFileName() const { return m_old_file_name; }
    const QString& newFileName() const { return m_new_file_name; }
    QAbstractItemModel* lineModel() { return &m_line_model; }

    /**
     * @brief Сравнить два текста
     * @param oldData старый текст
     * @param newData новый текст
     */
    void diffData(const QByteArray &oldData, const QByteArray &newData);

    /**
     * @brief Сравнить два файла
     *
     * Читает два файла и проводит их сравнение
     *
     * @param oldFile путь к старому файлу
     * @param newFile путь к новому файлу
     */
    void diffFiles(const QString &oldFile, const QString &newFile);

signals:

    void diffChanged();

};

