// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      TestModule.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'TestModule'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// ===========================================================================
#ifndef _TESTMODULE_H
#define _TESTMODULE_H


//@START_USER1
//@END_USER1


/*@NOTE_4805
the module and factory to drive the test source nodes.
*/
class TestModule
    : public Module
    , public NodeFactory
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    NodeVector nodes;
    int cycle;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    virtual ~TestModule();
    virtual Node* createNode(char* const name, StringMap& attributes);
    virtual void update();
};

#endif


#ifdef CB_INLINES
#ifndef _TESTMODULE_H_INLINES
#define _TESTMODULE_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif
