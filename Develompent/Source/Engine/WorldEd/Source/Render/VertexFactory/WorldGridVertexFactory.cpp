#include "Misc/Template.h"
#include "Render/VertexFactory/WorldGridVertexFactory.h"
#include "Render/VertexFactory/GeneralVertexFactoryParams.h"

IMPLEMENT_VERTEX_FACTORY_TYPE( FWorldGridVertexFactory, TEXT( "WorldGridVertexFactory.hlsl" ) )

void FWorldGridVertexFactory::InitRHI()
{
	FVertexDeclarationElementList		vertexDeclElementList =
	{
        FVertexElement( SSS_Main, sizeof( FWorldGridVertexType ), STRUCT_OFFSET( FWorldGridVertexType, position ), VET_Float4, VEU_Position, 0 ),
	};

	InitDeclaration( vertexDeclElementList );
}

FVertexFactoryShaderParameters* FWorldGridVertexFactory::ConstructShaderParameters( EShaderFrequency InShaderFrequency )
{
    return InShaderFrequency == SF_Vertex ? new FGeneralVertexShaderParameters() : nullptr;
}
