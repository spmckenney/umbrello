/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *  copyright (C) 2003-2006                                                *
 *  Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                   *
 ***************************************************************************/

#include "umlcheckconstraintdialog.h"
#include "../uml.h"
#include "../umldoc.h"
#include "../checkconstraint.h"

// kde includes
#include <kdebug.h>
#include <klocale.h>

// qt includes
#include <qlayout.h>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>


UMLCheckConstraintDialog::UMLCheckConstraintDialog(QWidget* parent, UMLCheckConstraint* pCheckConstraint) : KDialog(parent) {

    setCaption( i18n("Check Constraint Properties") );
    setButtons( Help | Ok | Cancel );
    setDefaultButton( Ok );
    setModal( true );
    showButtonSeparator( true );

    m_pCheckConstraint = pCheckConstraint;
    m_doc = UMLApp::app()->getDocument();

    setupDialog();
    connect(this,SIGNAL(okClicked()),this,SLOT(slotOk()));

}

UMLCheckConstraintDialog::~UMLCheckConstraintDialog() {

}

void UMLCheckConstraintDialog::setupDialog(){
    QFrame *frame = new QFrame( this );
    setMainWidget( frame );
    int margin = fontMetrics().height();

    //main layout contains the name fields, the text field
    QVBoxLayout* mainLayout = new QVBoxLayout( frame );
    mainLayout->setSpacing(15);

    // layout to hold the name label and line edit
    QHBoxLayout* nameLayout = new QHBoxLayout( mainLayout );

    // name label
    m_pNameL = new QLabel( i18n( "Name" ), this );
    nameLayout->addWidget( m_pNameL );
    // name lineEdit
    m_pNameLE = new QLineEdit( this );
    nameLayout->addWidget( m_pNameLE );

    QVBoxLayout* checkConditionLayout = new QVBoxLayout( mainLayout );

    m_pCheckConditionL = new QLabel( i18n( "Check Condition :" ), frame );
    checkConditionLayout->addWidget( m_pCheckConditionL );

    m_pCheckConditionTE = new QTextEdit( frame );
    checkConditionLayout->addWidget( m_pCheckConditionTE );

    // set text of text edit
    m_pCheckConditionTE->setText(m_pCheckConstraint->getCheckCondition());

    // set text of label
    m_pNameLE->setText( m_pCheckConstraint->getName() );
}



void UMLCheckConstraintDialog::slotOk(){
    if ( apply() ) {
        accept();
    }

}

bool UMLCheckConstraintDialog::apply() {

    m_pCheckConstraint->setCheckCondition( m_pCheckConditionTE->toPlainText().trimmed() );

    // set name
    m_pCheckConstraint->setName( m_pNameLE->text().trimmed() );

    return true;
}



#include "umlcheckconstraintdialog.moc"

