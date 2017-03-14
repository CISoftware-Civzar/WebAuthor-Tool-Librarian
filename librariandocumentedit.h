#ifndef LIBRARIANDOCUMENTEDIT_H
#define LIBRARIANDOCUMENTEDIT_H

#include <QTextEdit>
#include <QTextCursor>
#include <QAction>
#include <QToolBar>

class LibrarianDocumentEdit : public QTextEdit
{
    Q_OBJECT
public:
    LibrarianDocumentEdit( QWidget *parent = 0 );
    QToolBar *getEditingToolbar();
private:
    QToolBar *editingToolbar;
    void modifyTextTag( QString newTag );
private slots:
    void makeNormal();
    void makeBold();
    void makeItalic();
    void makeUnderline();
    void makeHead();
    void makePara();
};

#endif // LIBRARIANDOCUMENTEDIT_H
