/***************************************************************************
                          codegenfactory.cpp  -  description
                             -------------------
    begin                : Mon Jun 17 2002
    copyright            : (C) 2002 by Luis De la Parra Blum
					and Brian Thomas
    email                : luis@delaparra.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "codegenfactory.h"
#include "../codegenerator.h"
#include "../umldoc.h"

// the new
#include "cppcodegenerator.h"
#include "javacodegenerator.h"
#include "xmlschemawriter.h"

// the old
#include "adawriter.h"
// #include "cswriter.h" // missing in action?!?
#include "idlwriter.h"
#include "phpwriter.h"
#include "perlwriter.h"
#include "pythonwriter.h"
#include "sqlwriter.h"
#include "aswriter.h"
#include "jswriter.h"

#include "qstringlist.h"
#include <kdebug.h>

CodeGeneratorFactory::CodeGeneratorFactory()  {
	kdDebug()<<"CodeGeneratorFactory created"<<endl;
}

CodeGeneratorFactory::~CodeGeneratorFactory() {
}

QStringList CodeGeneratorFactory::languagesAvailable() {
	kdDebug()<<"Querying languages available"<<endl;

	QStringList l;
	l.append("Ada");
	l.append("ActionScript");
	l.append("Cpp");
//	l.append("C#");
	l.append("IDL");
	l.append("Java");
	l.append("JavaScript");
 	l.append("Perl");
	l.append("PHP");
	l.append("Python");
	l.append("SQL");
	l.append("XMLSchema");
	return l;
}

QString CodeGeneratorFactory::generatorName(const QString &l) {
	kdDebug()<<"Looking up generator for language "<<l<<endl;
	if (l=="Java")
		return "JavaCodeGenerator";
	if (l=="Cpp")
		return "CppCodeGenerator";
	if (l == "Ada")
		return "AdaWriter";
	if (l == "ActionScript")
		return "ASWriter";
/*
	if (l=="C#")
		return "C#Writer";
*/
	if (l=="IDL")
		return "IDLWriter";
	if (l == "JavaScript")
		return "JSWriter";
	if (l == "PHP")
		return "PHPWriter";
 	if (l == "Perl")
 		return "PerlWriter";
 	if (l == "Python")
 		return "PythonWriter";
 	if (l == "SQL")
 		return "SQLWriter";
 	if (l == "XMLSchema")
 		return "XMLSchemaWriter";
	//else...
	kdDebug()<<"CodeGeneratorFactory::Error: no generator for language "<<l<<endl;
	return "";
}

CodeGenerator* CodeGeneratorFactory::createObject(UMLDoc* doc, const char* name)  {
	CodeGenerator* obj = 0;
	QString cname(name);

	if (doc) {

		if(cname =="JavaCodeGenerator") {
			obj = new JavaCodeGenerator(doc, name);
		} else if(cname == "CppCodeGenerator") {
			obj = new CPPCodeGenerator(doc, name);
 		} else if (cname == "XMLSchemaCodeGenerator") {
			obj = new XMLSchemaWriter(doc, name);
		} else if (cname == "AdaWriter") {
			obj = new AdaWriter(doc, name);
		} else if(cname == "ASWriter") {
			obj = new ASWriter( doc, name );
//		} else if(cname == "C#Writer") {
//			obj = new CsWriter( doc, name );
		} else if(cname == "IDLWriter") {
			obj = new IDLWriter( doc, name );
		} else if(cname == "JSWriter") {
			obj = new JSWriter( doc, name );
		} else if (cname == "PHPWriter") {
			obj = new PhpWriter( doc, name);
 		} else if (cname == "PerlWriter") {
			obj = new PerlWriter( doc, name);
 		} else if (cname == "PythonWriter") {
			obj = new PythonWriter( doc, name);
 		} else if (cname == "SQLWriter") {
			obj = new SQLWriter( doc, name);
		} else {
			kdWarning() << "cannot create object of type " << name <<
				". Type unknown" << endl;
		}

	} else {
		kdWarning() << "cannot create parent UML document" << endl;
	}
	return obj;
}
