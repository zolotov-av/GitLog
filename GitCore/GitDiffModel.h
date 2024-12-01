#pragma once

#include <QAbstractListModel>
#include <QQmlEngine>
#include <QColor>
#include <QList>
#include <gitcxx/repository.h>
#include <gitcxx/diff_delta.h>

/**
 * @brief Модель для подсветки строк в DiffArea
 * @ingroup GitCore
 */
class GitDiffModel: public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(QString text READ text NOTIFY textChanged FINAL)

public:

    struct LineInfo
    {
        int lineNumber;
        QString lineText;
        QColor lineColor;
    };

    enum Roles
    {
        LineTextRole = Qt::DisplayRole,
        LineColorRole = Qt::UserRole,
        LineNumberRole
    };

private:

    QList<LineInfo> m_items;
    QString m_text;

public:

    explicit GitDiffModel(QObject *parent = nullptr);
    GitDiffModel(const GitDiffModel &) = delete;
    GitDiffModel(GitDiffModel &&) = delete;
    ~GitDiffModel();

    GitDiffModel& operator = (const GitDiffModel &) = delete;
    GitDiffModel& operator = (GitDiffModel &&) = delete;

    QModelIndex index(int row, int column = 0, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void loadFromFile(const QString &path);
    void setContent(const QByteArray &data);

    static QColor addedColor() { return QColor{"aquamarine"}; }
    static QColor removedColor() { return QColor{"crimson"}; }

    void setLineColor(int row, QColor color);

    Q_INVOKABLE void clear();
    void addLine(const QString &text, QColor color);

    void setDiffBuffers(const QByteArray &left, const QByteArray &right);
    void setGitDelta(git::repository *repo, git::diff_delta delta, bool isWorktree);

    QString text() { return m_text; }

signals:

    void textChanged();

};
