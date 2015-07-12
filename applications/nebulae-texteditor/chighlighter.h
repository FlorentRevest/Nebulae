/*
 * This file is part of Nebulae.
 * Copyright (C) 2011 - Leo Testard <leo.testard@gmail.com>
 *
 * Nebulae is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Nebulae is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Nebulae. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CHIGHLIGHTER_H
#define CHIGHLIGHTER_H

#include <QtGui/QSyntaxHighlighter>

class QTextDocument;

class CHighlighter : public QSyntaxHighlighter
{
public:
    CHighlighter(QTextDocument *parent = 0);
protected:
    void highlightBlock(const QString &text);

private:
    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;
    QTextCharFormat keywordFormat;
    QTextCharFormat values;
    QTextCharFormat preprocessor;
    QTextCharFormat quotationFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
};

#endif // CHIGHLIGHTER_H
