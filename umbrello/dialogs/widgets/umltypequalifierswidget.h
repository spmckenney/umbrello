#ifndef UMLTYPEQUALIFIERS_H
#define UMLTYPEQUALIFIERS_H

#include "basictypes.h"

#include <QtWidgets>
#include <QMap>

class UMLObject;

class UmlTypeQualifiersWidget : public QWidget
{
    Q_OBJECT
public:
    explicit UmlTypeQualifiersWidget(UMLObject *o, QWidget *parent = 0);

    void addToLayout(QGridLayout *layout,int row);
    void apply();

    typedef QMap<Uml::TypeQualifiers::Enum,QString> TextMap;
    typedef QMap<Uml::TypeQualifiers::Enum,QRadioButton*> ButtonMap;

private:
    void init(const QString &title);

    TextMap m_texts;
    QHBoxLayout *m_box;
    ButtonMap m_buttons;
    UMLObject *m_object;
    Uml::RoleType::Enum m_role;
    QLabel *m_label;

};

#endif // UMLTYPEQUALIFIERS_H