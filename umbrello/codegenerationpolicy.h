/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   copyright (C) 2004-2006                                               *
 *   Umbrello UML Modeller Authors <uml-devel@uml.sf.net>                  *
 ***************************************************************************/

/*  This code generated by:
 *      Author : thomas
 *      Date   : Fri Jun 20 2003
 */



#ifndef CODEGENERATIONPOLICY_H
#define CODEGENERATIONPOLICY_H

#include <qobject.h>
#include <qdir.h>
#include <qdom.h>

class QWidget;
class KConfig;
class CodeGenerationPolicyPage;

/**
  * class CodeGenerationPolicy
  * This class describes the code generation policy for this project.
  */

class CodeGenerationPolicy : public QObject {
    Q_OBJECT
public:

    /**
     * OverwritePolicy  can have the following values
     *  - Ok: if there is a file named the same as what you want to name your output file,
     *        you can overwrite the old file.
     *  - Ask:if there is a file named the same as what you want to name your output file,
     *        you should ask the User what to do, and give him the option to overwrite the file
     *        write the code to a different file, or to abort the generation of this class.
     *  - Never: you cannot overwrite any files. Generates a new file name like "fileName1.h", "fileName2.h"
     *        until you find an appropriate name.
     *  - Cancel: Do not output anything.  This is only set if the user chooses Apply to All Remaining Files
     *            and clicks on Do not Output  in the Ask dialog
     */
    enum OverwritePolicy {Ok=0, Ask, Never, Cancel};
    enum ModifyNamePolicy {No=0, Underscore, Capitalise};
    enum NewLineType {UNIX=0, DOS, MAC};
    enum IndentationType {NONE=0, TAB, SPACE};
    enum CommentStyle { SingleLine=0, MultiLine };
    enum ScopePolicy { Public=200, Private, Protected, FromParent };

    // set some reasonable defaults
    OverwritePolicy  defaultOverwritePolicy()          const;
    bool             defaultVerboseSectionComments()   const;
    bool             defaultVerboseDocumentComments()  const;
    bool             defaultIncludeHeadings()          const;
    NewLineType      defaultLineEndingType()           const;
    IndentationType  defaultIndentType()               const;
    int              defaultIndentAmount()             const;
    ModifyNamePolicy defaultModifyNamePolicy()         const;
    CommentStyle     defaultCommentStyle()             const;
    ScopePolicy      defaultAttribAccessorScope()      const;
    ScopePolicy      defaultAssocFieldScope()          const;
    bool             defaultAutoGenerateConstructors() const;

    // Constructors/Destructors
    //

    /**
     * Constructor
     */
    // note that as the code gen policy may be the 'default' policy, it may
    // not be coupled with a code generator.
    CodeGenerationPolicy (CodeGenerationPolicy * clone = 0);
    CodeGenerationPolicy (KConfig * config );

    /**
     * Empty Destructor
     */
    virtual ~CodeGenerationPolicy ( );

    // Public attributes
    //

    // Public attribute accessor methods
    //

    /**
     * Set the value of m_overwritePolicy
     * Policy of how to deal with overwriting existing files.
     * OverwritePolicy  can have the following values
     *  - Ok: if there is a file named the same as what you want to name your output file,
     *        you can overwrite the old file.
     *  - Ask:if there is a file named the same as what you want to name your output file,
     *        you should ask the User what to do, and give him the option to overwrite the file
     *        write the code to a different file, or to abort the generation of this class.
     *  - Never: you cannot overwrite any files. Generates a new file name like "fileName1.h", "fileName2.h"
     *        until you find an appropriate name.
     *  - Cancel: Do not output anything.  This is only set if the user chooses Apply to All Remaining Files
     *            and clicks on Do not Output  in the Ask dialog
     *
     * @param new_var the new value of m_overwritePolicy
     */
    void setOverwritePolicy ( OverwritePolicy new_var );

    /**
     * Get the value of m_overwritePolicy
     * Policy of how to deal with overwriting existing files. Allowed values are "Ok", "Ask",
     * "Never" "Cancel".
     * @return the value of m_overwritePolicy
     */
    OverwritePolicy getOverwritePolicy ( ) const;


    /**
     * Set the value of m_codeVerboseSectionComments
     * Whether or not verbose code commenting for sections is desired. If true, comments
     * for sections will be written even if the section is empty.
     * @param new_var the new value of m_codeVerboseSectionComments
     */
    void setCodeVerboseSectionComments ( bool new_var );

    /**
     * Get the value of m_codeVerboseSectionComments
     * Whether or not verbose code commenting for sections is desired. If true, comments
     * for sections will be written even if the section is empty.
     * @return the value of m_codeVerboseSectionComments
     */
    bool getCodeVerboseSectionComments ( ) const;


    /**
     * Set the value of m_codeVerboseDocumentComments
     * Whether or not verbose code commenting for documentation is desired. If true,
     * documentation for various code will be written even if no code would normally be
     * created at that point in the file.
     * @param new_var the new value of m_codeVerboseDocumentComments
     */
    void setCodeVerboseDocumentComments ( bool new_var );

    /**
     * Get the value of m_codeVerboseDocumentComments
     * Whether or not verbose code commenting for documentation is desired. If true,
     * documentation for various code will be written even if no code would normally be
     * created at that point in the file.
     * @return the value of m_codeVerboseDocumentComments
     */
    bool getCodeVerboseDocumentComments ( ) const;

    /**
     * Set the value of m_headingFileDir
     * location of the header file template.
     * @param path the new path of m_headingFileDir
     */
    void setHeadingFileDir ( const QString & path);

    /**
     * Get the value of m_headingFileDir
     * location of the header file template.
     * @return the value of m_headingFileDir
     */
    QString getHeadingFileDir ( ) const;


    /**
     * Set the value of m_includeHeadings
     * @param new_var the new value of m_includeHeadings
     */
    void setIncludeHeadings ( bool new_var );

    /**
     * Get the value of m_includeHeadings
     * @return the value of m_includeHeadings
     */
    bool getIncludeHeadings ( ) const;


    /**
     * Set the value of m_outputDirectory
     * location of where output files will go.
     * @param new_var the new value of m_outputDirectory
     */
    void setOutputDirectory ( QDir new_var );

    /**
     * Get the value of m_outputDirectory
     * location of where output files will go.
     * @return the value of m_outputDirectory
     */
    QDir getOutputDirectory ( );

    /**
     * Set the value of m_lineEndingType
     * What line ending characters to use.
     * @param new_var the new value of m_lineEndingType
     */
    void setLineEndingType ( NewLineType new_var );

    /**
     * Get the value of m_lineEndingType
     * What line ending characters to use.
     * @return the value of m_lineEndingType
     */
    NewLineType getLineEndingType ( );

    /** Utility function to get the actual characters.
     */
    QString getNewLineEndingChars ( ) const;

    /**
     * Set the value of m_indentationType
     * The amount and type of whitespace to indent with.
     * @param type the new value of m_indentationType
     */
    void  setIndentationType ( IndentationType type );

    /**
     * Get the value of m_indentationType
     */
    IndentationType getIndentationType ( );

    /** How many units to indent for each indentation level.
     */
    void  setIndentationAmount ( int amount );
    int getIndentationAmount ( );

    /**
     * Get the string representation of each level of indentation.
     * The amount and type of whitespace to indent with.
     * @return the value of m_indentationType
     */
    QString getIndentation ( ) const;

    /**
     * Set the value of m_modifyPolicy
     * @param new_var the new value of m_modifyPolicy
     */
    void setModifyPolicy ( ModifyNamePolicy new_var );

    /**
     * Get the value of m_modifyPolicy
     * @return the value of m_modifyPolicy
     */
    ModifyNamePolicy getModifyPolicy ( ) const;

    /**
     * Set the value of m_autoGenerateConstructors
     * @param var     the new value
     */
    void setAutoGenerateConstructors ( bool var );

    /**
     * Get the value of m_autoGenerateConstructors
     * @return value the boolean value of m_autoGenerateConstructors
     */
    bool getAutoGenerateConstructors ( );

    /**
     * Set the value of m_attributeAccessorScope
     * @param var the new value
     */
    void setAttributeAccessorScope(ScopePolicy var);

    /**
     * Get the value of m_attributeAccessorScope
     * @return the ScopePolicy value of m_attributeAccessorScope
     */
    ScopePolicy getAttributeAccessorScope();

    /**
     * Set the value of m_associationFieldScope
     * @param var the new value
     */
    void setAssociationFieldScope(ScopePolicy var);

    /**
     * Get the value of m_associationFieldScope
     * @return the ScopePolicy value of m_associationFieldScope
     */
    ScopePolicy getAssociationFieldScope();

    /**
     * Create a new dialog interface for this object.
     * @return dialog object
     */
    virtual CodeGenerationPolicyPage * createPage ( QWidget *parent = 0, const char * name = 0);

    /**
      *  Gets the heading file (as a string) to be inserted at the
      *  beginning of the generated file. you give the file type as
      *  parameter and get the string. if fileName starts with a
      *  period (.) then fileName is the extension (.cpp, .h,
      *  .java) if fileName starts with another character you are
      *  requesting a specific file (mylicensefile.txt).  The files
      *  can have parameters which are denoted by %parameter%.
      *
      *  current parameters are
      *  %author%
      *  %date%
      *  %time%
      *  %filepath%
      */
    QString getHeadingFile(QString str);

    /**
     * set the defaults for this code generator from the passed generator.
     */
    virtual void setDefaults (CodeGenerationPolicy * defaults, bool emitUpdateSignal = true);

    /**
     * set the defaults from a config file for this code generator from the passed KConfig pointer.
     */
    virtual void setDefaults(KConfig * config, bool emitUpdateSignal = true);

    /**
     * write Default params to passed KConfig pointer.
     */
    virtual void writeConfig (KConfig * config);

    void emitModifiedCodeContentSig();

    /**
     * Set the value of m_commentStyle
     * @param new_var the new value of m_commentStyle
     */
    void setCommentStyle ( CommentStyle new_var );

    /**
     * Get the value of m_commentStyle
     * @return the value of m_commentStyle
     */
    CommentStyle getCommentStyle ( );

signals:

    // this signal is sent whenever a change is made to the policy
    // which could modifiy code document content
    void modifiedCodeContent();

protected:

    // Policy of how to deal with overwriting existing files. Allowed values are "ask", "yes" and "no".
    OverwritePolicy m_overwritePolicy;

    // Whether or not verbose code commenting for sections is desired.
    // If true, comments for sections will be written even if the section is empty.
    bool m_codeVerboseSectionComments;

    // Whether or not verbose code commenting for documentation is desired.
    // If true, documentation for various code will be written even if no
    //code would normally be created at that point in the file.
    bool m_codeVerboseDocumentComments;

    QDir m_headingFiles; // location of the header file template.
    bool m_includeHeadings;
    QDir m_outputDirectory; // location of where output files will go.
    NewLineType m_lineEndingType; // What type of line ending characters to use.
    IndentationType m_indentationType; // The amount and type of whitespace to indent with.
    int m_indentationAmount; // The amount of units to indent with.
    ModifyNamePolicy m_modifyPolicy;
    bool m_autoGenerateConstructors;
    CommentStyle m_commentStyle;
    ScopePolicy m_attributeAccessorScope;
    ScopePolicy m_associationFieldScope;

    // these 2 private fields 'cache' the string values of other fields we may frequently call for
    QString m_lineEndingChars;
    QString m_indentation;

    void calculateIndentation ( );

protected:

    void initFields ( );

};

#endif // CODEGENERATIONPOLICY_H
