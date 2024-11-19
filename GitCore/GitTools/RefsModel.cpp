#include "RefsModel.h"

RefsModel::RefsModel(QObject *parent): QAbstractItemModel{parent}
{

}

RefsModel::RefsModel(const RefsModel &other):
    QAbstractItemModel{nullptr}, m_refs(other.m_refs)
{

}

RefsModel::RefsModel(RefsModel &&other):
    QAbstractItemModel{nullptr}, m_refs(other.m_refs)
{

}

RefsModel::~RefsModel()
{

}

RefsModel& RefsModel::operator = (const RefsModel &other)
{
    m_refs = other.m_refs;
    return *this;
}

RefsModel& RefsModel::operator = (RefsModel &&other)
{
    m_refs = std::move(other.m_refs);
    return *this;
}

int RefsModel::rowCount(const QModelIndex &parent) const
{
    return m_refs.size();
}

int RefsModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

QModelIndex RefsModel::index(int row, int column, const QModelIndex &parent) const
{
    if ( !parent.isValid() )
    {
        return createIndex(row, column, 1);
    }

    return QModelIndex{ };
}

QModelIndex RefsModel::parent(const QModelIndex &child) const
{
    return QModelIndex{ };
}

QVariant RefsModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() || index.row() < 0 || index.row() >= m_refs.size() || index.column() != 0 )
        return { };

    switch ( role )
    {
    case RefNameRole:
        return m_refs[index.row()].refName;
    case RefColorRole:
        return m_refs[index.row()].refColor;
    case RefTypeRole:
        return m_refs[index.row()].refType;
    default:
        return { };
    }
}

QHash<int, QByteArray> RefsModel::roleNames() const
{
    return {
        {RefNameRole, "refName"},
        {RefColorRole, "refColor"},
        {RefTypeRole, "refType"}
    };
}

void RefsModel::clear()
{
    beginResetModel();
    m_refs.clear();
    endResetModel();
}

static const char* getRefType(const git::reference_info &ref)
{
    if ( ref.isHead )
        return "HEAD";

    if ( ref.isBranch )
        return "branch";

    if ( ref.isTag )
        return "tag";

    return "other";
}

static QColor getRefColor(const git::reference_info &ref)
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

void RefsModel::append(const git::reference_info &ref)
{
    m_refs.append({ref.short_name, getRefType(ref), getRefColor(ref)});
}

void RefsModel::loadBranches(git::repository *repo)
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

void RefsModel::loadTags(git::repository *repo)
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
