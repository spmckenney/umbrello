/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <kdebug.h>
#include <klocale.h>

#include "classifierlistitem.h"

UMLClassifierListItem::UMLClassifierListItem(UMLObject *parent, QString Name, int id)
  : UMLObject(parent, Name, id) { 
}

UMLClassifierListItem::UMLClassifierListItem(UMLObject *parent)
  : UMLObject(parent) {
}

UMLClassifierListItem::~UMLClassifierListItem() {
}

QString UMLClassifierListItem::getShortName() {
	return getName();
}

#include "classifierlistitem.moc"
