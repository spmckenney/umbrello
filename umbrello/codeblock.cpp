
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Wed Jun 18 2003
 */

#include "codeblock.h"
#include "codedocument.h"

#include "hierarchicalcodeblock.h"

// Constructors/Destructors
//

CodeBlock::CodeBlock ( HierarchicalCodeBlock * hblock , const QString & body )
        : TextBlock ( hblock->getParentDocument(), body )
{
    initFields ( );
}

CodeBlock::CodeBlock ( CodeDocument * doc, const QString & body )
        : TextBlock ( doc, body )
{
    initFields( );
}

CodeBlock::~CodeBlock ( ) { }

//
// Methods
//


// Accessor methods
//


// Public attribute accessor methods
//


/**
 * Set the value of m_contentType
 * specifies whether the content (text) of this object was generated by the code
 * generator or was supplied by the user.
 * @param new_var the new value of m_contentType
 */
void CodeBlock::setContentType ( ContentType new_var ) {
    m_contentType = new_var;
}

/**
 * Get the value of m_contentType
 * specifies whether the content (text) of this object was generated by the code
 * generator or was supplied by the user.
 * @return the value of m_contentType
 */
CodeBlock::ContentType CodeBlock::getContentType ( ) {
    return m_contentType;
}

/**
 * Get the value of m_dialog
 * @return the value of m_dialog
 */
/*
CodeBlockDialog * CodeBlock::getDialog ( ) {
	return m_dialog;
}
*/

// Other methods
//

/**
 * Save the XMI representation of this object
 */
void CodeBlock::saveToXMI ( QDomDocument & doc, QDomElement & root ) {
    QDomElement blockElement = doc.createElement( "codeblock" );

    // set attributes
    setAttributesOnNode(doc, blockElement);

    root.appendChild( blockElement );
}

void CodeBlock::setAttributesOnNode( QDomDocument & doc, QDomElement & blockElement)
{

    // call super-class
    TextBlock::setAttributesOnNode(doc, blockElement);

    // local attributes
    if(m_contentType != AutoGenerated)
        blockElement.setAttribute("contentType",getContentType());

}

/**
 * load params from the appropriate XMI element node.
 */
void CodeBlock::loadFromXMI ( QDomElement & root ) {

    setAttributesFromNode(root);

}

void CodeBlock::setAttributesFromNode (QDomElement & elem) {

    // set attributes from the XMI in super-class
    TextBlock::setAttributesFromNode(elem);

    // set local fields now
    setContentType(((ContentType) elem.attribute("contentType","0").toInt()));

}

void CodeBlock::setAttributesFromObject(TextBlock * obj)
{

    TextBlock::setAttributesFromObject(obj);

    CodeBlock * cb = dynamic_cast<CodeBlock*>(obj);
    if(cb)
        setContentType(cb->getContentType());

}

void CodeBlock::initFields ( ) {
    m_contentType = AutoGenerated;
    //	m_dialog = new CodeBlockDialog( );
}

#include "codeblock.moc"
