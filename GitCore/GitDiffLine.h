#pragma once

#include <QColor>
#include <QString>
#include <QStringList>

/**
 * @brief Строка в GitDiff
 * @ingroup GitCore
 */
class GitDiffLine
{
public:

	struct LineList
	{
		QStringList lines;
		bool eof_missed { false };

		auto size() const { return lines.size(); }
	};

	int lineNumber;
	QString lineText;
	QColor lineColor;

	static QColor addedColor() { return QColor{"aquamarine"}; }
	static QColor removedColor() { return QColor{"crimson"}; }

	static LineList splitLines(const QByteArray &data);
	static QString joinLines(const QStringList &items);

};

