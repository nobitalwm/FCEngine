#ifndef FCENGINEEDITOR_COMMANDBASE_H__INCLUDE
#define FCENGINEEDITOR_COMMANDBASE_H__INCLUDE

class CEditorCommandBase
{
public:
    CEditorCommandBase();
    virtual ~CEditorCommandBase();

    virtual void Do() = 0;
    virtual void UnDo() = 0;
private:

};

#endif
