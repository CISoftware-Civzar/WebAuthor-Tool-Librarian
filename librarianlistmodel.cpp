#include "librarianlistmodel.h"
#include <QDebug>

LibrarianListModel::LibrarianListModel( const QString &filename, QObject *parent )
    : QAbstractListModel( parent )
{
    if ( filename != NULL ) {
        this->loadLibrarianFile( filename ); //if a librarian file is provided, load it
    }
}

QString LibrarianListModel::getItemContentAt(int index) {
    if ( index >= 0 && index < counter ) {
        return this->librarian_item_names.at( index );
    } else return QString( );
}

QString LibrarianListModel::getItemNameAt(int index) {
    if ( index >= 0 && index < counter ) {
        return this->librarian_item_contents.at( index );
    } else return QString();
}

int LibrarianListModel::rowCount(const QModelIndex &) const {
    return this->counter;
}

QVariant LibrarianListModel::data(const QModelIndex &index, int role) const {
    if ( role == Qt::DisplayRole ) return this->librarian_item_names.at( index.row() );
    else return QVariant();
}

void LibrarianListModel::insertData(QString name, QString content) {
    this->librarian_item_names << name;
    this->librarian_item_contents << content;
    counter ++;
}

void LibrarianListModel::removeData(int index) {
    if ( index >= 0 && index < counter ) {
        this->librarian_item_names.removeAt( index );
        this->librarian_item_contents.removeAt( index );
        counter --;
    }
}

void LibrarianListModel::loadLibrarianFile(const QString &filename) {
    librarian_item_names.clear();
    librarian_item_contents.clear();
    counter = 0;

    QFile librarianFile( filename );
    if ( librarianFile.open( QIODevice::ReadOnly )) {

        QTextStream *librarian_stream = new QTextStream( &librarianFile );
        bool shouldContinue = true;
        QStringList librarian_data = librarian_stream->readAll().split( "$$$" );

        for ( int i = 0; i < librarian_data.size(); ++i ) {
            if ( shouldContinue ) {
                QStringList current_item_data = librarian_data[i].split( "%%%" );
                if ( current_item_data.size() == 2 ) {
                    //read data from the current line
                    QString librarian_current_item_title = current_item_data[0];
                    QString librarian_current_item_content = current_item_data[1];

                    //load data to array
                    insertData( librarian_current_item_title, librarian_current_item_content );

                } else {
                    shouldContinue = false; //cancel read operation
                }
            } else return;
        }

        librarianFile.close();
    }

}

void LibrarianListModel::saveLibrarianFile(const QString &filename) {
    QFile librarianFile( filename );
    if ( librarianFile.open( QIODevice::WriteOnly )) {
        QTextStream librarian_stream( &librarianFile );
        for ( int i = 0; i < counter; ++i ) {
            librarian_stream << librarian_item_names[ i ] << "%%%" << librarian_item_contents[ i ] << "$$$";
        }
        librarianFile.close();
    }
}

void LibrarianListModel::dataReset() {
    librarian_item_names.clear();
    librarian_item_contents.clear();
    counter = 0;
}
