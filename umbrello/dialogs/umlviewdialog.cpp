/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2012                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "umlviewdialog.h"

// local includes
#include "classoptionspage.h"
#include "debug_utils.h"
#include "icon_utils.h"
#include "uml.h"
#include "umldoc.h"
#include "umlview.h"
#include "umlwidgetstylepage.h"

// kde includes
#include <klocale.h>
#include <kmessagebox.h>
#include <kfontdialog.h>
#include <kvbox.h>

// qt includes
#include <QtGui/QFrame>
#include <QtGui/QHBoxLayout>

/**
 * Constructor.
 */
UMLViewDialog::UMLViewDialog(QWidget * pParent, UMLView * pView)
  : KPageDialog(pParent)
{
    setCaption( i18n("Properties") );
    setButtons( Ok | Apply | Cancel | Help );
    setDefaultButton( Ok );
    setModal( true );
    setFaceType( KPageDialog::List );
    showButtonSeparator( true );
    m_pView = pView;
    m_options = m_pView->umlScene()->optionState();
    setupPages();
    connect(this,SIGNAL(okClicked()),this,SLOT(slotOk()));
    connect(this,SIGNAL(applyClicked()),this,SLOT(slotApply()));
}

/**
 * Destructor.
 */
UMLViewDialog::~UMLViewDialog()
{
}

void UMLViewDialog::slotOk()
{
    applyPage( m_pageGeneralItem );
    applyPage( m_pageDisplayItem );
    applyPage( m_pageFontItem );
    applyPage( m_pageStyleItem );
    accept();
}

void UMLViewDialog::slotApply()
{
    applyPage( currentPage() );
}

/**
 * Sets up the dialog pages.
 */
void UMLViewDialog::setupPages()
{
    setupDiagramPropertiesPage();
    setupStylePage();
    setupFontPage();
    setupClassPage();
}

/**
 * Sets up the general Diagram Properties Page
 */
void UMLViewDialog::setupDiagramPropertiesPage()
{
    KVBox *page = new KVBox();
    m_pageGeneralItem = new KPageWidgetItem( page, i18nc("general settings page", "General") );
    m_pageGeneralItem->setHeader( i18n("General Settings") );
    m_pageGeneralItem->setIcon(Icon_Utils::DesktopIcon(Icon_Utils::it_Properties_General) );
    addPage(m_pageGeneralItem);

    m_diagramProperties = new DiagramPropertiesPage(page);

    m_diagramProperties->ui_diagramName->setText( m_pView->umlScene()->name() );
    m_diagramProperties->ui_zoom->setValue(m_pView->currentZoom());
    m_diagramProperties->ui_showOpSigs->setChecked( m_pView->umlScene()->getShowOpSig() );

    m_diagramProperties->ui_checkBoxShowGrid->setChecked(m_pView->umlScene()->getShowSnapGrid());
    m_diagramProperties->ui_snapToGrid->setChecked(m_pView->umlScene()->getSnapToGrid());
    m_diagramProperties->ui_snapComponentSizeToGrid->setChecked(m_pView->umlScene()->getSnapComponentSizeToGrid());

    m_diagramProperties->ui_gridSpaceX->setValue( m_pView->umlScene()->getSnapX());
    m_diagramProperties->ui_gridSpaceY->setValue( m_pView->umlScene()->getSnapY());
    m_diagramProperties->ui_lineWidth->setValue( m_pView->umlScene()->lineWidth());
    m_diagramProperties->ui_documentation->setText(m_pView->umlScene()->documentation());
}

/**
 * Sets up the Class page
 */
void UMLViewDialog::setupClassPage()
{
    if ( m_pView->umlScene()->type() != Uml::DiagramType::Class ) {
        return;
    }
    QFrame * newPage = new QFrame();
    m_pageDisplayItem = new KPageWidgetItem( newPage, i18nc("classes display options page", "Display") );
    m_pageDisplayItem->setHeader( i18n("Classes Display Options") );
    m_pageDisplayItem->setIcon( Icon_Utils::DesktopIcon(Icon_Utils::it_Properties_Display) );
    addPage(m_pageDisplayItem);

    QHBoxLayout * pOptionsLayout = new QHBoxLayout( newPage );
    m_pOptionsPage = new ClassOptionsPage( newPage, &m_options );
    pOptionsLayout->addWidget( m_pOptionsPage );
}

/**
 * Sets up the style page.
 */
void UMLViewDialog::setupStylePage()
{
    QFrame * stylePage = new QFrame();
    m_pageStyleItem = new KPageWidgetItem( stylePage, i18nc("diagram style page", "Style") );
    m_pageStyleItem->setHeader( i18n("Diagram Style") );
    m_pageStyleItem->setIcon( Icon_Utils::DesktopIcon(Icon_Utils::it_Properties_Color) );
    addPage(m_pageStyleItem);

    QHBoxLayout * m_pStyleLayout = new QHBoxLayout(stylePage);
    m_pStylePage = new UMLWidgetStylePage( stylePage, &m_options );
    m_pStyleLayout->addWidget(m_pStylePage);
}

/**
 * Sets up font page.
 */
void UMLViewDialog::setupFontPage()
{
    KVBox *page = new KVBox();
    m_pageFontItem = new KPageWidgetItem( page, i18n("Font") );
    m_pageFontItem->setHeader( i18n("Font Settings") );
    m_pageFontItem->setIcon( Icon_Utils::DesktopIcon(Icon_Utils::it_Properties_Font) );
    addPage(m_pageFontItem);

    m_pChooser = new KFontChooser( (QWidget*)page, KFontChooser::NoDisplayFlags, QStringList(), 0);
    m_pChooser->setFont( m_pView->umlScene()->optionState().uiState.font );
}

/**
 * Applies the properties of the given page.
 */
void UMLViewDialog::applyPage(KPageWidgetItem *item)
{
    if ( item == m_pageGeneralItem )
    {
        checkName();
        m_pView->setZoom( m_diagramProperties->ui_zoom->value() );
        m_pView->umlScene()->setDocumentation( m_diagramProperties->ui_documentation->toPlainText() );
        m_pView->umlScene()->setSnapX( m_diagramProperties->ui_gridSpaceX->value() );
        m_pView->umlScene()->setSnapY( m_diagramProperties->ui_gridSpaceY->value() );
        m_pView->umlScene()->setLineWidth( m_diagramProperties->ui_lineWidth->value() );
        m_pView->umlScene()->setSnapToGrid( m_diagramProperties->ui_snapToGrid->isChecked() );
        m_pView->umlScene()->setSnapComponentSizeToGrid( m_diagramProperties->ui_snapComponentSizeToGrid->isChecked() );
        m_pView->umlScene()->setShowSnapGrid( m_diagramProperties->ui_checkBoxShowGrid->isChecked() );
        m_pView->umlScene()->setShowOpSig( m_diagramProperties->ui_showOpSigs->isChecked() );
    }
    else if ( item == m_pageStyleItem )
    {
        uDebug() << "setting colors ";
        m_pStylePage->updateUMLWidget();
        /* FIXME: updateUMLWidget uses UMLScene::setOptions() to set back m_options
         * into UMLScene (as also done for m_pageDisplayItem below) so theoretical
         * the following code would be obsolate.
         * The problems is that setOptions() do not signal UMLScene that options has
         * been changed. There is a signal UMLscene::sigColorChanged(), but this signal
         * is limited to color only changes. There is no common way to say UMLScene, that
         * one or all options has been changed.
         */
        m_pView->umlScene()->setLineWidth( m_options.uiState.lineWidth );
        m_pView->umlScene()->setUseFillColor( m_options.uiState.useFillColor );
        m_pView->umlScene()->setTextColor( m_options.uiState.textColor );
        m_pView->umlScene()->setLineColor( m_options.uiState.lineColor );
        m_pView->umlScene()->setFillColor( m_options.uiState.fillColor );
        m_pView->umlScene()->setBackgroundColor( m_options.uiState.backgroundColor );
        m_pView->umlScene()->setGridDotColor( m_options.uiState.gridDotColor );
    }
    else if ( item == m_pageFontItem )
    {
        uDebug() << "setting font " << m_pChooser->font().toString();
        m_pView->umlScene()->setFont( m_pChooser->font(), true );
    }
    else if ( item == m_pageDisplayItem )
    {
        if ( m_pView->umlScene()->type() != Uml::DiagramType::Class ) {
            return;
        }
        m_pOptionsPage->updateUMLWidget();
        m_pView->umlScene()->setClassWidgetOptions( m_pOptionsPage );
        //       sig = m_pTempWidget->getShowOpSigs();
        //       showSig = !( sig == Uml::st_NoSig || sig == Uml::st_NoSigNoVis );
        //       options.classState.showOpSig = showSig;
        //       sig = m_pTempWidget->getShowAttSigs();
        //       showSig = !( sig == Uml::st_NoSig || sig == Uml::st_NoSigNoVis );
        //       options.classState.showAttSig = showSig;
        m_pView->umlScene()->setOptionState( m_options );
    }
}

/**
 * Checks whether the name is unique and sets it if it is.
 */
void UMLViewDialog::checkName()
{
    QString newName = m_diagramProperties->ui_diagramName->text();
    if ( newName.length() == 0 ) {
        KMessageBox::sorry(this, i18n("The name you have entered is invalid."),
                           i18n("Invalid Name"), 0);
        m_diagramProperties->ui_diagramName->setText( m_pView->umlScene()->name() );
        return;
    }

    if (newName != m_pView->umlScene()->name()) {
        UMLDoc* doc = UMLApp::app()->document();
        UMLView* view = doc->findView(m_pView->umlScene()->type(), newName);
        if (view) {
            KMessageBox::sorry(this, i18n("The name you have entered is not unique."),
                               i18n("Name Not Unique"), 0);
            m_diagramProperties->ui_diagramName->setText( m_pView->umlScene()->name() );
        }
        else {
            // uDebug() << "Cannot find view with name " << newName;
            m_pView->umlScene()->setName( newName );
            doc->signalDiagramRenamed(m_pView);
        }
    }
}

#include "umlviewdialog.moc"
