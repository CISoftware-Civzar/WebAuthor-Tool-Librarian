#ifndef LIBRARIANLISTMODEL_H
#define LIBRARIANLISTMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QTextStream>
#include <QMessageBox>
#include <QFile>

class LibrarianListModel : public QAbstractListModel
{
public:
    LibrarianListModel( const QString &filename = NULL, QObject *parent = 0 );
    QStringList librarian_item_names;
    QStringList librarian_item_contents;

    int rowCount(const QModelIndex &parent) const Q_DECL_OVERRIDE;
    QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;

    QString getItemNameAt( int index );
    QString getItemContentAt( int index );
    void dataReset();

    void insertData( QString name, QString content );
    void removeData( int index );

    //librarian IO operations
    void loadLibrarianFile( const QString &filename );
    void saveLibrarianFile( const QString &filename);

private:
    int counter;
};

#endif // LIBRARIANLISTMODEL_H
