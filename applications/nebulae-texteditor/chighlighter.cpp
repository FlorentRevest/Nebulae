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

#include "chighlighter.h"

CHighlighter::CHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
            << "\\bdouble\\b" << "\\benum\\b" << "\\bint\\b"
            << "\\blong\\b" << "\\bshort\\b" << "\\bsigned\\b"
            << "\\bstatic\\b" << "\\bstruct\\b" << "\\btypedef\\b"
            << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvoid\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    values.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("[0-9]x?[0-9]*[bodh]?");
    rule.format = values;
    highlightingRules.append(rule);
    QStringList otherValues;
    otherValues << "\\bNULL\\b";
    foreach(QString pattern, otherValues)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = values;
        highlightingRules.append(rule);
    }

    preprocessor.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("#.*");
    rule.format = preprocessor;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("[\"<].*[\">]");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::red);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void CHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
