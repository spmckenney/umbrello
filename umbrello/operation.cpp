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

#include "operation.h"
#include "attribute.h"
#include "classifier.h"
#include "uml.h"
#include "umldoc.h"
#include "dialogs/umloperationdialog.h"

UMLOperation::UMLOperation(const UMLClassifier *parent, QString Name, int id,
			   Scope s, QString rt)
    : UMLClassifierListItem(parent, Name, id)
{
	m_TypeName = rt;   // FIXME: Change to use true model object.
	m_Scope = s;
	m_BaseType = ot_Operation;
	m_List.setAutoDelete(false);
}

UMLOperation::UMLOperation(const UMLClassifier * parent)
    : UMLClassifierListItem (parent)
{
	m_BaseType = ot_Operation;
	m_List.setAutoDelete(true);
}

UMLOperation::~UMLOperation() {
}

UMLAttribute * UMLOperation::addParm(QString type, QString name, QString initialValue,
				     QString doc, Uml::Parameter_Kind kind) {
	// make the new parameter (attribute) public, just to be safe
	UMLDoc *umldoc = UMLApp::app()->getDocument();
	UMLAttribute * a = new UMLAttribute(this, name, umldoc->getUniqueID(), Uml::Public, type);
	a -> setDoc(doc);
	a -> setInitialValue(initialValue);
	a -> setParmKind(kind);
	addParm(a);
	return a;
}

void UMLOperation::removeParm(UMLAttribute * a) {
	if (a == NULL) {
		kdDebug() << "UMLOperation::removeParm called on NULL attribute"
			  << endl;
		return;
	}
	kdDebug() << "UMLOperation::removeParm(" << a->getName() << ") called"
		  << endl;
	disconnect(a,SIGNAL(modified()),this,SIGNAL(modified()));
	if(!m_List.remove(a))
		kdDebug() << "Error removing parm " << a->getName() << endl;

	emit modified();
}

UMLAttribute* UMLOperation::findParm(QString name) {
	UMLAttribute * obj=0;
	for (obj = m_List.first(); obj; obj = m_List.next()) {
		if (obj->getName() == name)
			return obj;
	}
	return 0;
}

QString UMLOperation::toString(Signature_Type sig) {
	QString s = "";

	if(sig == st_ShowSig || sig == st_NoSig) {
		if(m_Scope == Public)
			s = "+ ";
		else if(m_Scope == Private)
			s = "- ";
		else if(m_Scope == Protected)
			s = "# ";
	}

	s += getName();
	s.append("(");

	if(sig == st_NoSig || sig == st_NoSigNoScope) {
		s.append(")");
		return s;
	}
	UMLAttribute * obj=0;
	int last = m_List.count(), i = 0;
	for(obj=m_List.first();obj != 0;obj=m_List.next()) {
		i++;
		s.append(obj -> toString(st_SigNoScope));
		if(i < last)
			s.append(", ");
	}
	s.append(")");
	QString returnType = UMLClassifierListItem::getTypeName();
	if (returnType.length() > 0 ) {
		s.append(" : ");

		if (returnType.startsWith("virtual ")) {
			s += returnType.mid(8);
		} else {
			s += returnType;
		}
	}
	return s;
}

void UMLOperation::addParm(UMLAttribute *parameter, int position) {
	if( position >= 0 && position <= (int)m_List.count() )
		m_List.insert(position,parameter);
	else
		m_List.append( parameter );
	emit modified();
	connect(parameter,SIGNAL(modified()),this,SIGNAL(modified()));
}

QString UMLOperation::getUniqueParameterName() {
	QString currentName = i18n("new_parameter");
	QString name = currentName;
	for (int number = 1; findParm(name); number++) {
	        name = currentName + "_" + QString::number(number);
	}
	return name;
}

bool UMLOperation::operator==( UMLOperation & rhs ) {
	if( this == &rhs )
		return true;

	if( !UMLObject::operator==( rhs ) )
		return false;

	if( getTypeName() != rhs.getTypeName() )
		return false;

	if( m_List.count() != rhs.m_List.count() )
		return false;

	if(!(m_List == rhs.m_List))
		return false;

	return true;
}

void UMLOperation::copyInto(UMLOperation *rhs) const
{
	UMLClassifierListItem::copyInto(rhs);

	m_List.copyInto(&(rhs->m_List));
}

UMLObject* UMLOperation::clone() const
{
	// TODO Why is this a UMLClassifier?
	// -- Huh? I don't understand this TODO   --okellogg
	UMLOperation *clone = new UMLOperation( (UMLClassifier *) parent());
	copyInto(clone);

	return clone;
}

bool UMLOperation::resolveParmTypes() {
	UMLAttribute *pAtt;
	bool overallSuccess = true;
	for (pAtt = m_List.first(); pAtt; pAtt = m_List.next()) {
		if (! pAtt->resolveType())
			overallSuccess = false;
	}
	return overallSuccess;
}

void UMLOperation::saveToXMI( QDomDocument & qDoc, QDomElement & qElement ) {
	QDomElement operationElement = UMLObject::save("UML:Operation", qDoc);
	operationElement.setAttribute( "type", getTypeName() );  // FIXME use model obj.
	//save each attribute here, type different
	UMLAttribute* pAtt = 0;
	for( pAtt = m_List.first(); pAtt != 0; pAtt = m_List.next() ) {
		QDomElement attElement = pAtt->UMLObject::save("UML:Parameter", qDoc);
		UMLClassifier *attrType = pAtt->getType();
		if (attrType) {
			attElement.setAttribute( "type", attrType->getID() );
		} else {
			attElement.setAttribute( "type", pAtt -> getTypeName() );
		}
		attElement.setAttribute( "value", pAtt -> getInitialValue() );

		Uml::Parameter_Kind kind = pAtt->getParmKind();
		if (kind == Uml::pk_Out)
			attElement.setAttribute("kind", "out");
		else if (kind == Uml::pk_InOut)
			attElement.setAttribute("kind", "inout");
		// The default for the parameter kind is "in".

		operationElement.appendChild( attElement );
	}
	qElement.appendChild( operationElement );
}

bool UMLOperation::load( QDomElement & element ) {
	QString typeName = element.attribute( "type", "" );
	UMLClassifierListItem::setTypeName( typeName );  // FIXME use model obj.
	QDomNode node = element.firstChild();
	if (node.isComment())
		node = node.nextSibling();
	QDomElement attElement = node.toElement();
	while( !attElement.isNull() ) {
		QString tag = attElement.tagName();
		if (tagEq(tag, "BehavioralFeature.parameter")) {
			if (! load(attElement))
				return false;
		} else if (tagEq(tag, "Parameter")) {
			UMLAttribute * pAtt = new UMLAttribute( this );
			if( !pAtt->loadFromXMI(attElement) ) {
				delete pAtt;
				return false;
			}
			QString kind = attElement.attribute("kind", "in");
			if (kind == "out")
				pAtt->setParmKind(Uml::pk_Out);
			else if (kind == "inout")
				pAtt->setParmKind(Uml::pk_InOut);
			else
				pAtt->setParmKind(Uml::pk_In);
			m_List.append( pAtt );
		}
		node = node.nextSibling();
		if (node.isComment())
			node = node.nextSibling();
		attElement = node.toElement();
	}//end while
	return true;
}

bool UMLOperation::isConstructorOperation() {
	UMLClassifier * c = dynamic_cast<UMLClassifier*>(this->parent());

	QString cName = c->getName();
	QString opName = getName();
	QString opReturn = getReturnType();

	// its a constructor operation if the operation name and return type
	// match that of the parent classifier
	return (cName == opName && cName == opReturn) ? true : false;
}

bool UMLOperation::showPropertiesDialogue(QWidget* parent) {
	UMLOperationDialog dialogue(parent, this);
	return dialogue.exec();
}


#include "operation.moc"
