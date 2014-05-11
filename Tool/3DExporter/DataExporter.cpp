#include "stdafx.h"
#include "DataExporter.h"
#include "Utility/BeatsUtility/Serializer.h"
#include "3DSceneExport.h"
#include "notetrck.h"
#include "Animatable.h"

#define  MAX_BONE_BLEND 4

inline static ESkeletonBoneType GetBoneType( const CHAR* pszNewName)
{
    ESkeletonBoneType ret = eSBT_Null;
    if (strcmp ( pszNewName, "Bip01 ͷ��")==0)  
        ret = eSBT_Head;
    else if (strcmp ( pszNewName, "Bip01 Neck")==0)  
        ret = eSBT_Neck;
    else if (strcmp ( pszNewName, "Bip01 R Clavicle")==0)  
        ret = eSBT_RightClavicle;
    else if (strcmp ( pszNewName, "Bip01 L Clavicle")==0)  
        ret = eSBT_LeftClavicle;
    else if (strcmp ( pszNewName, "Bip01 R UpperArm")==0)  
        ret = eSBT_RightUpperArm;
    else if (strcmp ( pszNewName, "Bip01 L UpperArm")==0)  
        ret = eSBT_LeftUpperArm;
    else if (strcmp ( pszNewName, "Bip01 R Forearm")==0)  
        ret = eSBT_RightForearm;
    else if (strcmp ( pszNewName, "Bip01 L Forearm")==0)  
        ret = eSBT_LeftForearm;
    else if (strcmp ( pszNewName, "Bip01 R Hand")==0)  
        ret = eSBT_RightHand;
    else if (strcmp ( pszNewName, "Bip01 L Hand")==0)  
        ret = eSBT_LeftHand;
    else if (strcmp ( pszNewName, "Bip01 R Finger0")==0)  
        ret = eSBT_RightFinger1;
    else if (strcmp ( pszNewName, "Bip01 R Finger1")==0)  
        ret = eSBT_RightFinger2;	
    else if (strcmp ( pszNewName, "Bip01 R Finger2")==0)  
        ret = eSBT_RightFinger3;
    else if (strcmp ( pszNewName, "Bip01 R Finger3")==0)  
        ret = eSBT_RightFinger4;
    else if (strcmp ( pszNewName, "Bip01 L Finger0")==0)  
        ret = eSBT_LeftFinger1;
    else if (strcmp ( pszNewName, "Bip01 L Finger1")==0)  
        ret = eSBT_LeftFinger2;
    else if (strcmp ( pszNewName, "Bip01 L Finger2")==0)  
        ret = eSBT_LeftFinger3;
    else if (strcmp ( pszNewName, "Bip01 L Finger3")==0)  
        ret = eSBT_LeftFinger4;	

    else if (strcmp ( pszNewName, "Bip01 R Finger01")==0)  
        ret = eSBT_RightFinger11;
    else if (strcmp ( pszNewName, "Bip01 R Finger11")==0)  
        ret = eSBT_RightFinger21;	
    else if (strcmp ( pszNewName, "Bip01 R Finger21")==0)  
        ret = eSBT_RightFinger31;
    else if (strcmp ( pszNewName, "Bip01 R Finger31")==0)  
        ret = eSBT_RightFinger41;
    else if (strcmp ( pszNewName, "Bip01 L Finger01")==0)  
        ret = eSBT_LeftFinger11;
    else if (strcmp ( pszNewName, "Bip01 L Finger11")==0)  
        ret = eSBT_LeftFinger21;
    else if (strcmp ( pszNewName, "Bip01 L Finger21")==0)  
        ret = eSBT_LeftFinger31;
    else if (strcmp ( pszNewName, "Bip01 L Finger31")==0)  
        ret = eSBT_LeftFinger41;	

    else if (strcmp ( pszNewName, "Bip01 R Finger02")==0)  
        ret = eSBT_RightFinger12;
    else if (strcmp ( pszNewName, "Bip01 R Finger12")==0)  
        ret = eSBT_RightFinger22;	
    else if (strcmp ( pszNewName, "Bip01 R Finger22")==0)  
        ret = eSBT_RightFinger32;
    else if (strcmp ( pszNewName, "Bip01 R Finger32")==0)  
        ret = eSBT_RightFinger42;
    else if (strcmp ( pszNewName, "Bip01 L Finger02")==0)  
        ret = eSBT_LeftFinger12;
    else if (strcmp ( pszNewName, "Bip01 L Finger12")==0)  
        ret = eSBT_LeftFinger22;
    else if (strcmp ( pszNewName, "Bip01 L Finger22")==0)  
        ret = eSBT_LeftFinger32;
    else if (strcmp ( pszNewName, "Bip01 L Finger32")==0)  
        ret = eSBT_LeftFinger42;	

    else if (strcmp ( pszNewName, "Bip01 R Finger0Nub")==0)  
        ret = eSBT_RightFinger13;
    else if (strcmp ( pszNewName, "Bip01 R Finger1Nub")==0)  
        ret = eSBT_RightFinger23;	
    else if (strcmp ( pszNewName, "Bip01 R Finger2Nub")==0)  
        ret = eSBT_RightFinger33;
    else if (strcmp ( pszNewName, "Bip01 R Finger3Nub")==0)  
        ret = eSBT_RightFinger43;
    else if (strcmp ( pszNewName, "Bip01 L Finger0Nub")==0)  
        ret = eSBT_LeftFinger13;
    else if (strcmp ( pszNewName, "Bip01 L Finger1Nub")==0)  
        ret = eSBT_LeftFinger23;
    else if (strcmp ( pszNewName, "Bip01 L Finger2Nub")==0)  
        ret = eSBT_LeftFinger33;
    else if (strcmp ( pszNewName, "Bip01 L Finger3Nub")==0)  
        ret = eSBT_LeftFinger43;	

    else if (strcmp ( pszNewName, "Bip01 Spine2")==0)  
        ret = eSBT_Spine2;
    else if (strcmp ( pszNewName, "Bip01 Spine1")==0)  
        ret = eSBT_Spine1;
    else if (strcmp ( pszNewName, "Bip01 Spine")==0)  
        ret = eSBT_Spine;
    else if (strcmp ( pszNewName, "Bip01 ����")==0)  
        ret = eSBT_Pelvis;
    else if (strcmp ( pszNewName, "Bip01")==0)  
        ret = eSBT_Root;
    else if (strcmp ( pszNewName, "Bip01 R Thigh")==0)  
        ret = eSBT_RightThigh;
    else if (strcmp ( pszNewName, "Bip01 L Thigh")==0)  
        ret = eSBT_LeftThigh;
    else if (strcmp ( pszNewName, "Bip01 R Calf")==0)  
        ret = eSBT_RightCalf;
    else if (strcmp ( pszNewName, "Bip01 L Calf")==0)  	
        ret = eSBT_LeftCalf;
    else if (strcmp ( pszNewName, "Bip01 R Foot")==0)  
        ret = eSBT_RightFoot;
    else if (strcmp ( pszNewName, "Bip01 L Foot")==0)  
        ret = eSBT_LeftFoot;
    else if (strcmp ( pszNewName, "Bip01 R Toe0")==0)  
        ret = eSBT_RightToe1;
    else if (strcmp ( pszNewName, "Bip01 L Toe0")==0)  
        ret = eSBT_LeftToe1;
    else if (strcmp ( pszNewName, "Bip01 R Toe01")==0)  
        ret = eSBT_RightToe2;
    else if (strcmp ( pszNewName, "Bip01 L Toe01")==0)  
        ret = eSBT_LeftToe2;
    else if (strcmp ( pszNewName, "Bip01 R Toe02")==0)  
        ret = eSBT_RightToe3;
    else if (strcmp ( pszNewName, "Bip01 L Toe02")==0)  
        ret = eSBT_LeftToe3;

    else if (strcmp ( pszNewName, "Bip01 R ForeTwist")==0)  
        ret = eSBT_RightForeTwist;
    else if (strcmp ( pszNewName, "Bip01 RUpArmTwist")==0)  
        ret = eSBT_RightUpArmTwist;
    else if (strcmp ( pszNewName, "Bip01 L ForeTwist")==0)  
        ret = eSBT_LeftForeTwist;
    else if (strcmp ( pszNewName, "Bip01 LUpArmTwist")==0)  
        ret = eSBT_LeftUpArmTwist;
    else if (strcmp ( pszNewName, "Bone04")==0)  
        ret = eSBT_Tooth;
    else if (strcmp ( pszNewName, "Bip01 L Toe0Nub")==0)  
        ret = eSBT_LeftToe5;
    else if (strcmp ( pszNewName, "Bip01 R Toe0Nub")==0)  
        ret = eSBT_RightToe5;
    else if (strcmp ( pszNewName, "Bone01")==0)  
        ret = eSBT_UserDefineBone;
    else if (strcmp ( pszNewName, "Bone02")==0)  
        ret = eSBT_UserDefineBone1;
    else if (strcmp ( pszNewName, "Bone03")==0)  
        ret = eSBT_UserDefineBone2;
    else if (strcmp ( pszNewName, "Bone01(mirrored)")==0)  
        ret = eSBT_UserDefineBone3;
    else if (strcmp ( pszNewName, "Bone02(mirrored)")==0)  
        ret = eSBT_UserDefineBone4;
    else if (strcmp ( pszNewName, "Bip01 ͷ��Nub")==0)  
        ret = eSBT_Count;

    BEATS_ASSERT(ret != eSBT_Null, _T("GetBoneType"));
    return ret;
}

inline static void GetMaterialCnt(IGameMaterial* pRootMat, UINT& uMatCnt)
{
    if(pRootMat == NULL)	
    {
        uMatCnt = 0;
        return;
    }

    if(!pRootMat->IsMultiType())
    {
        uMatCnt++;
    }
    else
    {
        INT nSubMatCnt = pRootMat->GetSubMaterialCount();
        for(INT x = 0; x<nSubMatCnt; x++)
        {
            IGameMaterial* pSubMat = pRootMat->GetSubMaterial(x);
            assert(pSubMat != NULL);
            GetMaterialCnt(pSubMat, uMatCnt);
        }
    }
}

inline static IGameMaterial*		GetMaterial(UINT& index, IGameMaterial* pRootMat, IGameMaterial* pParent, INT nChildID, IGameMaterial** pParentRet, INT* pChildIDRet)
{
    BEATS_ASSERT(pRootMat != NULL);
    BEATS_ASSERT(pParentRet != NULL);

    if(index == 0 && !pRootMat->IsMultiType())
    {
        *pParentRet = pParent;
        *pChildIDRet = nChildID;
        return pRootMat;
    }
    else if(index == 0 && pRootMat->IsMultiType())
    {
        INT nSubMatCnt = pRootMat->GetSubMaterialCount();

        for(INT x = 0; x < nSubMatCnt; x++)
        {
            IGameMaterial* pSubMat = pRootMat->GetSubMaterial(x);
            pSubMat = GetMaterial(index, pSubMat, pRootMat, x, pParentRet, pChildIDRet);
            if(pSubMat != NULL)
                return pSubMat;
        }
    }
    else if(index != 0 && !pRootMat->IsMultiType())
    {
        index--;

        INT nSubMatCnt = pRootMat->GetSubMaterialCount();
        for(INT x = 0; x<nSubMatCnt; x++)
        {
            IGameMaterial* pSubMat = pRootMat->GetSubMaterial(x);
            pSubMat = GetMaterial(index, pSubMat, pRootMat, x, pParentRet, pChildIDRet);
            if(pSubMat != NULL)
                return pSubMat;
        }
    }
    else
    {
        INT nSubMatCnt = pRootMat->GetSubMaterialCount();
        for(INT x = 0; x < nSubMatCnt; x++)
        {
            IGameMaterial* pSubMat = pRootMat->GetSubMaterial(x);
            pSubMat = GetMaterial(index, pSubMat, pRootMat, x, pParentRet, pChildIDRet);
            if(pSubMat != NULL)
                return pSubMat;
        }
    }

    return NULL;
}

CDataExporter::CDataExporter()
    : m_nBoneCount(0)
    ,m_nStartFrame(0)
    ,m_nEndFrame(0)
{
    m_pIGameScene = GetIGameInterface();
    m_pIGameScene ->InitialiseIGame();
}

CDataExporter::~CDataExporter()
{
    m_pIGameScene->ReleaseIGame();
}

void CDataExporter::InitNodeBones()
{
    UINT uTotalRootNodeCnt = m_pIGameScene->GetTopLevelNodeCount();
    for(UINT x = 0; x < uTotalRootNodeCnt; x++)
    {
        IGameNode* pNode = m_pIGameScene->GetTopLevelNode(x);
        this->CollectBoneNode(m_listBones, pNode);
    }
}

void CDataExporter::InitFrameInfo()
{
    TimeValue start_time = m_pIGameScene->GetSceneStartTime();
    TimeValue end_time = m_pIGameScene->GetSceneEndTime();
    INT nTickPerFrame = m_pIGameScene->GetSceneTicks();
    m_nStartFrame = start_time / nTickPerFrame;
    m_nEndFrame = end_time / nTickPerFrame;
    m_uFPS = TIME_TICKSPERSEC / nTickPerFrame;
}

void CDataExporter::ExportScene()
{
    InitNodeBones();
    InitFrameInfo();

    ExportSkeleton();
    ExportMesh();

    ExportAnimation();
}

void CDataExporter::ExportSkeleton()
{
    m_pIGameScene->SetStaticFrame(0);

    UINT nBoneCount = (UINT)m_listBones.Count();

    CSerializer serializer;
    serializer << nBoneCount;

    for(UINT x = 0; x < nBoneCount; x++)
    {
        IGameNode* pNode = m_listBones[x];
        assert(pNode != NULL);
        
        this->ExportSkeletonNode(pNode, serializer);
    }

    std::string  filename = m_strFileName;
    filename.append(_T(".ske"));
    serializer.Deserialize(filename.c_str());
}

void    CDataExporter::ExportMesh()
{
    GetSkinInfo();

    ExportSkinnedMesh();
}

BOOL CDataExporter::CollectBoneNode(Tab<IGameNode*>& res, IGameNode* pNode)
{
    IGameObject* pObject = pNode->GetIGameObject();

    IGameObject::ObjectTypes gameType = pObject->GetIGameType();
    IGameObject::MaxType maxType = pObject->GetMaxType();

    BOOL bBone = gameType == IGameObject::IGAME_BONE && maxType != IGameObject::IGAME_MAX_HELPER;

    pNode->ReleaseIGameObject();

    if (bBone)
    {
        res.Append(1, &pNode);
    }

    INT nChildCnt = pNode->GetChildCount();
    for(INT x = 0; x < nChildCnt; x++)
    {
        IGameNode* pChild = pNode->GetNodeChild(x);
        BEATS_ASSERT(pChild != NULL);
        this->CollectBoneNode(res, pChild);
    }

    return TRUE;
}

BOOL CDataExporter::ExportSkeletonNode(IGameNode* pNode, CSerializer& serializer)
{
#ifdef _DEBUG
    IGameObject* pObject = pNode->GetIGameObject();
    IGameObject::ObjectTypes gameType = pObject->GetIGameType();
    IGameObject::MaxType maxType = pObject->GetMaxType();

    BEATS_ASSERT(gameType == IGameObject::IGAME_BONE && maxType != IGameObject::IGAME_MAX_HELPER, _T("Error at exporting a bone!"));

#endif

    //BoneType
    ESkeletonBoneType type = GetBoneType(pNode->GetName());
    ESkeletonBoneType parentType = eSBT_Null;
    IGameNode* pParentNode = pNode->GetNodeParent();
    if ( pParentNode )
    {
        parentType = GetBoneType(pParentNode->GetName());
    }
    serializer << type << parentType;

    GMatrix matParent;
    GMatrix matRelative;
    GMatrix matWorld = pNode->GetWorldTM();

    if(pParentNode)
    {
        matParent = pParentNode->GetWorldTM();
        if(matParent == matWorld)
        {
            matRelative.SetIdentity();
        }
        else
        {
             matRelative = matWorld * matParent.Inverse();
        }
    }
    else
    {
        matRelative = matWorld;
    }

    for(size_t i = 0; i < 4; i++)
    {
        Point4 pt = matRelative.GetRow(i);
        serializer << pt.x << pt.y << pt.z << pt.w;
    }

    return TRUE;
}

BOOL CDataExporter::ExportAnimationNode(IGameNode* pNode, CSerializer& serializer)
{
#ifdef _DEBUG
	IGameObject* pObject = pNode->GetIGameObject();
	IGameObject::ObjectTypes gameType = pObject->GetIGameType();
	IGameObject::MaxType maxType = pObject->GetMaxType();

	BOOL bBone = gameType == IGameObject::IGAME_BONE && maxType != IGameObject::IGAME_MAX_HELPER;
	BEATS_ASSERT(bBone, _T("Error at exporting a bone! ExportAnimationNode"));
#endif // _DEBUG

    GMatrix matParent;
    GMatrix matRelative;
    GMatrix matWorld = pNode->GetWorldTM();
    IGameNode* pParentNode = pNode->GetNodeParent();
    if(pParentNode)
    {
        matParent = pParentNode->GetWorldTM();
        if(matParent == matWorld)
        {
            matRelative.SetIdentity();
        }
        else
        {
            matRelative = matWorld * matParent.Inverse();
        }
    }
    else
    {
        matRelative = matWorld;
    }

    for(size_t i = 0; i < 4; i++)
    {
        Point4 pt = matRelative.GetRow(i);
        serializer << pt.x << pt.y << pt.z << pt.w;
    }

    return TRUE;
}

bool CDataExporter::FilterVertexData(const Point3& ptPos, const Point3& ptUv)
{
    bool bRet = true;
    int iSizePos = m_vecPos.size();
    int iIndex = 0;
    for(iIndex = 0; iIndex < iSizePos; iIndex++)
    {
        if(ptPos == m_vecPos[iIndex] && ptUv == m_vecUV[iIndex])
        {
            break;
        }
    }

    if(iIndex == iSizePos)
    {
        m_vecUV.push_back(ptUv);
        m_vecPos.push_back(ptPos);
        bRet = false;
    }
    
    m_posIndexVector.push_back(iIndex);

    return bRet;
}

void CDataExporter::GetSkinInfo()
{
    m_vecPos.clear();
    m_vecUV.clear();
    m_vecWeightData.clear();
    m_vMeshVCount.clear();
    m_posIndexVector.clear();
    m_uMeshCount = 0;

    UINT uTotalRootNodeCnt = m_pIGameScene->GetTopLevelNodeCount();
    for(UINT x = 0; x < uTotalRootNodeCnt; x++)
    {
        IGameNode* pNode = m_pIGameScene->GetTopLevelNode(x);
        IGameObject* pObject = pNode->GetIGameObject();

        IGameObject::ObjectTypes gameType = pObject->GetIGameType();

        if(gameType == IGameObject::IGAME_MESH)
        {
            m_uMeshCount++;
            GetMaterialInfo(pNode);

            IGameMesh* pMesh = (IGameMesh*)pNode->GetIGameObject();
            IGameSkin* pSkin = pMesh->GetIGameSkin();

            pMesh->SetUseWeightedNormals();
            pMesh->InitializeData();

            Tab<int> mapNums = pMesh->GetActiveMapChannelNum();
            BEATS_ASSERT( mapNums.Count() == 1);
            int uFaceCount = pMesh->GetNumberOfFaces();
            
            int iVertexCount = 0;

            for(int i = 0; i < uFaceCount; ++i)
            {
                FaceEx* pFace = pMesh->GetFace(i);
                for(int  j = 0; j < 3; ++j)
                {
                    DWORD mapIndex[3];
                    Point3 ptUV;
                    int indexUV = pFace->texCoord[j];
                    if(pMesh->GetMapFaceIndex(mapNums[0], pFace->meshFaceIndex, mapIndex))
                        ptUV = pMesh->GetMapVertex(mapNums[0], mapIndex[j]);
                    else
                        ptUV = pMesh->GetMapVertex(mapNums[0], indexUV);
                   
                    int indexPos = pFace->vert[j];
                    Point3 pos = pMesh->GetVertex(indexPos);

                    if(false == FilterVertexData(pos, ptUV))
                    {
                        iVertexCount++;

                        int uEffectedBones = pSkin->GetNumberOfBones(indexPos);
                        float weightSum = 0;

                        std::map<ESkeletonBoneType, SWeightData> sortCache;
                        for (int j = 0; j < uEffectedBones; ++j)
                        {
                            INode* pBone = pSkin->GetBone(indexPos, j);
                            const char* pszBoneName = pBone->GetName();

                            ESkeletonBoneType boneType = GetBoneType(pszBoneName);

                            float fWeight = pSkin->GetWeight(indexPos, j);

                            if (!BEATS_FLOAT_EQUAL(fWeight, 0))
                            {
                                SWeightData data;
                                data.type = boneType;
                                data.fWeight = fWeight;
                                sortCache[boneType] = data;
                                weightSum += fWeight;
                            }
                        }
                        int counter = 0;
                        for (std::map<ESkeletonBoneType, SWeightData>::iterator iter = sortCache.begin(); iter != sortCache.end(); ++iter)
                        {
                            m_vecWeightData.push_back(iter->second);
                            if (++counter == MAX_BONE_BLEND)
                            {
                                break;
                            }
                        }
                        for (int k = counter; k < MAX_BONE_BLEND; ++k)
                        {
                            SWeightData weightdata;
                            weightdata.type = eSBT_Null;
                            weightdata.fWeight = 0;
                            m_vecWeightData.push_back(weightdata);
                        }

                        BEATS_ASSERT(weightSum > 0.98f && weightSum < 1.01f);
                    }
                }
            }
            m_MeshPosIndexCnt.push_back(uFaceCount*3);
            m_vMeshVCount.push_back(iVertexCount);
        }
        pNode->ReleaseIGameObject();
    }
}

void CDataExporter::ExportSkinnedMesh()
{
    CSerializer serializer;

    serializer << m_uMeshCount;

    int iStartMat = 0;
    int iEndMat = 0;

    int startIndexVertex = 0;
    int endIndexVertex = 0;

    int startPosIndex = 0;
    int endPosIndex = 0;
    for(size_t i = 0; i < m_uMeshCount; ++i)
    {
        //Meterial
        int iMeshMaterialCnt = m_vMeshMeterialCnt[i];
        serializer << iMeshMaterialCnt;
       
        iEndMat  += iMeshMaterialCnt;
        for(; iStartMat < iEndMat; ++iStartMat)
        {
            serializer << m_vMaterialName[iStartMat];
        }
        iStartMat = iEndMat;
       
        //vertex data
        int iMeshVertexCount = m_vMeshVCount[i];
        serializer << iMeshVertexCount;
           
        endIndexVertex += iMeshVertexCount;
        for(; startIndexVertex < endIndexVertex; ++startIndexVertex)
        {
            serializer << m_vecPos[startIndexVertex].x << m_vecPos[startIndexVertex].y << m_vecPos[startIndexVertex].z;
            serializer << m_vecUV[startIndexVertex].x << m_vecUV[startIndexVertex].y ;

            for(int j = 0; j < MAX_BONE_BLEND; ++j)
            {
                SWeightData weightdata = m_vecWeightData[startIndexVertex*MAX_BONE_BLEND+j];
                serializer << weightdata.type << weightdata.fWeight;
            }
        }
        startIndexVertex = endIndexVertex; 

        //PosIndex
        int iMeshPosIndexCount = m_MeshPosIndexCnt[i];
        serializer << iMeshPosIndexCount;
        endPosIndex += iMeshPosIndexCount;
        for(; startPosIndex < endPosIndex; ++startPosIndex)
        {
            serializer << m_posIndexVector[startPosIndex];
        }
        startPosIndex =  endPosIndex;
    }

    std::string  filename = m_strFileName;
    filename.append(_T(".skin"));
    serializer.Deserialize(filename.c_str());
  
}

void CDataExporter::ExportAnimation()
{
    InitAminiationSegment();

    size_t nBoneCount = (size_t)m_listBones.Count();
    if(nBoneCount > 0)
    {
        CSerializer serializer;
        int nFrameCount = m_nEndFrame - m_nStartFrame;
        if(m_vKeys.size() > 1)
        {
            size_t keySize = m_vKeys.size();
            int _nStartFrame = m_nStartFrame;
            for ( size_t i = 0 ; i < keySize; ++i )
            {
                int _nEndFrame = m_vKeys[i].m_nKeyTime;
                nFrameCount = _nEndFrame - _nStartFrame+1;
                serializer << nBoneCount << m_uFPS << nFrameCount;
                for(size_t x = 0; x < nBoneCount; x++)
                {
                    IGameNode* pNode = m_listBones[x];
                    ESkeletonBoneType type = GetBoneType(pNode->GetName());
                    serializer << type;
                }
                for(int frame = _nStartFrame; frame <= _nEndFrame; ++frame)
                {
                    m_pIGameScene->SetStaticFrame(frame);
                    for(size_t x = 0; x < nBoneCount; x++)
                    {
                        IGameNode* pNode = m_listBones[x];
                        BEATS_ASSERT(pNode != NULL);
                        this->ExportAnimationNode(pNode, serializer);
                    }
                }
                _nStartFrame = _nEndFrame;
                std::string  name = m_strFileName;
                std::string keyname = m_vKeys[i].m_strKeyName;
                keyname.erase(keyname.find_last_not_of("\n\r")+1);
                name.append(keyname);
                name.append(_T(".ani"));
                serializer.Deserialize(name.c_str());

            }	
        }
        else
        {
            serializer << nBoneCount << m_uFPS << nFrameCount;
            for(size_t x = 0; x < nBoneCount; x++)
            {
                IGameNode* pNode = m_listBones[x];
                ESkeletonBoneType type = GetBoneType(pNode->GetName());
                serializer << type;
            }
            for(int frame = m_nStartFrame; frame <= m_nEndFrame; frame++)
            {
                m_pIGameScene->SetStaticFrame(frame);
                for(size_t x = 0; x < nBoneCount; x++)
                {
                    IGameNode* pNode = m_listBones[x];
                    BEATS_ASSERT(pNode != NULL);
                    this->ExportAnimationNode(pNode, serializer);
                }
            }
            std::string  filename = m_strFileName;
            filename.append(_T(".ani"));
            serializer.Deserialize(filename.c_str());
        }
    }

}

void CDataExporter::InitAminiationSegment()
{
    BEATS_ASSERT(m_pIGameScene->GetTopLevelNodeCount() > 0);

    IGameNode* pGameNode = m_pIGameScene->GetTopLevelNode(0);
    INode* pNode = pGameNode->GetMaxNode()->GetParentNode();
   
    int numNoteTracks = pNode->NumNoteTracks();
   
    INT nTickPerFrame = m_pIGameScene->GetSceneTicks();
    for(int i = 0; i < numNoteTracks; i++)
    {
        DefNoteTrack* nt = (DefNoteTrack*)pNode->GetNoteTrack(i);
        for (int j = 0; j < nt->keys.Count(); j++)
        { 
           stKeyValue keyValue;
           keyValue.m_nKeyTime  = ( nt->GetKeyTime(j) / nTickPerFrame );
           keyValue.m_strKeyName =  nt->keys[j]->note;
           m_vKeys.push_back( keyValue );
        }
    }

}

void    CDataExporter::SetExportFileName(std::string fileName)
{
    m_strFileName = fileName;
    
    std::string  name = m_strFileName;
    std::string::size_type ix = m_strFileName.find_last_of('\\'); 
    if(ix != std::string::npos)
    {
        m_strFilePath = m_strFileName.substr(0,ix);
    }
}

void    CDataExporter::GetMaterialInfo(IGameNode* pNode)
{
    IGameMaterial* pRootMat = pNode->GetNodeMaterial();
    if(pRootMat != NULL)
    {
        UINT uMatCnt = 0;
        GetMaterialCnt(pRootMat, uMatCnt);
        m_vMeshMeterialCnt.push_back(uMatCnt);
        for(UINT x = 0; x < uMatCnt; x++)
        {
            UINT index = x;
            INT nChildID;
            IGameMaterial* pParentMat;
            IGameMaterial* pMat = GetMaterial(index, pRootMat, NULL, -1, &pParentMat, &nChildID);

            TCHAR* pszMatName = pMat->GetMaterialName();
            std::string strMatName = pszMatName;
            m_vMaterialName.push_back(strMatName);
        }
    }
}

