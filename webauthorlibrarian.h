#ifndef WEBAUTHORLIBRARIAN_H
#define WEBAUTHORLIBRARIAN_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QListView>
#include <QTextEdit>
#include <QStatusBar>
#include <QToolBar>
#include <QAction>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QTextBrowser>

#include "librarianlistmodel.h"

class WebAuthorLibrarian : public QMainWindow
{
    Q_OBJECT

public:
    WebAuthorLibrarian(QWidget *parent = 0);
    LibrarianListModel *model;

    QListView *librarianList;
    QTextEdit *librarianEntry;
    QTextBrowser *librarianPreview;

    QWidget *nameEditor;
    QLineEdit *lineEditName;

    QAction *addItem;
    QAction *removeItem;

    void updateList();
    void showEntryCount();

private slots:
    void AddNewItem();
    void RemoveItem();
    void SaveItem();
    void showNameEditor();
    void setName();

    void newDocument();
    void openLoadDialog();
    void openSaveDialog();

    void handleLoadDialog(const QString &filename);
    void handleSaveDialog(const QString &filename);

    void handleIndexChange( const QModelIndex &index );
private:
    int selected;
    QStatusBar *statusbar;
    QString currentItemName;
    bool switched = true;
};

#endif // WEBAUTHORLIBRARIAN_H
