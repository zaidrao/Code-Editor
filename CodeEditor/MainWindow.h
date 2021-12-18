#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDragEnterEvent>
#include <QLabel>
#include <QMainWindow>
#include "codeeditor.h"
#include "Highlighter.h"
#include "FindNReplace.h"
#include "about.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void newFile();
    void openFile();
    void save();
    void saveAs();
    void copy();
    void cut();
    void paste();
    void selectAll();
    void undo();
    void redo();
    void exit();
    void closeTab(int index);
    void zoomIn();
    void zoomOut();
    void help();
    void about();
    void find();
    void replace();
    void cSyntax();
    void changeOptions();
    void setHighlighter();
    void autoIndent();
    void removeIndent();
    void wordWrap(bool is);
private:
    Ui::MainWindow *ui;

    //private parametrs
    QList<codeeditor* > editorList;
    static int newfileCount;
    QList<Highlighter*> highlighterList;
    FindNReplace *findRe;
    bool m_wordWrap;
    class about *ab;

    QLabel *file;
    QLabel *length;
    QLabel *lines;

    //private methods
    void init();
    void connections();
    bool isSave(codeeditor* editor);
    void saveFile(QString path);
    void checkSyntax(QString formate);
    void syntaxActionCheck();
    void updateStatusBar();
    void addConnection(codeeditor *editor);
    void updateWrap();
    void updateTitle();
    void openFilex(QString path);



    // QWidget interface
protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void closeEvent(QCloseEvent *event);
};
#endif // MAINWINDOW_H
