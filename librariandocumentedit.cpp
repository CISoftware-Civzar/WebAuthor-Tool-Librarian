#include "librariandocumentedit.h"

LibrarianDocumentEdit::LibrarianDocumentEdit( QWidget *parent )
    : QTextEdit( parent )
{
    //set up editing toolbar
    editingToolbar = new QToolBar( );
    editingToolbar->setAllowedAreas( Qt::BottomToolBarArea );
    editingToolbar->setMovable( false );
    editingToolbar->setFloatable( false );

    //set up actions
    QAction *normalText = new QAction( NULL );
    QAction *boldText = new QAction( NULL );
    QAction *italicText = new QAction( NULL );
    QAction *underlineText = new QAction( NULL );
    QAction *makeHead = new QAction( NULL );
    QAction *makeParagraph = new QAction( NULL );

    //set up actions
    normalText->setText( tr( "Normal" ) );
    boldText->setText( tr( "Bold" ) );
    italicText->setText( tr( "Italicize" ) );
    underlineText->setText( tr( "Underline" ) );
    makeHead->setText( tr( "Heading" ) );
    makeParagraph->setText( tr( "Paragraph" ) );

    //set up action shortcuts
    normalText->setShortcut( QKeySequence( "ctrl+1" ) );
    boldText->setShortcut( QKeySequence( "ctrl+2" ) );
    italicText->setShortcut( QKeySequence( "ctrl+3" ) );
    underlineText->setShortcut( QKeySequence( "ctrl+4" ) );
    makeHead->setShortcut( QKeySequence( "ctrl+5" ) );
    makeParagraph->setShortcut( QKeySequence( "ctrl+6" ) );

    //connect to actions
    connect( normalText, SIGNAL(triggered(bool)), SLOT(makeNormal()) );
    connect( boldText, SIGNAL(triggered(bool)), SLOT(makeBold()) );
    connect( italicText, SIGNAL(triggered(bool)), SLOT(makeItalic()) );
    connect( underlineText, SIGNAL(triggered(bool)), SLOT(makeUnderline()) );
    connect( makeHead, SIGNAL(triggered(bool)), SLOT(makeHead()) );
    connect( makeParagraph, SIGNAL(triggered(bool)), SLOT(makePara()) );

    //populate toolbar
    editingToolbar->addAction( normalText );
    editingToolbar->addAction( boldText );
    editingToolbar->addAction( italicText );
    editingToolbar->addAction( underlineText );
    editingToolbar->addSeparator();

    editingToolbar->addAction( makeHead );
    editingToolbar->addAction( makeParagraph );

    //configure this widget
    this->setLineWrapMode( QTextEdit::WidgetWidth );

}

void LibrarianDocumentEdit::modifyTextTag(QString newTag) {
    QTextCursor cursor = textCursor();

    if ( !cursor.selectedText().isEmpty() ) {
        //select the word under the cursor
        QString oldString = cursor.selectedText();

        //insert the old text into the new tag
        QString updated = newTag.arg( oldString );
        cursor.insertHtml( updated );
    }

}

void LibrarianDocumentEdit::makeNormal() {
    modifyTextTag( "%1" );
}

void LibrarianDocumentEdit::makeBold() {
    modifyTextTag( "<b>%1</b>" );
}

void LibrarianDocumentEdit::makeItalic() {
    modifyTextTag( "<i>%1</i>" );
}

void LibrarianDocumentEdit::makeUnderline() {
    modifyTextTag( "<u>%1</u>" );
}

void LibrarianDocumentEdit::makeHead() {
    modifyTextTag( "<h3>%1</h3>" );
}

void LibrarianDocumentEdit::makePara() {
    modifyTextTag( "<p>%1</p>" );
}

QToolBar *LibrarianDocumentEdit::getEditingToolbar() {
    return this->editingToolbar;
}
