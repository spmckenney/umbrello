/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2010                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

// own header
#include "linkwidget.h"

// app includes
#include "umlview.h"
#include "umlobject.h"
#include "classifier.h"
#include "operation.h"
#include "uml.h"
#include "umlscene.h"

// qt/kde includes
#include <kdebug.h>

LinkWidget::LinkWidget()
{
}

LinkWidget::~LinkWidget()
{
}

/**
 * Motivated by FloatingTextWidget::slotMenuSelection(mt_Operation)
 */
UMLClassifier *LinkWidget::operationOwner()
{
    UMLOperation *op = operation();
    if (op == NULL)
        return NULL;
    return static_cast<UMLClassifier*>(op->parent());
}

// [PORT] May be make the param scene instead of view.
QString LinkWidget::operationText(UMLScene *scene /* = NULL */)
{
    UMLOperation *op = operation();
    if (op == NULL)
        return customOpText();
    if (scene == NULL)
        scene = UMLApp::app()->currentView()->umlScene();
    Uml::Signature_Type sigType;
    if (scene && scene->getShowOpSig())
        sigType = Uml::st_SigNoVis;
    else
        sigType = Uml::st_NoSigNoVis;
    QString opText = op->toString(sigType);
    return opText;
}

/**
 * Motivated by FloatingTextWidget::slotMenuSelection(mt_Reset_Label_Positions)
 * Only applies to AssociationWidget.
 */
void LinkWidget::resetTextPositions()
{
}

/**
 * Motivated by FloatingTextWidget::mouseDoubleClickEvent()
 * Only applies to AssociationWidget.
 */
void LinkWidget::showPropertiesDialog()
{
}

/**
 * Motivated by FloatingTextWidget::setLink().
 * Only applies to AssociationWidget.
 */
void LinkWidget::calculateNameTextSegment()
{
}
