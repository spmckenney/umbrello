
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
 *      Date   : Thu Jun 19 2003
 *  Bugs and comments to uml-devel@lists.sf.net or http://bugs.kde.org
 */



#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include <klibloader.h>

#include <qstring.h>
#include <qstringlist.h>
#include <qdict.h>
#include <qdom.h>
#include <qptrlist.h>
#include <qfile.h>
#include <qobject.h>
#include <kinstance.h>

#include "codegenerationpolicy.h"
#include "codeaccessormethod.h"
#include "umlclassifierlist.h"

class UMLAttribute;
class UMLDoc;
class UMLObject;
class UMLRole;
class UMLOperation;

class ClassifierCodeDocument;
class CodeAccessorMethod;
class CodeBlock;
class CodeBlockWithComments;
class CodeClassField;
class CodeClassFieldDeclarationBlock;
class CodeComment;
class CodeDocument;
class CodeOperation;
class CodeViewerDialog;

class KConfig;

#include "dialogs/codeviewerdialog.h"

/**
  * class CodeGenerator
  * This class collects together all of the code documents which form this project,
  * and generates code for them in a given language.
  */

/**
 * CodeGenerator is the base class for all CodeGenerators. It
 * provides the interface through which all Generators are invoked and
 * the all the basic functionality. The only thing it doesn't do is to
 * generate code =)
 *
 * If you want to implement a CodeGenerator for some language follow
 * these steps:
 *
 * Create a class which inherits CodeGenerator. This class can have
 * any name, I use names like CppCodeGenerator for the Cpp Generator,
 * JavaCodeGenerator  for the Java Generator and so on, but you can use what
 * you want.
 *
 * The code you generate should be output to "outputDirectory" and you
 * should respect the OverwritePolicy specified. You should call
 * findFileName(..) to get an appropriate file name, and then you can
 * call openFile if you want, but if you want to do it yourself you
 *
 * Finally put your generator in a library which can be dlopened
 * together with a factory class (see below) and you are ready to go.
 */


class CodeGenerator : public QObject {
      Q_OBJECT

public:

	// Constructors/Destructors
	//  

	/**
	 * Constructors
	 */

        /**
	 * Build a code generator.
         * @return      CodeGenerator
         * @param       doc 
         */
        CodeGenerator (UMLDoc * doc, const char *name);

        /**
	 * Build a code generator and then initialize it from an XMI element.
         * @return      CodeGenerator
         * @param       doc 
         * @param       element an element from an XMI document 
         */
        CodeGenerator (UMLDoc * doc, const char *name, QDomElement & element );

	/**
	 * Empty Destructor
	 */
	virtual ~CodeGenerator ( );

	// Public attributes
	//  

	// Public attribute accessor methods
	//  

	/**
	 * Add a CodeDocument object to the m_codedocumentVector List
         * @return boolean - will return false if it couldnt add a document.
	 */
	bool addCodeDocument ( CodeDocument * add_object );

	/**
	 * Replace (or possiblity Add a new) CodeDocument object to the m_codedocumentVector List. 
	 * As names must be unique and each code document must have a name.
	 * @return  boolean value which will be true if the passed document was able to replace some
         *          other document OR was added(no prior document existed..only when addIfPriorDocumentNotPresent is true). 
         *          The document which was replaced will be deleted IF deleteReplacedDocument is true.
	 */
	// bool replaceCodeDocument (  CodeDocument * replace_doc=0, bool addIfPriorDocumentNotPresent=true,
        //                                      bool deleteReplacedDocument=true ); 

	/**
	 * Remove a CodeDocument object from m_codedocumentVector List
         * @return boolean - will return false if it couldnt remove a document.
	 */
	bool removeCodeDocument ( CodeDocument * remove_object );

	/**
	 * Get the list of CodeDocument objects held by m_codedocumentVector
	 * @return QPtrList<CodeDocument *> list of CodeDocument objects held by
	 * m_codedocumentVector
	 */
	QPtrList<CodeDocument> * getCodeDocumentList ( );

	/**
	 * Set the codeGeneratorPolicy for this generator.
	 */
	void setPolicy ( CodeGenerationPolicy * object );

	/**
	 * Get the codeGeneratorPolicy for this generator.
	 */
	CodeGenerationPolicy * getPolicy ( );

	/**
	 * Get the value of m_document
	 * @return the value of m_document
	 */
	UMLDoc * getDocument ( );

	// get a unique id for this codedocument
	QString getUniqueID ( CodeDocument * codeDoc );

	/**
	 * @return	QString
	 */
	virtual bool saveToXMI ( QDomDocument & doc, QDomElement & root );

	/**
         * Find a code document by the given id string.
	 * @return	CodeDocument
   	 */
	CodeDocument * findCodeDocumentByID(QString id);

	/**
	 * Write out all code documents to file as appropriate.
	 */
	virtual void writeCodeToFile ( );

        // this method is here to provide class wizard the
        // ability to write out only those classes which
        // are selected by the user.
        virtual void writeCodeToFile(UMLClassifierList &list);

	// these are utility methods for accessing the default 
        // code gen policy object and *perhaps* should go away when we 
	// finally implement the CodeGenDialog class -b.t. 
        QString getNewLineEndingChars ( );

        void setOutputDirectory(QString d);
        QString outputDirectory() const;

        void setOverwritePolicy(CodeGenerationPolicy::OverwritePolicy p);
        CodeGenerationPolicy::OverwritePolicy overwritePolicy() const;

        void setModifyNamePolicy(CodeGenerationPolicy::ModifyNamePolicy p);
        CodeGenerationPolicy::ModifyNamePolicy modifyNamePolicy()const;

        void setIncludeHeadings(bool i);
        bool includeHeadings() const;

        void setHeadingFileDir(const QString &);
        QString headingFileDir() const;

        void setForceDoc(bool f);
        bool forceDoc() const;

        void setForceSections(bool f);
        bool forceSections() const;


        /**
         *  Gets the heading file (as a string) to be inserted at the
         *  begining of the generated file. you give the file type as
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
         *
         * @return      QString
         * @param       file 
         */
        virtual QString getHeadingFile (QString file );

        /**
         * Finds an appropiate file name for class c, taking into account the Overwrite
         * Policy and asking the user what to do if need be. (if policy == Ask)
         *
         * @param concept the class for which an output file name is desired.
         * @return the file name that should be used. (with extension) or 
         *      NULL if none to be used
         */
        QString findFileName(CodeDocument * doc);

        /**
         * Replaces spaces with underscores and capitalises as defined in m_modname
         * @return      QString
         * @param       name 
         */
        QString cleanName (QString name );

       /** Format documentation for output in source files
         *
         * @param text the documentation which has to be formatted
         * @param linePrefix the prefix which has to be added in the beginnig of each line
         * @param lineWidth the line width used for word-wrapping the documentation
         *
         * @return the formatted documentation text
         */
        QString formatDoc (const QString & text, const QString & lineprefix = " *", int linewidth = 80 );

       /**
         * Finds all classes in the current document to which objects of class c
         * are in some way related. Posible relations are Associations (gneratlization,
         * composition, etc) as well as parameters to methos and return values
         * this is useful in deciding which classes/files to import/include in code generation
         * @param c the class for which relations are to be found
         * @param cList a reference to the list into which return the result
         */
        void findObjectsRelated(UMLClassifier *c, UMLClassifierList &cList);

        // a series of accessor method constructors that we need to define
        // for any particular language.
        virtual CodeDocument * newClassifierCodeDocument (UMLClassifier * classifier ) = 0;

       /** get the name of the class which holds lists, e.g. "QPtrlist" or
         * "Vector" or "List" and so on. Not all languages may need this.
         */
	virtual QString getListFieldClassName();

        /**
         * @param       element
         */
        virtual void loadFromXMI (QDomElement & element );

        /**
         * Create a new code generation policy object.
         * This is not pure virutal because the vannila codegenerationpolicy may be
         * of sufficient utility that a sub-class isnt needed.
         */
        virtual CodeGenerationPolicy * newCodeGenerationPolicy ( KConfig * config );

	/**
 	 * Create a new Code document belonging to this package.
	 * @return CodeDocument pointer to new code document.
         */
	virtual CodeDocument * newCodeDocument ( );

        /**
         * Return the unique language string that identifies this type of code generator
         */
        virtual QString getLanguage() = 0;

        /**
         * Find a code document by the given classifier.
         * @return      CodeDocument
         * @param       classifier
         */
//FIX
	// NOTE: this should be 'protected' or we could have problems with CPP code generator
	CodeDocument * findCodeDocumentByClassifier (UMLClassifier * classifier );

	/**
	 * Create the default datatypes for your language (bool, int etc)
	 */
	virtual void createDefaultDatatypes();

        /** Get the editing dialog for this code document
         */
        virtual CodeViewerDialog * getCodeViewerDialog( QWidget* parent, CodeDocument * doc, 
                                                        CodeViewerDialog::CodeViewerState state);

protected:

        /**
         * Remove (and possibly delete) all AutoGenerted content type CodeDocuments but leave 
	 * the UserGenerated (and any other type) documents in this generator alone.
         */
	// void removeAndDeleteAllAutoGeneratedCodeDocuments ( bool deleteRemovedDocs=true );

        /**
         * Returns a name that can be written to in the output directory,
         * respecting the overwrite policy.
         * If a file of the given name and extension does not exist,
         * then just returns the name.
         * If a file of the given name and extension does exist,
         * then opens an overwrite dialog. In this case the name returned
         * may be a modification of the input name.
         * This method is invoked by findFileName().
         *
         * @param name the proposed output file name (incl. extension) 
         * @return the real file name that should be used. (with extension?) or 
         *      NULL if none to be used
         */
	QString overwritableName ( QString name );

        /** Opens a file named "name" for writing in the outputDirectory.
         * If something goes wrong, it informs the user
         * if this function returns true, you know you can write to the file
         * @return      bool
         * @param       file 
         * @param       name 
         */
        bool openFile (QFile & file, QString name );

       /**
         * Initialize this code generator from its parent UMLDoc. When this is called, it will
         * (re-)generate the list of code documents for this project (generator) by checking
         * for new objects/attributes which have been added or changed in the documnet. One or more
         * CodeDocuments will be created/overwritten/amended as is appropriate for the given language.
         */
        virtual void initFromParentDocument( );

        /** the actual internal routine which writes code documents
         */
        void writeCodeToFile(QPtrList<CodeDocument> & docs);

	static const char * hierarchicalCodeBlockNodeName;

	// map of what code documents we currently have in this generator.
	QDict<CodeDocument> m_codeDocumentDictionary;

        /**
         * used by overwriteDialogue to know if the apply to all
         * remaining files checkbox should be checked (is by default)
         */
        bool m_applyToAllRemaining;

	/**
	 * The document object
	 */
        UMLDoc* m_document;
private:

        /**
         * Maps CodeDocuments to filenames. Used for finding out which file
         * each class was written to.
         */
	// this seems silly and overkill now. -b.t.
        // QMap<CodeDocument*,QString> *m_fileMap;

        QPtrList<CodeDocument> m_codedocumentVector;
        CodeGenerationPolicy * m_codegeneratorpolicy;
	int lastIDIndex;

	void initFields ( UMLDoc * doc ) ;

public slots:

	/** These 2 functions check for adding or removing objects to the UMLDocument */
	virtual void checkAddUMLObject (UMLObject * obj);
	virtual void checkRemoveUMLObject (UMLObject * obj);

        /**
         * Force a synchronize of this code generator, and its present contents, to that of the parent UMLDocument.
         * "UserGenerated" code will be preserved, but Autogenerated contents will be updated/replaced
         * or removed as is apppropriate.
         */
        virtual void syncCodeToDocument ( );

signals:

        /**
         * This signal is emitted when code for a UMLClassifier has been
         * generated. Its only really used by the codegenerationwizard to
	 * update its progress.
         * @param concept    The concept which was processed
         * @param generated  Flag, set to true if generation was successfull
         */
	void codeGenerated(UMLClassifier* concept, bool generated);

};

//FIXME
// is this class really needed still??
//
// not really, it's just too much effort to replace it with a QStringList
// (needs editing uml.cpp in various places) -- jr
class GeneratorInfo {
public:
        QString language;
        QString object;

};

#endif // CODEGENERATOR_H
