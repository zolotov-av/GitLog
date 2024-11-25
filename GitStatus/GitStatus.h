#pragma once

#include <QQmlEngine>
#include <QObject>
#include <GitRepository.h>
#include <GitStatusModel.h>

class GitStatus: public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_UNCREATABLE("Don't create GitStatus from QML")

    Q_PROPERTY(QAbstractItemModel* model READ model CONSTANT FINAL)

private:

    GitRepository m_repo { this };
    GitStatusModel m_model { this };

public:

    explicit GitStatus(QObject *parent = nullptr);
    GitStatus(const GitStatus &) = delete;
    GitStatus(GitStatus &&) = delete;
    ~GitStatus();

    GitStatus& operator = (const GitStatus &) = delete;
    GitStatus& operator = (GitStatus &&) = delete;

    GitStatusModel* model() { return &m_model; }

};
