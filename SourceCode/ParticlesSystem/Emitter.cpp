#include "stdafx.h"
#include "Emitter.h"
#include "Render/Renderer.h"
#include "Render/RenderManager.h"
#include "Resource/ResourceManager.h"
#include "Render/Shader.h"
#include "render/camera.h"


namespace FCEngine
{
    CParticleEmitter::CParticleEmitter()
    {
        m_pVertData = 0;
        m_uCurrentCount = 0;
        m_fBeginTime = -1;
        m_fMinPreShootTime = -1;
        m_fDeltaTime = 0;
        m_pIndexBuffer = 0;
        m_bInit = false;

        m_uTotalCount = 10;
        m_pMaterial = NULL;
        kmVec3Fill( &m_vec3Origin, 0.0, 0.0, 0.0 );
        kmVec3Fill( &m_vec3Direction, 0.0, 1.0, 0.0 );
        kmVec3Fill( &m_vec3DirectionOffset, 0.0, 0.0, 0.0 );
        m_uRatePerSecond = 5;
        m_fMinParticleSpeed = 1.0;
        m_fMaxParticleSpeed = 3.0;
        m_fMinSpeedDecay = 0.0;
        m_fMaxSpeedDecay = 0.0;
        m_fForcePower = 0.0;
        kmVec3Fill( &m_vec3ForceDirection, 0.0, -1.0, 0.0 );
        m_fMinLiveTime = 3.0;
        m_fMaxLiveTime = 5.0;
        m_fDelayTime = 0.0;
        m_bLocalCoord = true;
        m_bIsLoop = true;

        SharePtr<CShader> pVS = CResourceManager::GetInstance()->GetResource<CShader>(_T("particle.vs"), false);
        SharePtr<CShader> pPS = CResourceManager::GetInstance()->GetResource<CShader>(_T("particle.ps"), false);
        m_pParticleProgram = CRenderManager::GetInstance()->GetShaderProgram(pVS->ID(), pPS->ID());

        m_VAO = 0;

        m_pMaterial = NULL;
        m_pEmitterBase = NULL;
        m_pParticleProperty = NULL;
    }

    CParticleEmitter::~CParticleEmitter()
    {
        for ( auto iter : m_aliveParticleList )
        {
           BEATS_SAFE_DELETE( iter );
        }

        for ( auto iter : m_particleRecycle )
        {
           BEATS_SAFE_DELETE( iter );
        }

        CRenderer* pRenderer = CRenderer::GetInstance();
        if ( 0 != m_VAO )
        {
            pRenderer->DeleteBuffers(2, m_VBO);
            pRenderer->DeleteBuffers(1, &m_VAO);
        }

        BEATS_SAFE_DELETE_ARRAY( m_pVertData );
        BEATS_SAFE_DELETE_ARRAY( m_pIndexBuffer );
    }

    void CParticleEmitter::ReflectData(CSerializer& serializer)
    {
        CComponentBase::ReflectData(serializer);
        DECLARE_PROPERTY( serializer, m_uTotalCount, true, 0xFFFFFFFF, _T( "发射粒子总数" ), NULL, NULL, NULL );
        DECLARE_PROPERTY( serializer, m_vec3Origin, true, 0xFFFFFFFF, _T( "发射器原点坐标" ), NULL, NULL, NULL );
        DECLARE_PROPERTY( serializer, m_vec3Direction, true, 0xFFFFFFFF, _T( "发射方向" ), NULL, NULL, NULL );
        DECLARE_PROPERTY( serializer, m_vec3DirectionOffset, true, 0xFFFFFFFF, _T( "发射角度" ), NULL, NULL, NULL );
        DECLARE_PROPERTY( serializer, m_uRatePerSecond, true, 0xFFFFFFFF, _T( "每秒发射粒子个数" ), NULL, NULL, NULL );
        DECLARE_PROPERTY( serializer, m_fMinParticleSpeed, true, 0xFFFFFFFF, _T( "最小运动速度" ), _T( "粒子运动速度" ), NULL, NULL );
        DECLARE_PROPERTY( serializer, m_fMaxParticleSpeed, true, 0xFFFFFFFF, _T( "最大运动速度" ), _T( "粒子运动速度" ), NULL, NULL );
        DECLARE_PROPERTY( serializer, m_fMinSpeedDecay, true, 0xFFFFFFFF, _T( "最小衰减速度" ), _T( "粒子运动速度衰减" ), NULL, NULL );
        DECLARE_PROPERTY( serializer, m_fMaxSpeedDecay, true, 0xFFFFFFFF, _T( "最大衰减速度" ), _T( "粒子运动速度衰减" ), NULL, NULL );
        DECLARE_PROPERTY( serializer, m_fForcePower, true, 0xFFFFFFFF, _T( "外力大小" ), _T( "外力" ), NULL, NULL );
        DECLARE_PROPERTY( serializer, m_vec3ForceDirection, true, 0xFFFFFFFF, _T( "外力方向" ), _T( "外力" ), NULL, NULL );
        DECLARE_PROPERTY( serializer, m_fMinLiveTime, true, 0xFFFFFFFF, _T( "最小生存时间" ), _T( "粒子运动生存周期" ), _T( "单位(s)" ), NULL );
        DECLARE_PROPERTY( serializer, m_fMaxLiveTime, true, 0xFFFFFFFF, _T( "最小生存时间" ), _T( "粒子运动生存周期" ), _T( "单位(s)" ), NULL );
        DECLARE_PROPERTY( serializer, m_fDelayTime, true, 0xFFFFFFFF, _T( "延迟发射时间" ), NULL, _T( "单位(s)" ), NULL );
        DECLARE_PROPERTY( serializer, m_bLocalCoord, true, 0xFFFFFFFF, _T( "是否约束粒子" ), NULL, NULL, NULL );
        DECLARE_PROPERTY( serializer, m_bIsLoop, true, 0xFFFFFFFF, _T( "是否循环播放" ), NULL, NULL, NULL );
        DECLARE_DEPENDENCY( serializer, m_pMaterial,_T( "材质" ), eDT_Weak );
        DECLARE_DEPENDENCY( serializer, m_pEmitterBase, _T( "发射器类型" ), eDT_Strong );
        DECLARE_DEPENDENCY( serializer, m_pParticleProperty, _T( "粒子属性" ), eDT_Strong );

        InitVAOAndVBO( m_uTotalCount );
        SetShootVelocity( m_uRatePerSecond );
    }

    void CParticleEmitter::BeginShoot()
    {
        m_fBeginTime = 0;
    }

    void CParticleEmitter::Update( float dtt )
    {
        if ( NULL != m_pEmitterBase && NULL != m_pParticleProperty )
        {
            UpdateActiveParticles( dtt );

            if ( m_fDelayTime > 0 && m_fBeginTime < m_fDelayTime )//delay emit
            {
                m_fBeginTime += dtt;
                if ( m_fBeginTime < m_fDelayTime )
                {
                    return;
                }
            }
            if ( m_uCurrentCount < m_uTotalCount || m_bIsLoop )
            {
                //To calculation how many particles will be shoot in current frame
                //check the time is longer than the minimum emit time
                if ( m_fDeltaTime < m_fMinPreShootTime )
                {
                    m_fDeltaTime += dtt;
                }
                float pCount = m_fDeltaTime * m_uRatePerSecond + 0.5f;
                ShootParicle( (int)pCount );
                if ( pCount > 1.0 )//the emitter is begin emit,set the delta time be zero
                {
                    m_fDeltaTime = 0;
                }
            }
        }
    }

    void CParticleEmitter::Render()
    {
        if ( NULL == m_pEmitterBase || NULL == m_pParticleProperty )
        {
            return;
        }
        if ( NULL != m_pVertData && NULL != m_pIndexBuffer && !m_aliveParticleList.empty())
        {
            unsigned int index = 0;
            TParticleList::iterator it = m_aliveParticleList.begin();
            //clear buffer before per frame
            memset( m_pVertData, 0, sizeof( CVertexPTC ) * m_uTotalCount * 4 );
            memset( m_pIndexBuffer, 0, sizeof( unsigned short ) * m_uTotalCount * 6 );
            for ( ; it != m_aliveParticleList.end(); )
            {
                CParticleEntity* pParticle = *it;
                BEATS_ASSERT(index < m_uTotalCount * 4, _T("index > m_uTotalCount * 4 !"));
                if ( index < m_uTotalCount * 4 )
                {
                    pParticle->ComputeVertices( &m_pVertData ,&m_pIndexBuffer, index );
                }
                if ( !pParticle->IsAlive() )
                {
                    m_particleRecycle.push_back( *it );
                    it = m_aliveParticleList.erase( it );
                }
                else
                {
                    ++it;
                }
            }

            //render
            CRenderer* pRenderer = CRenderer::GetInstance();

            pRenderer->BindBuffer( GL_ARRAY_BUFFER, m_VBO[ 0 ] ); 
            pRenderer->BufferData( GL_ARRAY_BUFFER, sizeof( CVertexPTC ) * m_uCurrentCount * 4 , m_pVertData, GL_DYNAMIC_DRAW ); 
            FC_CHECK_GL_ERROR_DEBUG( );

            pRenderer->BindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_VBO[ 1 ] ); 
            pRenderer->BufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned short ) * m_uCurrentCount * 6 , m_pIndexBuffer , GL_DYNAMIC_DRAW );
            FC_CHECK_GL_ERROR_DEBUG( );

            if( m_pMaterial )
            {
                m_pMaterial->Use();
            }

            pRenderer->UseProgram( m_pParticleProgram->ID() );
            
            kmMat4 viewMatrix;
            CRenderManager::GetInstance()->GetCamera()->GetViewMatrix( viewMatrix );
            kmMat4 translateMatrix;
            kmMat4Translation( &translateMatrix, -viewMatrix.mat[ 12 ], -viewMatrix.mat[ 13 ], -viewMatrix.mat[ 14 ] );
            kmMat4Multiply( &viewMatrix, &translateMatrix, &viewMatrix );
            kmMat4Inverse( &viewMatrix, &viewMatrix );
            GLint MVLocation = CRenderer::GetInstance()->GetUniformLocation( m_pParticleProgram->ID(), COMMON_UNIFORM_NAMES[ UNIFORM_MV_MATRIX ]);
            CRenderer::GetInstance()->SetUniformMatrix4fv(MVLocation, (const float*)viewMatrix.mat, 1);
            FC_CHECK_GL_ERROR_DEBUG();

            pRenderer->BindVertexArray( m_VAO );
            FC_CHECK_GL_ERROR_DEBUG( );
            pRenderer->DrawElements( GL_TRIANGLES, m_uCurrentCount * 6 , GL_UNSIGNED_SHORT, 0 );
            FC_CHECK_GL_ERROR_DEBUG( );
            pRenderer->BindVertexArray( 0 );
            pRenderer->UseProgram( 0 );
        }
    }

    void CParticleEmitter::ShootParicle( unsigned int count )
    {
        BEATS_ASSERT( NULL != m_pEmitterBase , _T( "The emitter type can't be sure!" ) );
        m_uCurrentCount += count;
        unsigned int pCreateCount = count;
        unsigned int pReUseCount = 0;
        if ( m_uCurrentCount >= m_uTotalCount )//check the particles number is bigger than the total number
        {
            pReUseCount = m_uCurrentCount - m_uTotalCount;
            pCreateCount -= pReUseCount;
            m_uCurrentCount = m_uTotalCount;
            if ( !m_bIsLoop )
            {
                pReUseCount = 0;
            }
            else
            {
                m_fBeginTime = 0;
            }
        }

        //create a new particle
        for ( unsigned int i = 0; i < pCreateCount; ++i )
        {
           CParticleEntity* pParticle = new CParticleEntity( );
           kmVec3 birthPosition;
           kmVec3Fill( &birthPosition, m_pEmitterBase->GetBirthPositon( ).x,
               m_pEmitterBase->GetBirthPositon( ).y, m_pEmitterBase->GetBirthPositon( ).z );
           kmVec3Add( &birthPosition, &m_vec3Origin, &birthPosition );
           pParticle->SetOriginPosition( birthPosition );
           SetParticleInitProperty( &pParticle );
           m_aliveParticleList.push_back( pParticle );
        }

        //get the particle from the recycle
        for ( unsigned int i = 0; i < pReUseCount; ++i )
        {
            CParticleEntity* pParticle = GetParticleFromRecycle( );
            if ( pParticle )
            { 
                pParticle->Activate();//active the died particle
                kmVec3 birthPosition;
                kmVec3Fill( &birthPosition, m_pEmitterBase->GetBirthPositon( ).x,
                    m_pEmitterBase->GetBirthPositon( ).y, m_pEmitterBase->GetBirthPositon( ).z );
                kmVec3Add( &birthPosition, &m_vec3Origin, &birthPosition );
                pParticle->SetOriginPosition( birthPosition );
                SetParticleInitProperty( &pParticle );
                m_aliveParticleList.push_back( pParticle );
            }
        }
    }


    void CParticleEmitter::SetTotalCount( unsigned int count )
    {
        m_uTotalCount = count;
        InitVAOAndVBO( count );
    }

    unsigned int CParticleEmitter::GetTotleCount()
    {
        return m_uTotalCount;
    }


    void CParticleEmitter::SetShootDeriction( const kmVec3& deriction )
    {
        kmVec3Fill( &m_vec3Direction, deriction.x, deriction.y, deriction.z );
    }

    const kmVec3& CParticleEmitter::GetShootDeriction() const
    {
        return m_vec3Direction;
    }

    void CParticleEmitter::SetShootOrigin( const kmVec3& origin )
    {
        kmVec3Fill( &m_vec3Origin, origin.x, origin.y, origin.z );
    }

    const kmVec3& CParticleEmitter::GetShootOrigin() const
    {
        return m_vec3Origin;
    }

    void CParticleEmitter::SetShootDegree( const kmVec3& degree )
    {
         kmVec3Fill( &m_vec3DirectionOffset, degree.x, degree.y, degree.z );
    }

    const kmVec3& CParticleEmitter::GetShootDegree() const
    {
        return m_vec3DirectionOffset;
    }

    void CParticleEmitter::SetShootVelocity( unsigned int velocity )
    {
        m_uRatePerSecond = velocity;
        m_fMinPreShootTime = 1.0f / velocity;
    }

    unsigned int CParticleEmitter::GetShootVelocity()
    {
        return m_uRatePerSecond;
    }

    void CParticleEmitter::SetMinParticleVelocity( float velocity )
    {
        m_fMinParticleSpeed = velocity;
    }

    float CParticleEmitter::GetMinParticleVelocity()
    {
        return m_fMinParticleSpeed;
    }

    void CParticleEmitter::SetMaxParticleVelocity( float velocity )
    {
        m_fMaxParticleSpeed = velocity;
    }

    float CParticleEmitter::GetMaxParticleVelocity()
    {
        return m_fMaxParticleSpeed;
    }

    void CParticleEmitter::SetMinParticleVelocityDecay( float decay )
    {
        m_fMinSpeedDecay = decay;
    }

    float CParticleEmitter::GetMinParticleVelocityDecay()
    {
        return m_fMinSpeedDecay;
    }

    void CParticleEmitter::SetMaxParticleVelocityDecay( float decay )
    {
        m_fMaxSpeedDecay = decay;
    }

    float CParticleEmitter::GetMaxParticleVelocityDecay()
    {
        return m_fMaxSpeedDecay;
    }

    void CParticleEmitter::SetForcePower( float power )
    {
        m_fForcePower = power;
    }

    float CParticleEmitter::GetForcePower()
    {
        return m_fForcePower;
    }

    void CParticleEmitter::SetFroceDeriction( const kmVec3& deriction )
    {
        kmVec3Fill( &m_vec3ForceDirection, deriction.x, deriction.y, deriction.z );
    }

    const kmVec3& CParticleEmitter::GetFroceDeriction() const
    {
        return m_vec3ForceDirection;
    }

    void CParticleEmitter::SetMinLiveTime( float time )
    {
        m_fMinLiveTime = time;
    }

    float CParticleEmitter::GetMinLiveTime()
    {
        return m_fMinLiveTime;
    }

    void CParticleEmitter::SetMaxLiveTime( float time )
    {
        m_fMaxLiveTime = time;
    }

    float CParticleEmitter::GetMaxLiveTime()
    {
        return m_fMaxLiveTime;
    }

    void CParticleEmitter::SetDelayTime( float time )
    {
        m_fDelayTime = time;
    }

    float CParticleEmitter::GetDelayTime()
    {
        return m_fDelayTime;
    }

    void CParticleEmitter::SetParticleConstraint( bool bConstraint )
    {
        m_bLocalCoord = bConstraint;
    }

    bool CParticleEmitter::GetParticleConstraint()
    {
        return m_bLocalCoord;
    }

    void CParticleEmitter::UpdateActiveParticles( float dtt )
    {
        for ( auto i : m_aliveParticleList )
        {
            CParticleEntity* pParticle = i;
            pParticle->Update( dtt );
        }
    }

    void CParticleEmitter::SetParticleInitProperty( CParticleEntity** pParticleEntity )
    {
        CParticleEntity* pParticle = *pParticleEntity;
        pParticle->SetEmitter( this );
        pParticle->SetForcePower( m_fForcePower );
        pParticle->SetForceDirection( m_vec3ForceDirection );

        //direction
        kmVec3 tempDegree;
        kmVec3Fill( &tempDegree, 0.0, 0.0, 0.0 );
        if ( m_vec3DirectionOffset.x != 0 )
        {
            tempDegree.x = RANGR_RANDOM_FLOAT( 0, m_vec3DirectionOffset.x );
            tempDegree.x -= m_vec3DirectionOffset.x * 0.5f;
        }
        if ( m_vec3DirectionOffset.y != 0 )
        {
            tempDegree.y = RANGR_RANDOM_FLOAT( 0, m_vec3DirectionOffset.y );
            tempDegree.y -= m_vec3DirectionOffset.y * 0.5f;
        }
        if ( m_vec3DirectionOffset.z != 0 )
        {
            tempDegree.z = RANGR_RANDOM_FLOAT( 0, m_vec3DirectionOffset.z );
            tempDegree.z -= m_vec3DirectionOffset.z * 0.5f;
        }
        
        kmMat4 rotateMat;
        kmMat4Identity( &rotateMat );
        kmMat4RotationPitchYawRoll( &rotateMat, tempDegree.x / MATH_PI_DEGREE * MATH_PI, tempDegree.y / MATH_PI_DEGREE * MATH_PI, tempDegree.z / MATH_PI_DEGREE * MATH_PI );
        kmVec3 pDeriction;
        kmVec3Transform( &pDeriction, &m_vec3Direction, &rotateMat );

        pParticle->SetOriginDirection( pDeriction );
        float tempNum = RANGR_RANDOM_FLOAT( m_fMinParticleSpeed, m_fMaxParticleSpeed );
        pParticle->SetSpeed( tempNum );
        tempNum = RANGR_RANDOM_FLOAT( m_fMinSpeedDecay, m_fMaxSpeedDecay );
        pParticle->SetDecay( tempNum );
        tempNum = RANGR_RANDOM_FLOAT( m_fMinLiveTime, m_fMaxLiveTime );
        pParticle->SetLiveTime( tempNum );
    }

    CParticleEntity* CParticleEmitter::GetParticleFromRecycle()
    {
        CParticleEntity* pParticle = 0;
        if ( !m_particleRecycle.empty() )
        {
            pParticle =  *m_particleRecycle.begin();
            m_particleRecycle.pop_front();
        }
        return pParticle;
    }

    void CParticleEmitter::SetPlayLoop( bool bLoop )
    {
        m_bIsLoop = bLoop;
    }

    bool CParticleEmitter::GetPlayLoop()
    {
        return m_bIsLoop;
    }

    void CParticleEmitter::InitVAOAndVBO( int count )
    {
        CRenderer* pRenderer = CRenderer::GetInstance();
        
        BEATS_SAFE_DELETE_ARRAY( m_pVertData );
        BEATS_SAFE_DELETE_ARRAY( m_pIndexBuffer );

        m_pVertData = new CVertexPTC[ count * 4 ];
        m_pIndexBuffer = new unsigned short[ count * 6 ];

        memset( m_pVertData, 0, sizeof( CVertexPTC ) * count * 4 );
        memset( m_pIndexBuffer, 0, sizeof( unsigned short ) * count * 6 );
        if ( !m_bInit )
        {
            pRenderer->GenVertexArrays( 1, &m_VAO );
            pRenderer->BindVertexArray( m_VAO ); 
            pRenderer->GenBuffers( 2, m_VBO ); 
            pRenderer->BindBuffer( GL_ARRAY_BUFFER, m_VBO[ 0 ] );

            pRenderer->EnableVertexAttribArray( ATTRIB_INDEX_POSITION );
            pRenderer->EnableVertexAttribArray( ATTRIB_INDEX_COLOR );
            pRenderer->EnableVertexAttribArray( ATTRIB_INDEX_TEXCOORD0 );

            pRenderer->VertexAttribPointer( ATTRIB_INDEX_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(CVertexPTC), (const GLvoid *)offsetof(CVertexPTC, position) ); 
            pRenderer->VertexAttribPointer( ATTRIB_INDEX_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(CVertexPTC), (const GLvoid *)offsetof(CVertexPTC, color) ); 
            pRenderer->VertexAttribPointer( ATTRIB_INDEX_TEXCOORD0, 2, GL_FLOAT, GL_FALSE, sizeof(CVertexPTC), (const GLvoid *)offsetof(CVertexPTC, tex) ); 
            FC_CHECK_GL_ERROR_DEBUG( );

            pRenderer->BindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_VBO[ 1 ] ); 

            pRenderer->BindBuffer( GL_ARRAY_BUFFER, 0 ); 
            pRenderer->BindVertexArray( 0 ); 
        }
        
        m_bInit = true;
    }

    const CParticleProperty* CParticleEmitter::GetParticleProperty() const
    {
        return m_pParticleProperty;
    }

};

