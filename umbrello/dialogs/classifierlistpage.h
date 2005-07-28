/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CLASSIFIERLISTPAGE_H
#define CLASSIFIERLISTPAGE_H

//qt  includes
#include <qwidget.h>
#include <q3groupbox.h>
#include <q3listbox.h>
#include <q3textedit.h>
//kde includes
#include <karrowbutton.h>

//app includes
#include "../listpopupmenu.h"
#include "../umlclassifierlistitemlist.h"

class UMLObject;
class UMLClassifier;
class UMLDoc;

/**
 * A dialogue page to display classifier list properties.  This is not normally setup
 * by you.  It is used by the @ref ClassPropDlg and new class wizard.
 *
 * @short A dialogue page to display classifier properties.
 * @author Paul Hensgen, Jonathan Riddell
 * Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */
class ClassifierListPage : public QWidget {
    Q_OBJECT
public:
    /**
     *	Sets up the ClassifierListPage
     *
     *	@param parent	The parent to the ClassAttPage.
     *	@param classifier	The Concept to display the properties of.
     *      @param doc The UMLDoc document
     *      @param type The type of listItem this handles
     */
    ClassifierListPage(QWidget* parent, UMLClassifier* classifier, UMLDoc* doc, Uml::Object_Type type);

    /**
     *	Standard deconstructor.
     */
    ~ClassifierListPage();

    /**
     *	Will move information from the dialogue into the object.
     *	Call when the ok or apply button is pressed.
     */
    void updateObject();

private:
    /**
     *	Set the state of the widgets on the page with the given value.
     *
     *	@param	state	The state to set the widgets as.
     */
    void enableWidgets(bool state);

    /**
     * Saves the documentation for the currently selected item
     */
    void saveCurrentItemDocumentation();

    /**
     * Get classifiers
     */
    UMLClassifierListItemList getItemList();

    /**
     * Attempts to add classifier to the appropriate list
     *
     * @param classifier	Pointer to the classifier to add.
     * @param position	Index at which to insert into the list.
     * @return true if the classifier could be added
     *
     */
    bool addClassifier(UMLClassifierListItem* classifier, int position = -1);

    /**
     * Take classifier, It is the client responsibility to hand over
     * ownership of the classifier, or to delete it.
     * @param classifier	Classifier to take.
     * @param wasAtIndex	Return value: Index in the UMLClassifier's item
     *			list at which the item was taken.
     * @return	Pointer to the UMLClassifierListItem taken.
     */
    UMLClassifierListItem* takeClassifier(UMLClassifierListItem* classifier,
                                          int &wasAtIndex);

    UMLClassifier* m_pClassifier;
    Q3GroupBox* m_pDocGB;
    Q3GroupBox* m_pItemListGB;
    Q3ListBox* m_pItemListLB;
    Q3TextEdit* m_pDocTE;
    Uml::Object_Type m_itemType;

    KArrowButton* m_pUpArrowB;
    KArrowButton* m_pDownArrowB;
    QPushButton* m_pDeleteListItemButton;
    QPushButton* m_pPropertiesButton;

    UMLClassifierListItem* m_pOldListItem;
    UMLDoc* m_pDoc;
    ListPopupMenu* m_pMenu;
    bool m_bSigWaiting;

public slots:
    /**
     * called when list view is clicked on
     * calls enableWidgets()
     */
    void slotClicked(Q3ListBoxItem* item);

    /**
    * Called when an item is selected in a right click menu
    */
    void slotPopupMenuSel(int id);

    void slotListItemCreated(UMLObject* object);
    void slotListItemModified();
    void slotRightButtonClicked(Q3ListBoxItem* item, const QPoint& p);
    void slotRightButtonPressed(Q3ListBoxItem* item, const QPoint& p);

    /**
     * shows properties dialogue for the attribute clicked on
     */
    void slotDoubleClick(Q3ListBoxItem* item);


    /**
     * moves selected attribute up in list
     */
    void slotUpClicked();

    /**
     * moved selected attribute down in list
     */
    void slotDownClicked();

    /**
     * shows dialogue for new attribute
     */
    void slotNewListItem();

    /**
     * removes currently seleted attribute
     */
    void slotDelete();

    /**
     * shows properties dialogue for currently selected attribute
     */
    void slotProperties();
};

#endif
