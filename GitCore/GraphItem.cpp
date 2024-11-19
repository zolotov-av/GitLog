#include "GraphItem.h"
#include <QPainter>
#include <GitTools/GitLogModel.h>

GraphItem::GraphItem(QQuickItem *parent): QQuickPaintedItem{parent}
{
    setAntialiasing(false);
}

GraphItem::~GraphItem()
{
}

void GraphItem::setColor(QColor c)
{
    if ( m_color == c )
        return;

    m_color = c;
    emit colorChanged();
}

void GraphItem::setModel(QAbstractItemModel *m)
{
    if ( m_model == m )
        return;

    m_model = m;
    emit modelChanged();
}

void GraphItem::setIndex(int idx)
{
    if ( m_index == idx )
        return;

    m_index = idx;
    emit indexChanged();
}

void GraphItem::setTime(const QDateTime &tm)
{
    if ( m_time == tm )
        return;

    m_time = tm;
    emit timeChanged();
}

void GraphItem::paint(QPainter *p)
{
    using GraphLane = git::GraphLane;

    const auto model = qobject_cast<GitLogModel*>(m_model);
    if ( model == nullptr )
        return;

    git::CommitInfo commit = model->commitInfoByIndex(m_index);
    if ( !commit.isCommit() )
        return;

    const int rowHeight = height();
    const int spacing = QFontMetrics{p->font()}.lineWidth() * 5 / 2;

    const auto rect = boundingRect();
    //p->fillRect(rect, Qt::white);

    p->save();
    p->setClipRect(rect, Qt::IntersectClip);
    p->translate(rect.topLeft());

    const auto size = spacing * 5;
    const auto offset = (rowHeight - size) / 2;
    const auto half = rowHeight / 2;
    const auto xlane = commit.lane * rowHeight;

    p->setPen(QPen{m_color, spacing * 2.0, Qt::SolidLine});
    for(int li = 0; li < commit.lanes.size(); li++)
    {
        auto lane = commit.lanes[li];
        if ( !lane.isFree() && li != commit.lane )
        {
            int offset = li * rowHeight;
            switch (lane.type)
            {
            case GraphLane::start:
                p->drawLine(offset, half, offset + half, rowHeight);
                break;
            case GraphLane::line:
                p->drawLine(offset + half, 0, offset + half, rowHeight);
                break;
            case GraphLane::end:
                p->drawLine(offset + half, 0, offset, half);
                break;
            default:
                p->drawLine(offset + half, rowHeight, offset + half*2, half);
            }

        }
    }

    p->setPen(Qt::NoPen);
    p->setBrush(m_color);

    if ( commit.isMerge() )
    {
        p->drawRect(xlane + offset, offset, size, size);
    }
    else
    {
        p->drawEllipse(xlane + offset, offset, size, size);
    }

    p->setPen(QPen{m_color, spacing * 2.0, Qt::SolidLine});

    if ( commit.down )
    {
        p->drawLine(xlane + half, half, xlane + half, rowHeight);
    }

    if ( commit.up )
    {
        p->drawLine(xlane + half, half, xlane + half, 0);
    }

    if ( commit.isMerge() || commit.isBranch() )
    {
        int i = commit.lanes.size()-1;
        while ( i > commit.lane && (commit.lanes[i].type == GraphLane::free || commit.lanes[i].type == GraphLane::line) ) i--;
        p->drawLine(xlane + half, half, xlane + rowHeight + (i - commit.lane - 1)*rowHeight, half);
        //qDebug() << "merge " << commit.message() << "lane:" << commit.lane << "last:" << i;
    }

    p->restore();
}
