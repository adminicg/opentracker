// ===========================================================================
//  (c) 2000 Vienna University of Technology
// ===========================================================================
//  NAME:      CursesOutput.h
//  TYPE:      h header file
//  PROJECT:   TrackerServer
//  CONTENT:   Declaration of class 'CursesOutput'
//  VERSION:   1.0
// ===========================================================================
//  Author:    reitmayr  Gerhard Reitmayr
// ===========================================================================
//  HISTORY:
//
//  @INSERT_MODIFICATIONS(// )
// August 25, 2000 13:44 gr added NetworkSource classes
//     Update comment header
// ===========================================================================
#ifndef _CURSESOUTPUT_H
#define _CURSESOUTPUT_H


//@START_USER1
//@END_USER1


/*@NOTE_7206
A Node to provide output using the curses library. It passes state information from its child
node to its parent node.
*/
class CursesOutput
    : public TreeNode
{

//@START_USER2
//@END_USER2

// Members
private:

protected:
    char* comment;

public:

// Methods
private:
    void ConstructorInclude();
    void DestructorInclude();

protected:

public:
    CursesOutput(char* comment_);
    virtual ~CursesOutput();
    char* getComment();
    virtual State* getState();
};

#endif


#ifdef CB_INLINES
#ifndef _CURSESOUTPUT_H_INLINES
#define _CURSESOUTPUT_H_INLINES

//@START_USER3
//@END_USER3

#endif
#endif