#include "repository.h"
#include "exception.h"
#include <QDir>
#include <QFile>

namespace git
{

    repository::repository(repository &&other): m_repo(other.m_repo)
    {
        other.m_repo = nullptr;
    }

    repository::repository(const QString &path)
    {
        open(path);
    }

    repository::~repository()
    {
        git_repository_free(m_repo);
    }

    repository& repository::operator = (repository &&other)
    {
        git_repository_free(m_repo);
        m_repo = other.m_repo;
        other.m_repo = nullptr;
        return *this;
    }

    void repository::open(const QString &path)
    {
        close();
        const auto utf8_path = path.toUtf8();
        check( git_repository_open(&m_repo, utf8_path.constData()) );
    }

    void repository::close()
    {
        git_repository_free(m_repo);
        m_repo = nullptr;
    }

    config repository::getConfig()
    {
        git_config *cfg { nullptr };
        check( git_repository_config(&cfg, m_repo) );

        return config{ cfg };
    }

    index repository::getIndex()
    {
        git_index *idx { nullptr };
        check( git_repository_index(&idx, m_repo) );
        return index{ idx };
    }

    QString repository::workdir()
    {
        return QString::fromUtf8( git_repository_workdir(m_repo) );
    }

    reference repository::head()
    {
        git_reference *ref { nullptr };
        check( git_repository_head(&ref, m_repo) );
        return reference{ ref };
    }

    reference::iterator repository::newReferenceRterator()
    {
        git_reference_iterator *iter { nullptr };
        check( git_reference_iterator_new(&iter, m_repo) );
        return reference::iterator{ iter };
    }

    reference repository::lookupBranch(const QString &name, git_branch_t type)
    {
        const auto utf8_name = name.toUtf8();
        git_reference *ref { nullptr };
        check( git_branch_lookup(&ref, m_repo, utf8_name.constData(), type) );
        return reference{ ref };
    }

    reference repository::lookupReference(const QString &name)
    {
        const auto utf8_name = name.toUtf8();
        git_reference *ref { nullptr };
        check( git_reference_lookup(&ref, m_repo, utf8_name.constData()) );
        return reference{ ref };
    }

    blob repository::lookupBlob(const git_oid *id)
    {
        git_blob *b { nullptr };
        check( git_blob_lookup(&b, m_repo, id) );
        return blob{ b };
    }

    commit repository::lookupCommit(const git_oid *id)
    {
        git_commit *c;
        check( git_commit_lookup(&c, m_repo, id) );
        return c;
    }

    commit repository::lookupCommit(const QString &hash)
    {
        return lookupCommit(object_id{hash});
    }

    commit repository::lookupCommit(const reference &ref)
    {
        return lookupCommit(ref.resolve().target());
    }

    tree repository::lookupTree(const object_id &oid)
    {
        git_tree *out { nullptr };
        check( git_tree_lookup(&out, m_repo, oid.data()) );
        return tree { out };
    }

    revwalk repository::newRevwalk()
    {
        git_revwalk *rw { nullptr };
        check( git_revwalk_new(&rw, m_repo) );
        return rw;
    }

    reference repository::createBranch(const QString &name, const commit &target, bool force)
    {
        const auto utf8_name = name.toUtf8();
        git_reference *ref { nullptr };
        check( git_branch_create(&ref, m_repo, utf8_name.constData(), target.data(), force) );
        return reference{ ref };
    }

    void repository::deleteBranch(reference &&ref)
    {
        ref.deleteBranch();
    }

    void repository::deleteBranch(const QString &name)
    {
        lookupBranch(name).deleteBranch();
    }

    void repository::deleteRefence(reference &&ref)
    {
        ref.deleteReference();
    }

    diff repository::diff()
    {
        git_diff *d { nullptr };
        git_diff_options opts { };
        opts.version = GIT_DIFF_OPTIONS_VERSION;
        opts.flags = GIT_DIFF_INCLUDE_UNTRACKED;
        check( git_diff_index_to_workdir(&d, m_repo, nullptr, &opts) );
        return { d };
    }

    diff repository::diff(const tree &a, const tree &b)
    {
        git_diff *d { nullptr };
        check( git_diff_tree_to_tree(&d, m_repo, a.data(), b.data(), nullptr) );
        return { d };
    }

    diff repository::diff_cached(const tree &a)
    {
        git_diff *d { nullptr };
        check( git_diff_tree_to_index(&d, m_repo, a.data(), nullptr, nullptr) );
        return { d };
    }

    void repository::stageAll(const QString &path, unsigned int flags)
    {
        auto index = getIndex();
        index.addAll(path, flags);
        index.write();
    }

    void repository::restoreStaged(const QString &file)
    {
        auto HEAD = head().peel(GIT_OBJECT_COMMIT);

        // Настраиваем pathspec
        git_strarray pathspec { };
        auto utf8_path = file.toUtf8();
        char *paths[] = { utf8_path.data() };
        pathspec.strings = paths;
        pathspec.count = 1;

        // Сбрасываем файл к состоянию в HEAD
        check( git_reset_default(m_repo, HEAD.data(), &pathspec) );
    }

    void repository::checkoutHead(const QString &file)
    {
        git_checkout_options opts = GIT_CHECKOUT_OPTIONS_INIT;
        opts.checkout_strategy = GIT_CHECKOUT_FORCE;
        git_strarray paths { };
        auto utf8_path = file.toUtf8();
        char *pathspec[] = { utf8_path.data() };
        paths.strings = pathspec;
        paths.count = 1;
        opts.paths = paths;

        check( git_checkout_head(m_repo, &opts) );
    }

    void repository::removeFile(const QString &file)
    {
        auto index = getIndex();
        index.removeByPath(file);
        index.write();

        QFileInfo fileInfo { QDir{workdir()}.filePath(file) };

        if ( fileInfo.isDir() ) {
            QDir dir{fileInfo.filePath()};
            if ( dir.removeRecursively() )
                return;

            throw exception("remove dir failed");
        }

        QFile qFile{ fileInfo.filePath() };
        if ( !qFile.remove() )
        {
            throw exception("%s", qFile.errorString());
        }
    }

    signature repository::signatureDefault()
    {
        git_signature *sig { nullptr };
        check( git_signature_default(&sig, m_repo) );
        return signature{ sig };
    }

    void repository::createCommit(const QString &author_name, const QString &author_email, const QString &message)
    {
        const auto msg = message.toUtf8();

        git_oid commit_id;

        auto author = signature::now(author_name, author_email);

        auto index = getIndex();
        auto tree_id = index.writeTree();
        auto tree = lookupTree(tree_id);
        auto parent = lookupCommit(head());

        const git_commit* parents[1] = { parent.data() };
        git_commit_create(&commit_id, m_repo, "HEAD", author.data(), author.data(), "UTF-8", msg.data(), tree.data(), 1, parents);

        index.readTree(tree);
    }

    object_id repository::createCommit(const QString &message)
    {
        auto author = signatureDefault();
        auto tree = lookupTree(getIndex().writeTree());
        auto parent = lookupCommit(head());
        const git_commit* parents[1] = { parent.data() };
        const auto utf8_msg = message.toUtf8();

        git_oid commit_id { };
        git_commit_create(&commit_id, m_repo, "HEAD", author.data(), author.data(), "UTF-8", utf8_msg.constData(), tree.data(), 1, parents);

        return object_id{ &commit_id };
    }

    object_id repository::amendCommit(const QString &message)
    {
        auto tree = lookupTree(getIndex().writeTree());
        auto commit = lookupCommit(head());
        const auto utf8_message = message.toUtf8();

        git_oid commit_id { };
        check( git_commit_amend(&commit_id, commit.data(), "HEAD", nullptr, nullptr, nullptr, utf8_message.constData(), tree.data()) );

        return object_id{ &commit_id };
    }

}
