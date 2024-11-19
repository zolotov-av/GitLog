#pragma once

#include "QuickDialog.h"
#include <QQmlEngine>
#include <QDialog>
#include <GitTools/base.h>
#include <GitTools/GitLogModel.h>

class CreateBranchDialog: public QuickDialog
{
    Q_OBJECT
    Q_PROPERTY(QString commitHash READ commitHash NOTIFY commitChanged FINAL)

private:

    git::object_id commit_id { };
    git::repository *repo { nullptr };
    GitLogModel *model { nullptr };

public:

    explicit CreateBranchDialog(QQmlEngine *engine);
    CreateBranchDialog(const CreateBranchDialog &) = delete;
    CreateBranchDialog(CreateBranchDialog &&) = delete;
    ~CreateBranchDialog();

    CreateBranchDialog& operator = (const CreateBranchDialog &) = delete;
    CreateBranchDialog& operator = (CreateBranchDialog &&) = delete;

private:

    git::commit lookup_target();

private slots:

    void on_pbOk_clicked();

public:

    QString commitHash() { return commit_id.toString(); }

    void setModel(GitLogModel *m);
    void setRepositiory(git::repository *r);
    void setCommitId(const git::object_id &id);

    Q_INVOKABLE void accept();

signals:

    void commitChanged();

};
