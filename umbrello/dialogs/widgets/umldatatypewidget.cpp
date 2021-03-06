/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2016                                              *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

#include "umldatatypewidget.h"

#include "classifierlistitem.h"
#include "classifier.h"
#include "debug_utils.h"
#include "entityattribute.h"
#include "import_utils.h"
#include "object_factory.h"
#include "operation.h"
#include "template.h"
#include "uml.h"
#include "umldoc.h"

#include <KComboBox>
#include <KLocalizedString>

#include <QApplication>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QWidget>

UMLDatatypeWidget::UMLDatatypeWidget(UMLAttribute *attribute, QWidget *parent)
  : QWidget(parent),
    m_attribute(attribute),
    m_datatype(0),
    m_entityAttribute(0),
    m_operation(0),
    m_template(0)
{
    init();
    m_parent = m_attribute->umlParent()->umlParent()->asUMLClassifier();
    insertTypesSortedParameter(m_attribute->getTypeName());
}

UMLDatatypeWidget::UMLDatatypeWidget(UMLClassifierListItem *datatype, QWidget *parent)
  : QWidget(parent),
    m_attribute(0),
    m_datatype(datatype),
    m_entityAttribute(0),
    m_operation(0),
    m_template(0)
{
    init();
    m_parent = m_datatype->umlParent()->asUMLClassifier();
    insertTypesSortedAttribute(m_datatype->getTypeName());
}

UMLDatatypeWidget::UMLDatatypeWidget(UMLEntityAttribute *entityAttribute, QWidget *parent)
   : QWidget(parent),
     m_attribute(0),
     m_datatype(0),
     m_entityAttribute(entityAttribute),
     m_operation(0),
     m_template(0)
{
    init();
    m_parent = 0;
    insertTypesSortedEntityAttribute(m_entityAttribute->getTypeName());
}

UMLDatatypeWidget::UMLDatatypeWidget(UMLOperation *operation, QWidget *parent)
 :  QWidget(parent),
    m_attribute(0),
    m_datatype(0),
    m_entityAttribute(0),
    m_operation(operation),
    m_template(0)
{
    init();
    m_parent = m_operation->umlParent()->asUMLClassifier();
    insertTypesSortedOperation(m_operation->getTypeName());
}

UMLDatatypeWidget::UMLDatatypeWidget(UMLTemplate *_template, QWidget *parent)
 :  QWidget(parent),
    m_attribute(0),
    m_datatype(0),
    m_entityAttribute(0),
    m_operation(0),
    m_template(_template)
{
    init();
    m_parent = 0;
    insertTypesSortedTemplate(m_template->getTypeName());
}

void UMLDatatypeWidget::init()
{
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setContentsMargins(0,0,0,0);
    m_label = new QLabel(i18n("&Type:"), this);
    layout->addWidget(m_label);

    m_comboBox = new KComboBox(true, this);
    layout->addWidget(m_comboBox);
    m_label->setBuddy(m_comboBox);

    m_comboBox->setDuplicatesEnabled(false); // only allow one of each type in box
#if QT_VERSION < 0x050000
    m_comboBox->setCompletionMode(KGlobalSettings::CompletionPopup);
#endif
    setLayout(layout);
}

UMLDatatypeWidget::~UMLDatatypeWidget()
{
    delete m_comboBox;
    delete m_label;
}

bool UMLDatatypeWidget::apply()
{
    if (m_datatype)
        return applyAttribute();
    else if (m_entityAttribute)
        return applyEntityAttribute();
    else if (m_operation)
        return applyOperation();
    else if (m_attribute)
        return applyParameter();
    else if (m_template)
        return applyTemplate();
    return false;
}

bool UMLDatatypeWidget::applyAttribute()
{
    QString typeName = m_comboBox->currentText();
    UMLTemplate *tmplParam = m_parent->findTemplate(typeName);
    if (tmplParam) {
        m_datatype->setType(tmplParam);
        return true;
    }
    UMLDoc * pDoc = UMLApp::app()->document();

    UMLObject *obj = 0;
    if (!typeName.isEmpty()) {
        obj = pDoc->findUMLObject(typeName);
    }

    UMLClassifier *classifier = obj->asUMLClassifier();
    if (classifier == NULL) {
        Uml::ProgrammingLanguage::Enum pl = UMLApp::app()->activeLanguage();
        // Import_Utils does not handle creating a new object with empty name
        // string well. Use Object_Factory in those cases.
        if (
            (!typeName.isEmpty()) &&
            ((pl == Uml::ProgrammingLanguage::Cpp) ||
                (pl == Uml::ProgrammingLanguage::Java))
        ) {
            // Import_Utils::createUMLObject works better for C++ namespace
            // and java package than Object_Factory::createUMLObject
            Import_Utils::setRelatedClassifier(m_parent);
            obj = Import_Utils::createUMLObject(UMLObject::ot_UMLObject, typeName);
            Import_Utils::setRelatedClassifier(NULL);
        } else {
            // If it's obviously a pointer type (C++) then create a datatype.
            // Else we don't know what it is so as a compromise create a class.
            UMLObject::ObjectType ot =
                (typeName.contains(QChar::fromLatin1('*')) ? UMLObject::ot_Datatype
                                                          : UMLObject::ot_Class);
            obj = Object_Factory::createUMLObject(ot, typeName);
        }
        if (obj == NULL)
            return false;
        classifier = obj->asUMLClassifier();
    }
    m_datatype->setType(classifier);
    return true;
}

bool UMLDatatypeWidget::applyEntityAttribute()
{
    QString typeName = m_comboBox->currentText();
    UMLDoc *pDoc = UMLApp::app()->document();
    UMLClassifierList dataTypes = pDoc->datatypes();
    foreach (UMLClassifier* dat, dataTypes) {
        if (typeName == dat->name()) {
            m_entityAttribute->setType(dat);
            return true;
        }
    }
    UMLObject *obj = pDoc->findUMLObject(typeName);
    UMLClassifier *classifier = obj->asUMLClassifier();
    if (classifier == NULL) {
        // If it's obviously a pointer type (C++) then create a datatype.
        // Else we don't know what it is so as a compromise create a class.
        UMLObject::ObjectType ot =
            (typeName.contains(QChar::fromLatin1('*')) ? UMLObject::ot_Datatype
                                                      : UMLObject::ot_Class);
        obj = Object_Factory::createUMLObject(ot, typeName);
        if (obj == NULL)
            return false;
        classifier = obj->asUMLClassifier();
    }
    m_entityAttribute->setType(classifier);
    return true;
}

bool UMLDatatypeWidget::applyOperation()
{
    QString typeName = m_comboBox->currentText();
    UMLTemplate *tmplParam = 0;
    if (m_parent) {
        tmplParam = m_parent->findTemplate(typeName);
    }
    if (tmplParam)
        m_operation->setType(tmplParam);
    else
        m_operation->setTypeName(typeName);
    return true;
}

bool UMLDatatypeWidget::applyParameter()
{
    // set the type name
    QString typeName = m_comboBox->currentText();
    if (m_parent == NULL) {
        uError() << "grandparent of " << m_attribute->name() << " is not a UMLClassifier";
    } else {
        UMLTemplate *tmplParam = m_parent->findTemplate(typeName);
        if (tmplParam) {
            m_attribute->setType(tmplParam);
            return true;
        }
    }
    UMLDoc * uDoc = UMLApp::app()->document();
    UMLClassifierList namesList(uDoc->concepts());
    bool matchFound = false;

    foreach (UMLClassifier* obj, namesList) {
        if (obj->fullyQualifiedName() == typeName) {
            m_attribute->setType(obj);
            matchFound = true;
            break;
        }
    }
    if (!matchFound) {
        // Nothing found: Create a new type on the fly.
        // @todo There should be an extra dialog to decide whether to
        // create a datatype or a class. For now, we create a class.
        uDebug() << typeName << " not found."
            << " Creating a new class for the type.";
        UMLObject *newObj = Object_Factory::createUMLObject(UMLObject::ot_Class, typeName);
        m_attribute->setType(newObj);
    }
    return true;
}

bool UMLDatatypeWidget::applyTemplate()
{
    QString typeName = m_comboBox->currentText();
    UMLDoc *pDoc = UMLApp::app()->document();
    UMLClassifierList namesList(pDoc->concepts());
    foreach (UMLClassifier* obj, namesList) {
        if (typeName == obj->name()) {
            m_template->setType(obj);
        }
    }
    if (namesList.isEmpty()) { // not found.
        // FIXME: This implementation is not good yet.
        m_template->setTypeName(typeName);
    }
    return true;
}

/**
 * Initialize types combo box from a list of types and a selected type.
 * @param types list of types to add to combo box
 * @param type selected type
 */
void UMLDatatypeWidget::initTypesBox(QStringList &types, const QString& type)
{
    if (!types.contains(type)) {
        types << type;
    }
    types.sort();

    m_comboBox->clear();
    m_comboBox->insertItems(-1, types);

    int currentIndex = m_comboBox->findText(type);
    if (currentIndex > -1) {
        m_comboBox->setCurrentIndex(currentIndex);
    }
    m_comboBox->completionObject()->addItem(type);
}

/**
 * Add classes and interfaces from document instance to the given string list.
 * @param types list to store the classes and interfaces
 */
void UMLDatatypeWidget::insertTypesFromConcepts(QStringList& types, bool fullName)
{
    UMLDoc * uDoc = UMLApp::app()->document();
    UMLClassifierList namesList(uDoc->concepts());
    foreach (UMLClassifier* obj, namesList) {
         types << (fullName ? obj->fullyQualifiedName() : obj->name());
    }
}

/**
 * Add datatypes from document instance to the given string list.
 * @param types list to store the datatypes
 */
void UMLDatatypeWidget::insertTypesFromDatatypes(QStringList& types)
{
    // add the data types
    UMLDoc * pDoc = UMLApp::app()->document();
    UMLClassifierList dataTypes = pDoc->datatypes();
    if (dataTypes.count() == 0) {
        // Switch to SQL as the active language if no datatypes are set.
        UMLApp::app()->setActiveLanguage(Uml::ProgrammingLanguage::SQL);
        pDoc->addDefaultDatatypes();
        qApp->processEvents();
        dataTypes = pDoc->datatypes();
    }
    foreach (UMLClassifier* dat, dataTypes) {
        types << dat->name();
    }
}

/**
 * Inserts @p type into the type-combobox as well as its completion object.
 * The combobox is cleared and all types together with the optional new one
 * sorted and then added again.
 * @param type   a new type to add
 */
void UMLDatatypeWidget::insertTypesSortedAttribute(const QString& type)
{
    QStringList types;
    insertTypesFromConcepts(types);
    initTypesBox(types, type);
}

/**
 * Inserts @p type into the type-combobox as well as its completion object.
 */
void UMLDatatypeWidget::insertTypesSortedEntityAttribute(const QString& type)
{
    QStringList types;
    insertTypesFromDatatypes(types);
    initTypesBox(types, type);
}

/**
 * Inserts @p type into the type-combobox.
 * The combobox is cleared and all types together with the optional new one
 * sorted and then added again.
 * @param type   a new type to add and selected
 */
void UMLDatatypeWidget::insertTypesSortedOperation(const QString& type)
{
    QStringList types;
    // Add "void". We use this for denoting "no return type" independent
    // of the programming language.
    // For example, the Ada generator would interpret the return type
    // "void" as an instruction to generate a procedure instead of a
    // function.
    types << QLatin1String("void");
    // add template parameters
    UMLClassifier *classifier = m_parent->asUMLClassifier();
    if (classifier) {
        UMLClassifierListItemList tmplParams(classifier->getFilteredList(UMLOperation::ot_Template));
        foreach (UMLClassifierListItem* li, tmplParams) {
            types << li->name();
        }
    }
    insertTypesFromConcepts(types);
    initTypesBox(types, type);
}

/**
 * Inserts @p type into the type-combobox as well as its completion object.
 * The combobox is cleared and all types together with the optional new one
 * sorted and then added again.
 * @param type   a new type to add and selected
 */
void UMLDatatypeWidget::insertTypesSortedParameter(const QString& type)
{
    QStringList types;
    // add template parameters
    UMLClassifier *pConcept = m_parent->asUMLClassifier();
    if (pConcept == NULL) {
        uError() << "ParameterPropertiesDialog: grandparent of " << m_attribute->name()
                 << " is not a UMLClassifier";
    } else {
        UMLTemplateList tmplParams(pConcept->getTemplateList());
        foreach(UMLTemplate* t, tmplParams) {
            types << t->name();
        }
    }
    insertTypesFromConcepts(types);
    initTypesBox(types, type);
}

/**
 * Inserts @p type into the type-combobox.
 * The combobox is cleared and all types together with the optional new one
 * sorted and then added again.
 * @param type   a new type to add and selected
 */
void UMLDatatypeWidget::insertTypesSortedTemplate(const QString& type)
{
    QStringList types;
    // "class" is the nominal type of template parameter
    types << QLatin1String("class");
    insertTypesFromConcepts(types, false);
    initTypesBox(types, type);
}

/**
 * Add this widget to a given grid layout. Umbrello dialogs places labels in column 0
 * and the editable field in column 1.
 * @param layout The layout to which the widget should be added
 * @param row The row in the grid layout where the widget should be placed
 */
void UMLDatatypeWidget::addToLayout(QGridLayout *layout, int row, int startColumn)
{
    layout->addWidget(m_label, row, startColumn);
    layout->addWidget(m_comboBox, row, startColumn + 1);
}
