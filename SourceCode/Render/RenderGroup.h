#ifndef RENDER_RENDER_GROUP_H__INCLUDE
#define RENDER_RENDER_GROUP_H__INCLUDE

class CRenderObject;

class CRenderGroup
{
public:
    CRenderGroup(int ID);
    virtual ~CRenderGroup();

    virtual void PreRender();
    virtual void Render();
    virtual void PostRender();

    int ID() const;
    virtual void AddRenderObject(CRenderObject *object);
    virtual void Clear();

protected:
    int m_nID;
    std::vector<CRenderObject *> m_objects;
};

#endif // !RENDER_RENDER_QUEUE_H__INCLUDE
