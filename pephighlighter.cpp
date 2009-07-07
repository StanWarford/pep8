#include "pephighlighter.h"

PepHighlighter::PepHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    oprndFormat.setForeground(Qt::darkBlue);
    oprndFormat.setFontWeight(QFont::Bold);
    QStringList oprndPatterns;
    oprndPatterns << "\\bADD(A|SP|X)\\b" << "\\bAND(A|X)\\b" << "\\bASL(A|X)\\b"
            << "\\bASR(A|X)\\b" << "\\bBR\\b" << "\\bBR(C|EQ|GE|GT|LE|LT|NE|V)\\b"
            << "\\bCALL\\b" << "\\bCHAR(I|O)\\b" << "\\bCP(A|X)\\b"
            << "\\bDEC(I|O)\\b" << "\\bLD(A|X|BYTEA|BYTEX)\\b" << "\\bMOV(FLGA|SPA)\\b"
            << "\\bNEG(A|X)\\b" << "\\bNOP\\b" << "\\bNOP(0|1|2|3)\\b"
            << "\\bNOT(A|X)\\b" << "\\bOR(A|X)\\b" << "\\bRET(0|1|2|3|4|5|6|7|TR)\\b"
            << "\\bRO|LA|LX|RA|RX)\\b" << "\\bST(A|X|BYTEA|BYTEX)\\b" << "\\bSTOP\\b"
            << "\\bSTRO\\b" << "\\bSTX\\b" << "\\bSUB(A|X|SP)\\b";
    foreach (const QString &pattern, oprndPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = oprndFormat;
        highlightingRules.append(rule);
    }

    dotFormat.setForeground(Qt::darkBlue);
    dotFormat.setFontItalic(true);
    QStringList dotPatterns;
    dotPatterns << "[\\.]\\bEQUATE\\b" << "[\\.]\\bASCII\\b" << "[\\.]\\bBLOCK\\b"
            << "[\\.]\\bBURN\\b" << "[\\.]\\bBYTE\\b" << "[\\.]\\bEND\\b"
            << "[\\.]\\bEQUATE\\b" << "[\\.]\\bWORD\\b" << "[\\.]\\bADDRSS\\b";
    foreach (const QString &pattern, dotPatterns) {
        rule.pattern = QRegExp(pattern);
        rule.format = dotFormat;
        highlightingRules.append(rule);
    }

    symbolFormat.setFontWeight(QFont::Bold);
    symbolFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("[A-Za-z0-9_]+(?=:)");
    rule.format = symbolFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp(";.*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::white);
    multiLineCommentFormat.setBackground(Qt::red);

    singleQuotationFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("((\')(?![\'])(([^\'|\\\\]){1}|((\\\\)([\'|b|f|n|r|t|v|\"|\\\\]))|((\\\\)(([x|X])([0-9|A-F|a-f]{2}))))(\'))");
    rule.format = singleQuotationFormat;
    highlightingRules.append(rule);

    doubleQuotationFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("((\")((([^\"|\\\\])|((\\\\)([\'|b|f|n|r|t|v|\"|\\\\]))|((\\\\)(([x|X])([0-9|A-F|a-f]{2}))))*)(\"))");
    rule.format = doubleQuotationFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp(";ERROR:[\\s]");
    commentEndExpression = QRegExp("$");
}

void PepHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        expression.setCaseSensitivity(Qt::CaseInsensitive);
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
