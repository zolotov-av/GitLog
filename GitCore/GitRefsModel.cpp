#include "GitRefsModel.h"

GitRefsModel::GitRefsModel(QObject *parent): QAbstractItemModel{parent}
{

}

GitRefsModel::GitRefsModel(const GitRefsModel &other):
    QAbstractItemModel{nullptr}, m_refs(other.m_refs)
{

}

GitRefsModel::GitRefsModel(GitRefsModel &&other):
    QAbstractItemModel{nullptr}, m_refs(other.m_refs)
{

}

GitRefsModel::~GitRefsModel()
{

}

GitRefsModel& GitRefsModel::operator = (const GitRefsModel &other)
{
    m_refs = other.m_refs;
    return *this;
}

GitRefsModel& GitRefsModel::operator = (GitRefsModel &&other)
{
    m_refs = std::move(other.m_refs);
    return *this;
}

int GitRefsModel::rowCount(const QModelIndex &parent) const
{
    return m_refs.size();
}

int GitRefsModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex GitRefsModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !parent.isValid() )
    {
        return createIndex(row, column, 1);
    }

    return QModelIndex{ };
}

QModelIndex GitRefsModel::parent(const QModelIndex &child) const
{
    return QModelIndex{ };
}

QVariant GitRefsModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || index.row() < 0 || index.row() >= m_refs.size() || index.column() != 0 )
        return { };

    switch ( role )
    {
    case RefNameRole:
        return m_refs[index.row()].refName;
    case RefFullNameRole:
        return m_refs[index.row()].refFullName;
    case RefColorRole:
        return m_refs[index.row()].refColor;
    case RefTypeRole:
        return m_refs[index.row()].refType;
    default:
        return { };
    }
}

QHash<int, QByteArray> GitRefsModel::roleNames() const
{
    return {
        {RefNameRole, "refName"},
        {RefFullNameRole, "refFullName"},
        {RefColorRole, "refColor"},
        {RefTypeRole, "refType"}
    };
}

void GitRefsModel::setRepository(GitRepository *r)
{
    if ( m_repo == r )
        return;

    if ( m_repo )
        disconnect(m_repo, &GitRepository::stateChanged, this, &GitRefsModel::update);

    m_repo = r;

    if ( m_repo )
        connect(m_repo, &GitRepository::stateChanged, this, &GitRefsModel::update);

    emit repositoryChanged();

    update();
}

void GitRefsModel::clear()
{
    beginResetModel();
    m_refs.clear();
    endResetModel();
}

static const char* getRefType(const ReferenceInfo &ref)
{
    if ( ref.isHead )
        return "HEAD";

    if ( ref.isBranch )
        return "branch";

    if ( ref.isTag )
        return "tag";

    return "other";
}

static QColor getRefColor(const ReferenceInfo &ref)
{
    const QColor GT_Green("#00B64F");
    const QColor GT_Red("#FF3500");
    const QColor GT_Orange("#FF8900");
    const QColor GT_Blue("#086FA1");
    const QColor GT_Tag("#febc70");

    if ( ref.isHead )
        return GT_Red;

    if ( ref.isBranch )
        return GT_Green;

    if ( ref.isTag )
        return GT_Tag;

    return GT_Orange;
}

void GitRefsModel::append(const ReferenceInfo &ref)
{
    m_refs.append({ref.short_name, ref.name, getRefType(ref), getRefColor(ref)});
}

void GitRefsModel::loadBranches(git::repository *repo)
{
    beginResetModel();

    m_refs.clear();
    if ( repo )
    {
        git_branch_iterator *iter;
        int status = git_branch_iterator_new(&iter, repo->data(), GIT_BRANCH_ALL);
        if ( status == 0 )
        {
            git_reference *ref;
            git_branch_t type;

            while ( true )
            {
                status = git_branch_next(&ref, &type, iter);
                if ( status != 0 )
                {
                    break;
                }
                append(git::reference(ref));
            }

            git_branch_iterator_free(iter);
        }
    }

    endResetModel();
}

void GitRefsModel::loadTags(git::repository *repo)
{
    beginResetModel();

    m_refs.clear();
    if ( repo )
    {
        git_reference_iterator *iter;
        int status = git_reference_iterator_new(&iter, repo->data());
        if ( status == 0 )
        {
            git_reference *item;

            while ( true )
            {
                status = git_reference_next(&item, iter);
                if ( status != 0 )
                {
                    break;
                }

                git::reference ref{item};
                if ( ref.isTag() )
                    append(std::move(ref));
            }

            git_reference_iterator_free(iter);
        }
    }

    endResetModel();
}

void GitRefsModel::update()
{
    if ( m_repo == nullptr || !m_repo->isOpened() )
    {
        clear();
        return;
    }

    beginResetModel();

    m_refs.clear();

    auto iter = m_repo->newReferenceRterator();
    while ( true )
    {
        auto ref = iter.next();
        if ( ref.isNull() )
            break;

        append(std::move(ref));
    }

    endResetModel();
}
