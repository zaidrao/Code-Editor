#ifndef FINDNREPLACE_H
#define FINDNREPLACE_H

#include <QDialog>
#include <QPlainTextEdit>

namespace Ui {
class FindNReplace;
}

class FindNReplace : public QDialog
{
    Q_OBJECT

public:
    explicit FindNReplace(QWidget *parent = nullptr);
    ~FindNReplace();
    void setIndex(int ind);
    void setEditor(QPlainTextEdit *edit);
private slots:
    void findNext();
    void findPre();
    void find();
    void replace();
    void reNext();
    void rePre();

    void on_buttonBox_2_rejected();

    void on_buttonBox_rejected();

private:
    Ui::FindNReplace *ui;

    QString m_findText;
    QString m_replaceText;
    bool m_caseSensitive;
    bool m_wholeWords;
    bool m_backwards;

    QPlainTextEdit *editor;

    void init();
};

#endif // FINDNREPLACE_H
