#ifndef PEPHIGHLIGHTER_H
#define PEPHIGHLIGHTER_H

#include <QSyntaxHighlighter>

#include <QHash>
#include <QTextCharFormat>

QT_BEGIN_NAMESPACE
class QTextDocument;
QT_END_NAMESPACE

class PepHighlighter : public QSyntaxHighlighter
{
public:
    PepHighlighter(QTextDocument *parent = 0);

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

    QTextCharFormat oprndFormat;
    QTextCharFormat dotFormat;
    QTextCharFormat symbolFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat singleQuotationFormat;
    QTextCharFormat doubleQuotationFormat;

};

#endif // PEPHIGHLIGHTER_H
