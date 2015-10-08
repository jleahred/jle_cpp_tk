#ifndef __HIGHLIGHTER_H
#define __HIGHLIGHTER_H

#include <QSyntaxHighlighter>



class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

 public:
    Highlighter(QTextDocument *parent = 0);

 protected:
     void highlightBlock(const QString &text);

 private:
     struct HighlightingRule
     {
         QRegExp pattern;
         QTextCharFormat format;
     };
     QVector<HighlightingRule> highlightingRules;

     QTextCharFormat predefinedVars;
     QTextCharFormat keywordFormat;
     QTextCharFormat nonTerminalSymbol;
     QTextCharFormat singleLineCommentFormat;
     QTextCharFormat expandVar;
     QTextCharFormat expandPredefVar;
     QTextCharFormat templateFormat;
};

#endif // HIGHLIGHTER_H
