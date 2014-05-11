#ifndef RENDER_RENDER_GROUP_MANAGER_H__INCLUDE
#define RENDER_RENDER_GROUP_MANAGER_H__INCLUDE

class CRenderGroup;

class CRenderGroupManager
{
    BEATS_DECLARE_SINGLETON(CRenderGroupManager);
public:
    enum RenderGroupID
    {
        LAYER_3D = 0,
        LAYER_3D_ALPHA = 1 << 8,
        LAYER_2D = 2 << 8,
        LAYER_GUI = 3 << 8,
        LAYER_USER = 4 << 8,
    };

    CRenderGroup *GetRenderGroup(int groupID);

    void Render();

    void Clear();

private:
    CRenderGroup *createRenderGroup(int groupID);

private:
    std::map<int, CRenderGroup *> m_groupMap;
};

#endif // !RENDER_RENDER_GROUP_MANAGER_H__INCLUDE
