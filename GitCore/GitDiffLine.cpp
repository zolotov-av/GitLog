#include "GitDiffLine.h"
#include <QBuffer>
#include <awCore/trace.h>

GitDiffLine::LineList GitDiffLine::splitLines(const QByteArray &data)
{
	QByteArray data2 { data };
	QBuffer file(&data2);
	if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
		LineList items;

		while ( !file.atEnd() )
		{
			const QString line = QString::fromUtf8( file.readLine() );
			if ( line.endsWith(QChar{'\n'}) )
			{
				items.lines.append(line.mid(0, line.length()-1));
			}
			else
			{
				items.lines.append(line);
			}
		}

		return items;
	}
	else
	{
		aw::trace::log("Could not open file: " + file.errorString());

		return { };
	}
}

QString GitDiffLine::joinLines(const QStringList &items)
{
	int size = items.size();
	for(const auto &item : items)
	{
		size += item.length();
	}

	QString text;
	text.reserve(size);

	for(const auto &item : items)
	{
		text.append(item).append(QChar{'\n'});
	}

	return text;
}
