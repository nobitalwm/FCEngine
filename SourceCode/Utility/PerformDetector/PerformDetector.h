#ifndef BEATS_DEBUGTOOL_PERFORMDETECTOR_PERFORMDETECTOR_H__INCLUDE
#define BEATS_DEBUGTOOL_PERFORMDETECTOR_PERFORMDETECTOR_H__INCLUDE

#include <stack>

#define FC_PERFORMDETECT_STOP(type) {CPerformDetector::GetInstance()->StopDetectNode(type);}
#define FC_PERFORMDETECT_START(type) {int eipValue=0; BEATS_ASSI_GET_EIP(eipValue);CPerformDetector::GetInstance()->StartDetectNode(type, eipValue);}
#define FC_PERFORMDETECT_RESET() {CPerformDetector::GetInstance()->ResetFrameResult();}
#define FC_PERFORMDETECT_SCOPE(type) CAutoPerformDetector _auto_detector_(type)

enum EPerformNodeType
{
    ePNT_Editor,
    ePNT_UpdateCamera,
    ePNT_Render,
    ePNT_RenderGrid,
    ePNT_RenderGrid_1,
    ePNT_RenderGrid_2,
    ePNT_RenderGrid_3,
    ePNT_RenderGrid_4,
    ePNT_ObjectRender,
    ePNT_ParticleSystemRender,
    ePNT_GUI,
    ePNT_3D,
    ePNT_2D,
    ePNT_GUIRender,
    ePNT_ContextFlush,
    ePNT_GroupRender,
    ePNT_Animation2D,

    ePNT_Count,
    ePNT_Force32Bit = 0xFFFFFFFF
};

static const TCHAR* pszPerformNodeStr[] = 
{
    _T("Editor"),
    _T("UpdateCamera"),
    _T("Render"),
    _T("RenderGrid"),
    _T("RenderGrid_1"),
    _T("RenderGrid_2"),
    _T("RenderGrid_3"),
    _T("RenderGrid_4"),
    _T("ObjectRender"),
    _T("ParticleSystemRender"),
    _T("GUI"),
    _T("3D"),
    _T("2D"),
    _T("GUIRender"),
    _T("ContextFlush"),
    _T("GroupRender"),
    _T("Animation2D")
};

struct SPerformanceResult
{
    size_t id;
    float result;
    LARGE_INTEGER startCount;
};

struct SPerformanceRecord
{
    int type;
    size_t id;//this will be an unique id for each record.
    size_t updateCount;
    float maxValue;
    float totalValue;
    SPerformanceRecord* pParent;
    const TCHAR* typeStr;
    std::vector<SPerformanceRecord*> children;

    SPerformanceRecord(size_t idParam = 0, int typeParam = 0)
        : id (idParam)
        , updateCount(0)
        , type(typeParam)
        , maxValue(0)
        , totalValue(0)
        , pParent(NULL)
        , typeStr(NULL)
    {

    }

    void Reset()
    {
        updateCount = 0;
        maxValue = 0;
        totalValue = 0;
        for (size_t i = 0; i < children.size(); ++i)
        {
            children[i]->Reset();
        }
    }
};

class CPerformDetector
{
    BEATS_DECLARE_SINGLETON(CPerformDetector);

public:
    void StartDetectNode(int type, size_t id);

    float StopDetectNode(int type);
    void ResetFrameResult();
    void ClearFrameResult();//Put this method at the end of the process.

    void SetTypeName(const TCHAR* typeStr[], size_t uCount);
    const TCHAR* GetTypeName(int type);

    SPerformanceRecord* GetRecord(size_t id);
    void GetResultThisFrame(std::vector<SPerformanceResult*>& outResult);

    bool PauseSwitcher();
    bool IsPaused();

    void ClearAllResult();

private:
    SPerformanceResult* GetResultFromPool();
    void IncreaseResultPool();
    void DestroyResultPool();
    void UpdateRecord(SPerformanceResult* pResult);

private:
    size_t m_resultPoolIndex;
    size_t m_lastResultPoolIndex;
    SPerformanceRecord* m_pCurRecord;
    bool m_bPause;
    bool m_bPauseRequest;    //We can only switch pause when a frame is end. Save the request.
    bool m_bClearAllRequest; //We can only clear result when a frame is end. Save the request.
    LARGE_INTEGER m_freq;

    std::vector<SPerformanceResult*> m_resultPool;//use a pool to avoid calling delete/new frequently.
    std::stack<SPerformanceResult*> m_resultOrder;
    std::vector<const TCHAR*> m_typeName;
    std::map<size_t, SPerformanceRecord*> m_recordMap;
    SPerformanceRecord m_rootRecord;
};

class CAutoPerformDetector
{
public:
    CAutoPerformDetector(int type)
        : m_type(type)
    {
        FC_PERFORMDETECT_START(m_type);
    }
    ~CAutoPerformDetector()
    {
        FC_PERFORMDETECT_STOP(m_type);
    }
private:
    int m_type;
};

#endif