// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      ConfigurationParser.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'ConfigurationParser'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 10, 2000 10:22 Gerhard Reitmayr
//     Update comment header
// ===========================================================================
#ifndef _CONFIGURATIONPARSER_H
#define _CONFIGURATIONPARSER_H


//@START_USER1
//@END_USER1


/*@NOTE_3732
parses the XML configuration file. This class reads the configuration file using DOM and 
builds the tracker tree.
*/
class ConfigurationParser
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    ModuleMap modules;
    NodeFactory* factory;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    ConfigurationParser(NodeFactory* factory_);
    virtual ~ConfigurationParser();
    void addModule(const char* name, Module& module);
    Node* buildTree(DOM_Element& element);
    NodeVector* parseConfigurationFile(const char* filename);
    StringMap* parseElement(DOM_Element& element);
    void removeModule(const char* name);
};

#endif


#ifdef CB_INLINES
#ifndef _CONFIGURATIONPARSER_H_INLINES
#define _CONFIGURATIONPARSER_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
