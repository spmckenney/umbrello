/***************************************************************************
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2003      Brian Thomas <thomas@mail630.gsfc.nasa.gov>   *
 *   copyright (C) 2004-2010                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

#include "codemethodblock.h"

#include "codeclassfield.h"
#include "classifiercodedocument.h"
#include "codegenerationpolicy.h"
#include "uml.h"

CodeMethodBlock::CodeMethodBlock ( ClassifierCodeDocument * doc, UMLObject * parentObj, const QString & body, const QString & comment)
        : OwnedCodeBlock (parentObj), CodeBlockWithComments ((CodeDocument*)doc, body, comment) 
{
    m_startMethod.clear();
    m_endMethod.clear();
}

CodeMethodBlock::~CodeMethodBlock ( )
{ 
}

// we can just call the superclass

/**
 * Get the parent code document.
 */
CodeDocument * CodeMethodBlock::getParentDocument()
{

/**
 * Get the parent code document.
 */
    return TextBlock::getParentDocument();
}

/**
 * Get the starting text that begins this method before the body is printed.
 */
QString CodeMethodBlock::getStartMethodText() const
{
    return m_startMethod;
}

/**
 * Get the ending text that finishes this method after the body is printed.
 */
QString CodeMethodBlock::getEndMethodText() const
{
    return m_endMethod;
}

/**
 * Set the starting text that begins this method before the body is printed.
 */
void CodeMethodBlock::setStartMethodText (const QString &value)
{
    m_startMethod = value;
}

/**
 * Set the ending text that finishes this method after the body is printed.
 */
void CodeMethodBlock::setEndMethodText (const QString &value)
{
    m_endMethod = value;
}

/**
 * Causes the text block to release all of its connections
 * and any other text blocks that it 'owns'.
 * needed to be called prior to deletion of the textblock.
 */
void CodeMethodBlock::release ()
{
    // just call super-class versions
    OwnedCodeBlock::release();
    TextBlock::release();
}

/**
 * Set attributes of the node that represents this class
 * in the XMI document.
 */
void CodeMethodBlock::setAttributesOnNode ( QDomDocument & doc, QDomElement & elem)
{
    // set super-class attributes
    CodeBlockWithComments::setAttributesOnNode(doc, elem);
    OwnedCodeBlock::setAttributesOnNode(doc, elem);

    // set local class attributes
    if (getContentType() != AutoGenerated)
    {
        QString endLine = UMLApp::app()->commonPolicy()->getNewLineEndingChars();
        elem.setAttribute("startMethodText",encodeText(getStartMethodText(),endLine));
        elem.setAttribute("endMethodText",encodeText(getEndMethodText(),endLine));
    }
}

/**
 * Set the class attributes of this object from
 * the passed element node.
 */
void CodeMethodBlock::setAttributesFromNode ( QDomElement & elem)
{
    // set attributes from the XMI
    CodeBlockWithComments::setAttributesFromNode(elem); // superclass load
    OwnedCodeBlock::setAttributesFromNode(elem); // superclass load

    // now load local attributes
    if (getContentType() != AutoGenerated)
    {
        QString endLine = UMLApp::app()->commonPolicy()->getNewLineEndingChars();
        setStartMethodText(decodeText(elem.attribute("startMethodText",""),endLine));
        setEndMethodText(decodeText(elem.attribute("endMethodText",""),endLine));
    }
}

/**
 * Set the class attributes from a passed object
 */
void CodeMethodBlock::setAttributesFromObject(TextBlock * obj)
{
    CodeBlockWithComments::setAttributesFromObject(obj);

    CodeMethodBlock * mb = dynamic_cast<CodeMethodBlock*>(obj);
    if (mb)
    {
        setStartMethodText(mb->getStartMethodText());
        setEndMethodText(mb->getEndMethodText());
    }
}

/**
 * @return  QString
 */
QString CodeMethodBlock::toString ( ) const
{
    QString string;

    if (getWriteOutText()) {
        QString indent = getIndentationString();
        QString bodyIndent = getIndentationString(getIndentationLevel()+1);
        QString endLine = UMLApp::app()->commonPolicy()->getNewLineEndingChars();

        QString startMethod = formatMultiLineText ( getStartMethodText(), indent, endLine);
        QString body = formatMultiLineText (getText(), bodyIndent, endLine);
        QString endMethod = formatMultiLineText( getEndMethodText(), indent, endLine);

        QString comment = getComment()->toString();
        if (!comment.isEmpty() && getComment()->getWriteOutText())
            string.append(comment);

        if (!startMethod.isEmpty())
            string.append(startMethod);

        if (!body.isEmpty())
            string.append(body);

        if (!endMethod.isEmpty())
            string.append(endMethod);

    }
    return string;
}

void CodeMethodBlock::syncToParent ( )
{
    getComment()->setText(getParentObject()->getDoc());

    updateMethodDeclaration();

    // only update IF we are NOT AutoGenerated
    if (getContentType() != AutoGenerated)
        return;

    updateContent();
}

#include "codemethodblock.moc"
