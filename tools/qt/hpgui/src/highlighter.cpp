#include "highlighter.h"






 Highlighter::Highlighter(QTextDocument *parent)
     : QSyntaxHighlighter(parent)
 {
     HighlightingRule rule;

     {
         predefinedVars.setForeground(Qt::darkCyan);
         QStringList keywordPatterns;
         keywordPatterns << "__any__" << "__isalpha__"
                         << "__islower__" << "__isupper__"
                         << "__isdigit__" << "__isalnum__"
                         << "__endl__" << "__isspace__"
                         << "__isspace*__" << "__isspace+__"
                         << "__space_tab__"
                         ;
         foreach (const QString &pattern, keywordPatterns) {
             rule.pattern = QRegExp(pattern);
             rule.format = predefinedVars;
             highlightingRules.append(rule);
         }
    }



     {
         keywordFormat.setForeground(Qt::darkRed);
         keywordFormat.setFontWeight(QFont::Bold);
         QStringList keywordPatterns;
         keywordPatterns << " ::= " << " ##transf2->";
         keywordPatterns << "__BEGIN_TEMPLATE__::";
         keywordPatterns << "__END_TEMPLATE__::";
            foreach (const QString &pattern, keywordPatterns) {
                 rule.pattern = QRegExp(pattern);
                 rule.format = keywordFormat;
                 highlightingRules.append(rule);
             }
    }



     //nonTerminalSymbol.setFontWeight(QFont::Bold);
     nonTerminalSymbol.setForeground(Qt::darkBlue);
     rule.pattern = QRegExp("(^| )[A-Z][^ ]+($|)");
     rule.format = nonTerminalSymbol;
     highlightingRules.append(rule);


     singleLineCommentFormat.setForeground(Qt::darkGreen);
     rule.pattern = QRegExp("//[^\n]*");
     rule.format = singleLineCommentFormat;
     highlightingRules.append(rule);


     expandVar.setForeground(Qt::darkMagenta);
     rule.pattern = QRegExp("\\$\\([^\\)]+\\)");
     rule.format = expandVar;
     highlightingRules.append(rule);

     {
         expandPredefVar.setForeground(Qt::darkGreen);
         QStringList keywordPatterns;
         keywordPatterns << "\\$\\(__endl__\\)" << "\\$\\(__nothing__\\)"
                         << "\\$\\(__endl__\\)" << "\\$\\(__prune__\\)"
                         << "\\$\\(__space__\\)" << "\\$\\(__counter__\\)"
                         << "\\$\\(__ident\\+__\\)"  << "\\$\\(__ident-__\\)"
                         <<  R"(\$\(__date_time__\))"  <<  R"(\$\(__date__\))"
                         <<  R"(\$\(__run__\))"
                         //<<  R"(\$\(__get__ +[A-Z][A-Z_0-9]* *\))"
                         <<  R"(\$\(__set__ +[A-Z][A-Z_0-9]* +[A-Z][A-Z_0-9]* *\))"
                         <<  R"(\$\(__copy__ +[A-Z][A-Z_0-9]* +[A-Z][A-Z_0-9]* *\))"
                         ;
         foreach (const QString &pattern, keywordPatterns) {
             rule.pattern = QRegExp(pattern);
             rule.format = expandPredefVar;
             highlightingRules.append(rule);
         }
    }
     /*
     expandPredefVar.setFontItalic(true);
     expandPredefVar.setForeground(Qt::darkMagenta);
     rule.pattern = QRegExp("\\$\\(__[^\\)]+__\\)");
     rule.format = expandPredefVar;
     highlightingRules.append(rule);
    */

 }


 void Highlighter::highlightBlock(const QString &text)
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
 }
