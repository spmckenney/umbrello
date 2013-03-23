/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2002-2013                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#ifndef ASSOCIATIONWIDGET_H
#define ASSOCIATIONWIDGET_H

#include "associationline.h"
#include "associationwidgetlist.h"
#include "linkwidget.h"
#include "messagewidgetlist.h"
#include "umlwidgetlist.h"
#include "widgetbase.h"

class AssociationLine;
class AssociationWidget;
class ClassifierWidget;
class FloatingTextWidget;
class UMLAssociation;
class UMLAttribute;
class UMLClassifierListItem;
class UMLWidget;

struct WidgetRole
{
    FloatingTextWidget *multiplicityWidget;
    FloatingTextWidget *changeabilityWidget;
    FloatingTextWidget *roleWidget;

    UMLWidget *umlWidget;

    // The following are used only in case of absence of UMLObject
    Uml::Visibility::Enum      visibility;
    Uml::Changeability::Enum   changeability;
    QString                    roleDocumentation;

    WidgetRole();
    ~WidgetRole();

    void initFloatingWidgets(Uml::RoleType::Enum role, AssociationWidget *parent);
};

/**
 * This class represents an association inside a diagram.
 *
 * Associations exist not only between UML objects. For example, when a Note is
 * attached to a UML object, the Note itself is not a UML object.
 * This class supports both kinds of associations. An association where one or
 * both roles are not a UML object is called a "pure widget association".
 *
 * An AssociationWidget where both roles are UML objects has a corresponding
 * UMLAssociation. The UMLAssociation can be retrieved using the getAssociation
 * method.
 * A pure widget association does not have a corresponding UMLAssociation.
 * The getAssociation method returns NULL in this case.
 *
 *
 * @author Gustavo Madrigal
 * @author Gopala Krishna
 * @short This class represents an association inside a diagram.
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class AssociationWidget : public WidgetBase, public LinkWidget
{
    Q_OBJECT
public:
    AssociationWidget();
    AssociationWidget(UMLWidget *widgetA, Uml::AssociationType::Enum type,
                      UMLWidget *widgetB, UMLObject *obj = 0);
    virtual ~AssociationWidget();

    virtual void setUMLObject(UMLObject *obj, bool notifyAsSlot = false);

    //---------- LinkWidget Interface methods implemementation from now on.

    virtual void lwSetFont(QFont font);
    virtual UMLClassifier *operationOwner();

    virtual UMLOperation *operation();
    virtual void setOperation(UMLOperation *op);

    virtual QString customOpText();
    virtual void setCustomOpText(const QString &opText);

    virtual void resetTextPositions();

    virtual void setMessageText(FloatingTextWidget *ft);
    virtual void setText(FloatingTextWidget *ft, const QString &newText);

    virtual void showPropertiesDialog();

    virtual UMLClassifier* seqNumAndOp(QString& seqNum, QString& op);
    virtual void setSeqNumAndOp(const QString &seqNum, const QString &op);

    virtual void constrainTextPos(UMLSceneValue &textX, UMLSceneValue &textY,
                                  UMLSceneValue textWidth, UMLSceneValue textHeight,
                                  Uml::TextRole::Enum tr);

    virtual void calculateNameTextSegment();

    //---------- End LinkWidget Interface methods implemementation.

    UMLAssociation* association() const;
    UMLAttribute* attribute() const;

    bool isEqual(AssociationWidget *other) const;

    FloatingTextWidget* textWidgetByRole(Uml::TextRole::Enum tr) const;

    FloatingTextWidget* nameWidget() const;

    FloatingTextWidget* roleWidget(Uml::RoleType::Enum role) const;
    QString roleName(Uml::RoleType::Enum role) const;
    void setRoleName(const QString &strRole, Uml::RoleType::Enum role);

    QString roleDocumentation(Uml::RoleType::Enum role) const;
    void setRoleDocumentation(const QString& doc, Uml::RoleType::Enum role);

    FloatingTextWidget* multiplicityWidget(Uml::RoleType::Enum role) const;
    QString multiplicity(Uml::RoleType::Enum role) const;
    void setMultiplicity(const QString& text, Uml::RoleType::Enum role);

    Uml::Visibility::Enum visibility(Uml::RoleType::Enum role) const;
    void setVisibility(Uml::Visibility::Enum value, Uml::RoleType::Enum role);

    FloatingTextWidget* changeabilityWidget(Uml::RoleType::Enum role) const;
    Uml::Changeability::Enum changeability(Uml::RoleType::Enum role) const;
    void setChangeability(Uml::Changeability::Enum value, Uml::RoleType::Enum role);

    Uml::ID::Type widgetIDForRole(Uml::RoleType::Enum role) const;
    UMLWidget* widgetForRole(Uml::RoleType::Enum role) const;
    void setWidgetForRole(UMLWidget* widget, Uml::RoleType::Enum role);

    void setWidgets(UMLWidget *widgetA, UMLWidget *widgetB);

    bool associates(UMLWidget *a, UMLWidget *b) const;
    bool containsWidget(UMLWidget *widget) const;

    Uml::RoleType::Enum roleForWidget(UMLWidget *widget) const;
    bool isSelf() const;

    ClassifierWidget* associationClass() const;
    void setAssociationClass(ClassifierWidget *classifier);

    Uml::AssociationType::Enum associationType() const;
    void setAssociationType(Uml::AssociationType::Enum type);

    bool isCollaboration() const;

    QString toString() const;

    AssociationLine* associationLine() const;

    virtual bool activate();
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

    bool loadFromXMI(QDomElement& qElement, const UMLWidgetList& widgets,
                     const MessageWidgetList* messages);
    virtual bool loadFromXMI(QDomElement& qElement);
    virtual void saveToXMI(QDomDocument &qDoc, QDomElement &qElement);

    virtual void contextMenu(const QPointF& pos, Qt::KeyboardModifiers modifiers, const QPoint& screenPos);

public Q_SLOTS:
    virtual void slotMenuSelection(QAction* action);

protected Q_SLOTS:
    virtual void slotUMLObjectDataChanged();
    virtual void slotClassifierListItemRemoved(UMLClassifierListItem* classifierItem);

protected:
    virtual void updateGeometry();
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);
    virtual QVariant attributeChange(WidgetAttributeChange change, const QVariant& oldValue);

    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    virtual void umlObjectChanged(UMLObject *old);

private:
    void init();

    UMLScenePoint calculateTextPosition(Uml::TextRole::Enum role);
    void setTextPosition(Uml::TextRole::Enum role);
    void setFloatingText(Uml::TextRole::Enum role, const QString& text, FloatingTextWidget* ft);

    void updateNameWidgetRole();

    friend class AssociationLine;

    AssociationLine *m_associationLine;
    ClassifierWidget *m_associationClass;
    Uml::AssociationType::Enum m_associationType;
    WidgetRole m_role[2];
    FloatingTextWidget *m_nameWidget;
    int m_nameSegmentIndex;

    bool m_slotUMLObjectDataChangedFirstCall;
};

#endif
