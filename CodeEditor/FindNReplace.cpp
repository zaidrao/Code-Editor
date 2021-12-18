#include "FindNReplace.h"
#include "ui_FindNReplace.h"
#include <QPushButton>
#include <QMessageBox>

FindNReplace::FindNReplace(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindNReplace)
{
    ui->setupUi(this);
    init();
    QPixmap pix(8,8);
    pix.fill(Qt::transparent);
    setWindowIcon(QIcon(pix));
    setWindowTitle("Find & Replace");
}

FindNReplace::~FindNReplace()
{
    delete ui;
}

void FindNReplace::setIndex(int ind)
{
    ui->tabWidget->setCurrentIndex(ind);
}

void FindNReplace::setEditor(QPlainTextEdit *edit)
{
    editor = edit;
}

void FindNReplace::init()
{

    QPushButton *btnFindNext = new QPushButton("Next",this);
    connect(btnFindNext,&QPushButton::clicked,this,&FindNReplace::findNext);
    ui->buttonBox->addButton(btnFindNext,QDialogButtonBox::ActionRole);
    QPushButton *btnFindPre = new QPushButton("Previous",this);
    connect(btnFindPre,&QPushButton::clicked,this,&FindNReplace::findPre);
    ui->buttonBox->addButton(btnFindPre,QDialogButtonBox::ActionRole);

    QPushButton *btnReNext = new QPushButton("Next",this);
    QPushButton *btnRePre = new QPushButton("Previous",this);
    connect(btnReNext,&QPushButton::clicked,this,&FindNReplace::reNext);
    connect(btnRePre,&QPushButton::clicked,this,&FindNReplace::rePre);
    ui->buttonBox_2->addButton(btnReNext,QDialogButtonBox::ActionRole);
    ui->buttonBox_2->addButton(btnRePre,QDialogButtonBox::ActionRole);
    QPushButton *btnReplace = new QPushButton("Replace",this);
    connect(btnReplace,&QPushButton::clicked,this,&FindNReplace::replace);
    ui->buttonBox_2->addButton(btnReplace,QDialogButtonBox::ActionRole);
}

void FindNReplace::findNext()
{
    m_findText = ui->lineEditFind->text();
    m_caseSensitive = ui->cbCase->isChecked();
    m_wholeWords = ui->cbWholeWord->isChecked();
    m_backwards = false;
    find();
}

void FindNReplace::findPre()
{
    m_findText = ui->lineEditFind->text();
    m_caseSensitive = ui->cbCase->isChecked();
    m_wholeWords = ui->cbWholeWord->isChecked();
    m_backwards = true;
    find();
}

void FindNReplace::find()
{
    QTextDocument::FindFlags flags;
    if(m_caseSensitive) flags = flags | QTextDocument::FindFlag::FindCaseSensitively;
    if(m_wholeWords) flags = flags | QTextDocument::FindFlag::FindWholeWords;
    if(m_backwards) flags = flags | QTextDocument::FindFlag::FindBackward;

    bool value = editor->find(m_findText,flags);
    if(!value) QMessageBox::information(this,"Not Found","Could not found \"" + m_findText + "\"");
}

void FindNReplace::replace()
{
    m_replaceText = ui->lineEditReplace->text();
    QTextCursor cursor = editor->textCursor();
    cursor.insertText(m_replaceText);
}

void FindNReplace::reNext()
{
    m_findText = ui->lineEditFind_2->text();
    m_caseSensitive = ui->cbCase_2->isChecked();
    m_wholeWords = ui->cbWord->isChecked();
    m_backwards = false;
    find();
}

void FindNReplace::rePre()
{
    m_findText = ui->lineEditFind_2->text();
    m_caseSensitive = ui->cbCase_2->isChecked();
    m_wholeWords = ui->cbWord->isChecked();
    m_backwards = true;
    find();
}


void FindNReplace::on_buttonBox_2_rejected()
{
    close();
}


void FindNReplace::on_buttonBox_rejected()
{
    close();
}

