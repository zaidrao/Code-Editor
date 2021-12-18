#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QDebug>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>
#include <QTabBar>
#include "cStyle.h"

int MainWindow::newfileCount = 1;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
    connections();
    setAcceptDrops(true);
    updateStatusBar();
    findRe = new FindNReplace(this);
    m_wordWrap = false;
    updateTitle();
}

MainWindow::~MainWindow()
{
    delete ui;
//    for (int i=0;i<highlighterList.size() ;i++ )
//    {
//        Highlighter *high = highlighterList[i];
//        delete high;
//    }
}

void MainWindow::newFile()
{
    codeeditor *editor = new codeeditor(this);
    QString name = "New " + QString::number(newfileCount++);
    ui->tabWidget->addTab(editor,name);
    editor->setName(name);
    addConnection(editor);
    editorList.append(editor);
    ui->tabWidget->setCurrentIndex(editorList.size()-1);
}

void MainWindow::openFile()
{
    QString path = QFileDialog::getOpenFileName(this,"Open File");
    openFilex(path);
}

void MainWindow::save()
{
    saveFile(editorList[ui->tabWidget->currentIndex()]->path());
}

void MainWindow::saveFile(QString path)
{
    if(path.isEmpty())
    {
        saveAs();
        return;
    }

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this,"Error", file.errorString());
        ui->statusbar->showMessage("Error could not save file!");
        saveAs();
        return;
    }

    QTextStream stream(&file);
    stream << editorList[ui->tabWidget->currentIndex()]->toPlainText();
    file.close();

    editorList[ui->tabWidget->currentIndex()]->setPath(path);
}

void MainWindow::checkSyntax(QString formate)
{
   if (formate.toLower() == QString("c") || formate.toLower() == QString("cpp") || formate.toLower() == QString("h"))
       emit ui->actionC_C->triggered(true);
}

void MainWindow::syntaxActionCheck()
{
    if(editorList.size() == 0) return; //check List size otherwise could get error
    if(editorList[ui->tabWidget->currentIndex()]->getSyntax() == codeeditor::c)
    {
        ui->actionC_C->setIcon(QIcon(":/img/icons/dot.png"));
        ui->actionAuto_Indentation->setEnabled(true);
        ui->actionRemove_Indentation->setEnabled(true);
    }
    else
    {
        ui->actionC_C->setIcon(QIcon(""));
        ui->actionAuto_Indentation->setEnabled(false);
        ui->actionRemove_Indentation->setEnabled(false);
    }
}

void MainWindow::updateStatusBar()
{
    QString le = QString("Length : "+QString::number(editorList[ui->tabWidget->currentIndex()]->document()->characterCount()));
    QString li = QString("Lines : "+QString::number(editorList[ui->tabWidget->currentIndex()]->document()->lineCount()));

    QString fileType;
    if(editorList[ui->tabWidget->currentIndex()]->getSyntax() == codeeditor::c) fileType = "C++ File";
    else fileType = "Normal File";

    file->setText(fileType);
    length->setText(le);
    lines->setText(li);

    ui->statusbar->addWidget(file,1);
    ui->statusbar->addWidget(lines);
    ui->statusbar->addWidget(length);
}

void MainWindow::addConnection(codeeditor *editor)
{
    connect(editor,&QPlainTextEdit::textChanged,this,&MainWindow::updateStatusBar);
}

void MainWindow::updateWrap()
{
    if(editorList.size() > 0 && m_wordWrap == true)
    {
        for (int i=0; i < editorList.size() ;i++ )
        {
            editorList[i]->setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        }
    }
    if(editorList.size() > 0 && m_wordWrap == false)
    {
        for (int i=0; i < editorList.size() ;i++ )
        {
            editorList[i]->setWordWrapMode(QTextOption::NoWrap);
        }
    }
}

void MainWindow::updateTitle()
{
    if(editorList.size()> 0)
    {
        setWindowTitle(editorList[ui->tabWidget->currentIndex()]->name());
    }
}

void MainWindow::openFilex(QString path)
{
    if(path.isEmpty()) return;

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(this,"Error", file.errorString());
        return;
    }

    QTextStream stream(&file);
    codeeditor *editor = new codeeditor(this);
    editor->setPlainText(stream.readAll());
    QFileInfo fileInfo(path);
    editor->setName(fileInfo.fileName());
    ui->tabWidget->addTab(editor,fileInfo.fileName());
    addConnection(editor);
    editorList.append(editor);
    file.close();

    //set cuurent tab of open file
    ui->tabWidget->setCurrentIndex(editorList.size()-1);
    checkSyntax(fileInfo.suffix());

    editor->setIsChanged(false);
    editor->setPath(path);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    foreach(const QUrl &url, event->mimeData()->urls())
    {
        openFilex(url.toLocalFile());
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(editorList.size()>0)
    {
        for (int i =0; i< editorList.size() ;i++ )
        {
            if(!isSave(editorList[i]))
            {
                event->ignore();
                return;
            }
        }
        event->accept();
    }
}

void MainWindow::saveAs()
{
    QString path = QFileDialog::getSaveFileName(this,"Save File",editorList[ui->tabWidget->currentIndex()]->name());
    if(path.isEmpty()) return;
    saveFile(path);
}

void MainWindow::copy()
{
    editorList[ui->tabWidget->currentIndex()]->copy();
}

void MainWindow::cut()
{
    editorList[ui->tabWidget->currentIndex()]->cut();
}

void MainWindow::paste()
{
    editorList[ui->tabWidget->currentIndex()]->paste();
}

void MainWindow::selectAll()
{
    editorList[ui->tabWidget->currentIndex()]->selectAll();
}

void MainWindow::undo()
{
    editorList[ui->tabWidget->currentIndex()]->undo();
}

void MainWindow::redo()
{
    editorList[ui->tabWidget->currentIndex()]->redo();
}

void MainWindow::exit()
{
    close();
}

void MainWindow::closeTab(int index)
{
    codeeditor *edit = editorList[index];
   //   qDebug()<<"run"<<index<<":"<<editorList.size();
    if(isSave(edit))
    {
        editorList.removeAt(index);
        delete edit;
    }

    if(editorList.size()==0) newFile();
}

void MainWindow::zoomIn()
{
    editorList[ui->tabWidget->currentIndex()]->zoomIn();
}

void MainWindow::zoomOut()
{
    editorList[ui->tabWidget->currentIndex()]->zoomOut();
}

void MainWindow::help()
{
    QDesktopServices::openUrl(QUrl("https://github.com/zaidrao/Code-Editor"));
}

void MainWindow::about()
{
    ab->show();
}

void MainWindow::find()
{
    findRe->setEditor(editorList[ui->tabWidget->currentIndex()]);
    findRe->setIndex(0);
    findRe->show();
}

void MainWindow::replace()
{
    findRe->setEditor(editorList[ui->tabWidget->currentIndex()]);
    findRe->setIndex(1);
    findRe->show();
}

void MainWindow::cSyntax()
{
    editorList[ui->tabWidget->currentIndex()]->setSyntax(codeeditor::c);
    syntaxActionCheck();
    setHighlighter();
}

void MainWindow::changeOptions()
{
    if(editorList.size() > 0)
    {
        syntaxActionCheck();
        updateStatusBar();
        updateWrap();
        updateTitle();
    }
}

void MainWindow::setHighlighter()
{
   // qDebug()<<"run";
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editorList[ui->tabWidget->currentIndex()]->setFont(font);
    Highlighter *highlighter = new Highlighter(Highlighter::c,editorList[ui->tabWidget->currentIndex()]->document());
    highlighterList.append(highlighter);
}

void MainWindow::autoIndent()
{
    cStyle style(editorList[ui->tabWidget->currentIndex()]->toPlainText().toStdString());
    QString doc = QString::fromStdString(style.save());
    QString fileName = editorList[ui->tabWidget->currentIndex()]->name();
    fileName.insert(0,"cStyle_ai_");
    codeeditor *editor = new codeeditor(this);
    editor->setPlainText(doc);
    editor->setName(fileName);
    addConnection(editor);
    editorList.append(editor);
    ui->tabWidget->addTab(editor,fileName);
    ui->tabWidget->setCurrentIndex(editorList.size()-1);
    emit ui->actionC_C->triggered(true);

}

void MainWindow::removeIndent()
{
    cStyle style(editorList[ui->tabWidget->currentIndex()]->toPlainText().toStdString());
    QString doc = QString::fromStdString(style.save(cStyle::rmd_Indent));
    QString fileName = editorList[ui->tabWidget->currentIndex()]->name();
    fileName.insert(0,"cStyle_ri_");
    codeeditor *editor = new codeeditor(this);
    editor->setPlainText(doc);
    editor->setName(fileName);
    addConnection(editor);
    editorList.append(editor);
    ui->tabWidget->addTab(editor,fileName);
    ui->tabWidget->setCurrentIndex(editorList.size()-1);
    emit ui->actionC_C->triggered(true);
}

void MainWindow::wordWrap(bool is)
{
    m_wordWrap = is;
    updateWrap();
}

void MainWindow::init()
{
    setWindowIcon(QIcon(":/img/icons/appic.ico"));
    ui->toolBar->setFloatable(false);
    ui->toolBar->setMovable(false);
    setCentralWidget(ui->tabWidget);
    ui->tabWidget->clear();
    ui->tabWidget->setTabsClosable(true);
    newFile();
    file = new QLabel(this);
    length = new QLabel(this);
    lines = new QLabel(this);
    ab = new class about(this);
}

void MainWindow::connections()
{
    connect(ui->actionNew_File,&QAction::triggered,this,&MainWindow::newFile);
    connect(ui->actionOpen_File,&QAction::triggered,this,&MainWindow::openFile);
    connect(ui->actionSave,&QAction::triggered,this,&MainWindow::save);
    connect(ui->actionSave_As,&QAction::triggered,this,&MainWindow::saveAs);
    connect(ui->actionCopy,&QAction::triggered,this,&MainWindow::copy);
    connect(ui->actionCut,&QAction::triggered,this,&MainWindow::cut);
    connect(ui->actionPaste,&QAction::triggered,this,&MainWindow::paste);
    connect(ui->actionUndo,&QAction::triggered,this,&MainWindow::undo);
    connect(ui->actionRedo,&QAction::triggered,this,&MainWindow::redo);
    connect(ui->actionSelect_All,&QAction::triggered,this,&MainWindow::selectAll);
    connect(ui->tabWidget->tabBar(),&QTabBar::tabCloseRequested,this, &MainWindow::closeTab);
    connect(ui->actionZoom_In,&QAction::triggered,this,&MainWindow::zoomIn);
    connect(ui->actionZoom_Out,&QAction::triggered,this,&MainWindow::zoomOut);
    connect(ui->actionHelp,&QAction::triggered,this,&MainWindow::help);
    connect(ui->actionExit,&QAction::triggered,this,&MainWindow::exit);
    connect(ui->actionAbout_Code_Editor,&QAction::triggered,this,&MainWindow::about);
    connect(ui->actionC_C,&QAction::triggered,this,&MainWindow::cSyntax);
    connect(ui->tabWidget,&QTabWidget::currentChanged,this,&MainWindow::changeOptions);
    connect(ui->actionAuto_Indentation,&QAction::triggered,this,&MainWindow::autoIndent);
    connect(ui->actionRemove_Indentation,&QAction::triggered,this,&MainWindow::removeIndent);
    connect(ui->actionFind,&QAction::triggered,this,&MainWindow::find);
    connect(ui->actionReplace,&QAction::triggered,this,&MainWindow::replace);
    connect(ui->actionWord_wrap,&QAction::triggered,this,&MainWindow::wordWrap);

}

bool MainWindow::isSave(codeeditor *editor)
{
    if(!editor->getIsChanged())
    {
        return true ;
    }
    QString name;
    if (editor->path().isEmpty()) name = editor->name();
    else name = editor->path();
    int value = QMessageBox::question(this,"Save","Save? \""+name+"\"",QMessageBox::StandardButton::Yes,QMessageBox::StandardButton::No,QMessageBox::StandardButton::Cancel);

    if(value == QMessageBox::StandardButton::No) return true;
    if(value == QMessageBox::StandardButton::Cancel) return false;
    if(editor->path().isEmpty())
    {
        saveAs();
        return true;
    }
    else
    {
        saveFile(editor->path());
        return true;
    }
}

