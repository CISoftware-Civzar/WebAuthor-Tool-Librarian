#include "webauthorlibrarian.h"

WebAuthorLibrarian::WebAuthorLibrarian(QWidget *parent)
    : QMainWindow(parent)
{

    //initialize widgets
    QToolBar *toolbar = new QToolBar();
    QWidget *view = new QWidget();
    QHBoxLayout *layout = new QHBoxLayout();
    QVBoxLayout *vlayout = new QVBoxLayout();

    nameEditor = new QWidget();
    statusbar = new QStatusBar();
    model = new LibrarianListModel( );
    lineEditName = new QLineEdit();

    nameEditor->setWindowTitle( tr( "Item Name Editor" ) );
    nameEditor->setMinimumSize( 400, 100 );
    nameEditor->setMaximumSize( 700, 100 );
    nameEditor->resize( 400, 100 );

    statusbar->showMessage( tr( "Welcome to Librarian!" ) );

    QPushButton *done = new QPushButton( tr( "Set Name" ) );
    done->setShortcut( Qt::Key_Return );

    vlayout->addWidget( lineEditName );
    vlayout->addWidget( done );
    nameEditor->setLayout( vlayout );

    QAction *inewDocument = new QAction( tr( "New" ) );
    inewDocument->setShortcut( QKeySequence::New );
    inewDocument->setStatusTip( tr( "Creates a new Librarian document." ) );
    QAction *iopenDocument = new QAction( tr( "Open" ) );
    iopenDocument->setShortcut( QKeySequence::Open );
    iopenDocument->setStatusTip( tr( "Opens an existing Librarian document." ) );
    QAction *isaveDocument = new QAction( tr ( "Save" ) );
    isaveDocument->setShortcut( QKeySequence::Save );
    isaveDocument->setStatusTip( tr( "Save this as a Librarian document." ) );
    QAction *setTitle = new QAction( tr( "Set Name" ) );
    setTitle->setStatusTip( tr( "Change the selected item's name" ) );

    addItem = new QAction( tr( "Add Item" ) );
    addItem->setStatusTip( tr( "Add a new entry." ) );
    addItem->setShortcut( QKeySequence( "Ctrl+shift+n" ) );
    removeItem = new QAction( tr( "Remove Item" ) );
    removeItem->setStatusTip( tr( "Removes the selected entry." ) );
    removeItem->setShortcut( Qt::Key_Delete );

    //set up toolbar
    toolbar->addAction( inewDocument );
    toolbar->addAction( iopenDocument );
    toolbar->addAction( isaveDocument );
    toolbar->addSeparator();
    toolbar->addAction( addItem );
    toolbar->addAction( removeItem );
    toolbar->addAction( setTitle );

    librarianList = new QListView( );
    librarianEntry = new QTextEdit( );
    librarianPreview = new QTextBrowser( );

    librarianEntry->setLineWrapMode( QTextEdit::NoWrap );

    //set up widgets
    librarianList->setModel( model );

    //set up layout
    layout->addWidget( librarianList, 8 );
    layout->addWidget( librarianEntry, 30 );
    layout->addWidget( librarianPreview, 15 );

    view->setLayout( layout );

    //set initial values
    selected = 0;
    this->AddNewItem();

    //connect
    connect( librarianEntry, SIGNAL(textChanged()), SLOT(SaveItem()) );
    connect( librarianList, SIGNAL(clicked(QModelIndex)), SLOT(handleIndexChange(QModelIndex)) );
    connect( addItem, SIGNAL(triggered(bool)), SLOT(AddNewItem()) );
    connect( removeItem, SIGNAL(triggered(bool)), SLOT(RemoveItem()) );

    connect( setTitle, SIGNAL(triggered(bool)), SLOT(showNameEditor()) );
    connect( done, SIGNAL(clicked(bool)), SLOT(setName()));

    connect( inewDocument, SIGNAL(triggered(bool)), SLOT(newDocument()) );
    connect( iopenDocument, SIGNAL(triggered(bool)), SLOT(openLoadDialog()) );
    connect( isaveDocument, SIGNAL(triggered(bool)), SLOT(openSaveDialog()) );

    this->setMinimumSize( 720, 400 );
    this->resize( 720, 400 );

    this->setCentralWidget( view );
    this->addToolBar( toolbar );
    this->setStatusBar( statusbar );
    this->setWindowTitle( tr( "Librarian" ) );

}

void WebAuthorLibrarian::setName() {
    currentItemName = lineEditName->text();
    model->librarian_item_names[selected] = lineEditName->text();
    nameEditor->hide();
}

void WebAuthorLibrarian::AddNewItem() {
    QString newItemName = tr( "New Item" );
    QString newItemContent = tr( "<h2>Enter title here</h2>\n<h3> What does it do? </h3>\n<p>Enter description here</p>"
                                 "\n<h3>Example</h3>\n<p>&lt;\ntagname\n&gt;\n give an example here\n&lt;\n/tagname\n&gt;</p>" );

    model->insertData( newItemName, newItemContent );

    updateList();
    showEntryCount();
}

void WebAuthorLibrarian::RemoveItem() {
    if ( selected != -1 && model->rowCount(QModelIndex()) > 1 ) {
        int confirm = QMessageBox::question( this, tr( "WebAuthor Librarian Tool" ),
                                             tr( "You are about to delete this entry, are you sure? ( This action is irreversible )" ),
                                             QMessageBox::Yes | QMessageBox::No );

        if ( confirm == QMessageBox::Yes ) {
            model->removeData( selected );
            updateList();
            showEntryCount();
        }
    }
}

void WebAuthorLibrarian::showNameEditor() {
    lineEditName->setText( currentItemName );
    nameEditor->show();
}

void WebAuthorLibrarian::SaveItem() {
    if ( !switched ) {
        QString updatedContent = librarianEntry->document()->toPlainText();
        model->librarian_item_contents[selected] = updatedContent;
        updateList();
    } else {
        switched = false;
    }

    //update layout preview and entry count
    librarianPreview->setHtml( librarianEntry->document()->toPlainText() );
    showEntryCount();
}

void WebAuthorLibrarian::showEntryCount() {
    int nEntries = model->rowCount(QModelIndex());
    statusbar->showMessage( ( nEntries > 1 ) ? tr( "%1 Entries" ).arg( nEntries ) : tr( "%1 Entry" ).arg( nEntries ) );
}

void WebAuthorLibrarian::handleIndexChange(const QModelIndex &index) {
    this->selected = index.row();

    //update view
    librarianEntry->setPlainText( model->librarian_item_contents.at( index.row() ) );
    lineEditName->setText( model->data( index, Qt::DisplayRole ).toString() );

    showEntryCount();
    switched = true;
}

void WebAuthorLibrarian::updateList() {
    librarianList->reset( );
}

void WebAuthorLibrarian::newDocument() {
    int confirm = QMessageBox::question( this, tr( "WebAuthor Librarian Tool" ),
                                         tr( "Creating a new document will destroy any unsaved progress!" ),
                                         QMessageBox::Yes | QMessageBox::No );

    if ( confirm == QMessageBox::Yes ) {
        model->dataReset();
        librarianEntry->document()->setPlainText( "" );
        updateList();
        AddNewItem();
        showEntryCount();
    }
}

void WebAuthorLibrarian::openLoadDialog() {
    QFileDialog *openDialog = new QFileDialog();
    openDialog->setNameFilter( "WebAuthor Librarian Files (*.wblibrarian)" );
    openDialog->setAcceptMode( QFileDialog::AcceptOpen );
    openDialog->setFileMode( QFileDialog::ExistingFile );
    connect( openDialog, SIGNAL(fileSelected(QString)), SLOT(handleLoadDialog(QString)) );
    openDialog->show();
}

void WebAuthorLibrarian::openSaveDialog() {
    QFileDialog *saveDialog = new QFileDialog();
    saveDialog->setNameFilter( "WebAuthor Librarian Files (*.wblibrarian)" );
    saveDialog->setAcceptMode( QFileDialog::AcceptSave );
    saveDialog->setDefaultSuffix( "wblibrarian" );
    connect( saveDialog, SIGNAL(fileSelected(QString)), SLOT(handleSaveDialog(QString)) );
    saveDialog->show();
}

void WebAuthorLibrarian::handleLoadDialog(const QString &filename) {
    model->loadLibrarianFile( filename );
    updateList();
    showEntryCount();
}

void WebAuthorLibrarian::handleSaveDialog(const QString &filename) {
    model->saveLibrarianFile( filename );
}
