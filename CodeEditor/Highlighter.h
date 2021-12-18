#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H


#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>


class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:

    enum language {text,c};

    Highlighter(language lang,QTextDocument *parent = 0);

protected:
    void highlightBlock(const QString &txt) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    language lang;

    QList<HighlightingRule> highlightingRules;

    QRegularExpression cStarExp;
    QRegularExpression cEndExp;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;

    // private methods
    void selectLanguage ();
    void cRules();
    void initCRules();
    void applyCRules(const QString &text);
    void applyTextRules(const QString &text);
};


#endif // HIGHLIGHTER_H
