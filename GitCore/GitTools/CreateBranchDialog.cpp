#include "CreateBranchDialog.h"
#include <QQmlContext>

CreateBranchDialog::CreateBranchDialog(QQmlEngine *engine): QuickDialog{engine}
{
    qDebug() << "create CreateBranchDialog";
    //ui.setupUi(this);

    const auto ctx = context();
    ctx->setContextProperty("form", this);
    load(engine, QUrl{"qrc:/qt/qml/GitCore/qml/CreateBranchDialog.qml"});

    //ui.rbCommit->setChecked(true);
    //connect(this, SIGNAL(accepted()), this, SLOT(on_accepted()));
}

CreateBranchDialog::~CreateBranchDialog()
{
    qDebug() << "destroy CreateBranchDialog";
}

git::commit CreateBranchDialog::lookup_target()
{
    /*
    if ( ui.rbHead->isChecked() )
    {
        qDebug() << "on HEAD";

        return repo->get_commit( repo->get_head().target() );
    }

    if ( ui.rbBranch->isChecked() )
    {
        const QString on_branch = ui.cbBranch->currentText();
        qDebug() << QString("on other branch: '%1'").arg(on_branch);
        return repo->get_commit( repo->get_branch(on_branch).resolve().target() );
    }

    if ( ui.rbTag->isChecked() )
    {
        const QString on_tag = ui.cbTag->currentText();
        qDebug() << QString("on tag: '%1'").arg(on_tag);
        return repo->get_commit( repo->get_reference(on_tag).resolve().target() );
    }

    if ( ui.rbCommit->isChecked() )
    {
        const QString on_commit = ui.cbCommit->currentText();
        qDebug() << QString("on tag: '%1'").arg(on_commit);
        return repo->get_commit(on_commit);
    }
    */

    throw git::exception("nothing selected");

}

void CreateBranchDialog::on_pbOk_clicked()
{
    qDebug() << "on_pbOk_clicked()";

    /*
    try
    {
        const QString name = ui.leBranch->text();
        qDebug() << QString("create branch: %1").arg(name);
        if ( name.isEmpty() )
        {
            ExceptionTooltip::showBelow(ui.leBranch, "Error", "branch name is empty");
            return;
        }

        const bool force = ui.cbForce->checkState();
        qDebug() << QString("force: %1").arg(force ? "yes" : "no");

        const git::commit target = lookup_target();
        repo->create_branch(name, target, force);

        if ( model ) model->update();

        if ( ui.cbSwitchToNew->checkState() )
        {
            ExceptionTooltip::showBelow(ui.leBranch, "TODO", "switch not implemented yet");
        }

        //accept();

    }
    catch (const std::exception &e)
    {
//        ExceptionTooltip::show(this, e);
    }
    */
}

void CreateBranchDialog::setModel(GitLogModel *m)
{
    model = m;
}

void CreateBranchDialog::setRepositiory(git::repository *r)
{
    repo = r;
    if ( repo == nullptr ) throw git::exception("setRepository(null)");

    git_reference_iterator *iter;
    int status = git_reference_iterator_new(&iter, repo->data());
    if ( status == 0 )
    {
        git_reference *ref;

        while ( true )
        {
            status = git_reference_next(&ref, iter);
            if ( status != 0 )
            {
                break;
            }
            const git::reference r = ref;
            if ( r.isBranch() )
            {
               // ui.cbBranch->addItem(r.shortName());
            }
            else if ( r.isTag() )
            {
               // ui.cbTag->addItem(r.shortName());
            }
        }

        git_reference_iterator_free(iter);
    }

}

void CreateBranchDialog::setCommitId(const git::object_id &id)
{
    commit_id = id;
    emit commitChanged();
}

void CreateBranchDialog::accept()
{
    qDebug().noquote() << "CreateBranchDialog::accept()";
}
