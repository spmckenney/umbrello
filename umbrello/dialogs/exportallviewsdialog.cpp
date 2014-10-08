/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2006-2014                                               *
 *   Umbrello UML Modeller Authors <umbrello-devel@kde.org>                *
 ***************************************************************************/

// own header
#include "exportallviewsdialog.h"

// kde include files
#include <kfilefiltercombo.h>
#include <klocale.h>

// application specific includes
#include "umlviewimageexportermodel.h"

/**
 * Constructor for UMLViewImageExporterModel.
 *
 * @param parent The parent of the dialog.
 * @param name The internal name.
 *
 * @see KDialog::KDialog
 */
ExportAllViewsDialog::ExportAllViewsDialog(QWidget* parent, const char* name)
  : KDialog(parent)
{
    setObjectName(QString::fromLatin1(name));
    setModal(false);
    setupUi(mainWidget());

    // create and initialize m_imageType
    m_imageType = new KFileFilterCombo(this);
    QSizePolicy sp(QSizePolicy::Preferred, QSizePolicy::Fixed);
    sp.setHorizontalStretch(0);
    sp.setVerticalStretch(0);
    sp.setHeightForWidth(m_imageType->sizePolicy().hasHeightForWidth());
    m_imageType->setSizePolicy(sp);
    m_imageType->setEditable(false);

    m_imageType->setMimeFilter(UMLViewImageExporterModel::supportedMimeTypes(), QString::fromLatin1("image/png"));

    // Cannot give an object name to the layout when using QtDesigner,
    // therefore go and use an editor and add it by hand.
    ui_imageTypeLayout->addWidget(m_imageType);

    ui_imageTypeLabel->setBuddy(m_imageType);

    // reload the strings so the m_imageType tooltip is added
    languageChange();

    m_kURL->setMode(KFile::Directory | KFile::ExistingOnly);
}

/**
 * Destructor for UMLViewImageExporterModel.
 */
ExportAllViewsDialog::~ExportAllViewsDialog()
{
}

/**
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ExportAllViewsDialog::languageChange()
{
    //ExportAllViewsDialogBase::languageChange();
    m_imageType->setToolTip(i18n("The format that the images will be exported to"));
}
