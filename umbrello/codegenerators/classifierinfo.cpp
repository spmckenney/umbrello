
/***************************************************************************
			  classifierinfo.cpp  -  description
			     -------------------
    copyright	    : (C) 2003 Brian Thomas brian.thomas@gsfc.nasa.gov
***************************************************************************/

/***************************************************************************
 *									 *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.				   *
 *									 *
 ***************************************************************************/


#include "classifierinfo.h"

#include "../class.h"
#include "../interface.h"
#include "../operation.h"

ClassifierInfo::ClassifierInfo( UMLClassifier *classifier , UMLDoc */*doc*/)
{

	init(classifier);
}

ClassifierInfo::~ClassifierInfo() { }

void ClassifierInfo::init(UMLClassifier *c) {

	// make all QPtrLists autoDelete false
	atpub.setAutoDelete(false);
	atprot.setAutoDelete(false);
	atpriv.setAutoDelete(false);

	static_atpub.setAutoDelete(false);
	static_atprot.setAutoDelete(false);
	static_atpriv.setAutoDelete(false);

	// set default class, file names
	className = c->getName();
	fileName = c->getName().lower();

	// determine up-front what we are dealing with
	if(dynamic_cast<UMLInterface*>(c))
		isInterface = true;
	else
		isInterface = false;

	// set id
	m_nID = c->getID();

	// sort attributes by Scope
	if(!isInterface) {
		UMLClass * myClass = dynamic_cast<UMLClass *>(c);
		UMLAttributeList *atl = myClass->getFilteredAttributeList();
		for(UMLAttribute *at=atl->first(); at ; at=atl->next()) {
			switch(at->getScope())
			{
			case Uml::Public:
				if(at->getStatic())
					static_atpub.append(at);
				else
					atpub.append(at);
				break;
			case Uml::Protected:
				if(at->getStatic())
					static_atprot.append(at);
				else
					atprot.append(at);
				break;
			case Uml::Private:
				if(at->getStatic())
					static_atprot.append(at);
				else
					atpriv.append(at);
				break;
			}
			m_AttsList.append(at);
		}
	}

	// inheritance issues
	superclasses = c->getSuperClasses(); // list of what we inherit from
	superclasses.setAutoDelete(false);

	subclasses = c->getSubClasses();     // list of what inherits from us
	subclasses.setAutoDelete(false);

	// another preparation, determine what we have
	plainAssociations = c->getSpecificAssocs(Uml::at_Association); // BAD! only way to get "general" associations.
	plainAssociations.setAutoDelete(false);

	aggregations = c->getAggregations();
	aggregations.setAutoDelete(false);

	compositions = c->getCompositions();
	compositions.setAutoDelete(false);

	// set some summary information about the classifier now
	hasAssociations = plainAssociations.count() > 0 || aggregations.count() > 0 || compositions.count() > 0;
	hasAttributes = atpub.count() > 0 || atprot.count() > 0 || atpriv.count() > 0
			     || static_atpub.count() > 0
			     || static_atprot.count() > 0
			     || static_atpriv.count() > 0;

	hasStaticAttributes = static_atpub.count() > 0
			     || static_atprot.count() > 0
			     || static_atpriv.count() > 0;

	hasAccessorMethods = hasAttributes || hasAssociations;

	hasOperationMethods = c->getFilteredOperationsList().last() ? true : false;

	hasMethods = hasOperationMethods || hasAccessorMethods;

	// this is a bit too simplistic..some associations are for
	// SINGLE objects, and WONT be declared as Vectors, so this
	// is a bit overly inclusive (I guess that's better than the other way around)
	hasVectorFields = hasAssociations ? true : false;


}

UMLClassifierList ClassifierInfo::getPlainAssocChildClassifierList() {
	return findAssocClassifierObjsInRoles(&plainAssociations);
}

UMLClassifierList ClassifierInfo::getAggregateChildClassifierList() {
	return findAssocClassifierObjsInRoles(&aggregations);
}

UMLClassifierList ClassifierInfo::getCompositionChildClassifierList() {
	return findAssocClassifierObjsInRoles(&compositions);
}

UMLClassifierList ClassifierInfo::findAssocClassifierObjsInRoles (UMLAssociationList * list)
{


	UMLClassifierList classifiers;
	classifiers.setAutoDelete(false);

	for (UMLAssociation *a = list->first(); a; a = list->next()) {
		// DONT accept a classifier IF the association role is empty, by
		// convention, that means to ignore the classifier on that end of
		// the association.
		// We also ignore classifiers which are the same as the current one
		// (e.g. id matches), we only want the "other" classifiers
		if (a->getRoleId(A) == m_nID && a->getRoleName(B) != "") {
			UMLClassifier *c = dynamic_cast<UMLClassifier*>(a->getObject(B));
			if(c)
				classifiers.append(c);
		} else if (a->getRoleId(B) == m_nID && a->getRoleName(A) != "") {
			UMLClassifier *c = dynamic_cast<UMLClassifier*>(a->getObject(A));
			if(c)
				classifiers.append(c);
		}
	}

	return classifiers;
}

UMLAttributeList* ClassifierInfo::getAttList() {
	        return &m_AttsList;
}

