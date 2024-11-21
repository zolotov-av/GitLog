#pragma once

#include "QuickDialog.h"
#include <QQmlEngine>
#include <QDialog>
#include <GitTools/GitLogModel.h>
#include <GitTools/RefsModel.h>

class CreateBranchDialog: public QuickDialog
{
    Q_OBJECT
    Q_PROPERTY(QString commitHash READ commitHash NOTIFY commitChanged FINAL)
    Q_PROPERTY(QAbstractItemModel* branchesModel READ branchesModel CONSTANT FINAL)
    Q_PROPERTY(QAbstractItemModel* tagsModel READ tagsModel CONSTANT FINAL)

private:

    git::object_id commit_id { };
    RefsModel m_branches { this };
    RefsModel m_tags { this };

public:

    explicit CreateBranchDialog(QQmlEngine *engine);
    CreateBranchDialog(const CreateBranchDialog &) = delete;
    CreateBranchDialog(CreateBranchDialog &&) = delete;
    ~CreateBranchDialog();

    CreateBranchDialog& operator = (const CreateBranchDialog &) = delete;
    CreateBranchDialog& operator = (CreateBranchDialog &&) = delete;

    QAbstractItemModel* branchesModel() { return &m_branches; }
    QAbstractItemModel* tagsModel() { return &m_tags; }

private:

    git::commit lookup_target();

private slots:

    void on_pbOk_clicked();

public:

    QString commitHash() { return commit_id.toString(); }

    void setRepositiory(git::repository *r);
    void setCommitId(const git::object_id &id);

    Q_INVOKABLE void accept();

signals:

    void commitChanged();

};
