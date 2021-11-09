/**
 * @file
 * @addtogroup Engine Engine
 *
 * Copyright Broken Singularity, All Rights Reserved.
 * Authors: Yehor Pohuliaka (zombiHello)
 */

#ifndef DRAWINGPOLICY_H
#define DRAWINGPOLICY_H

#include "Misc/RefCountPtr.h"
#include "Render/Material.h"
#include "Render/VertexFactory/VertexFactory.h"
#include "Core.h"

/**
 * @ingroup Engine
 * The base mesh drawing policy.  Subclasses are used to draw meshes with type-specific context variables.
 * May be used either simply as a helper to render a dynamic mesh, or as a static instance shared between
 * similar meshs
 */
class FMeshDrawingPolicy
{
public:
	/**
	 * Constructor
	 * 
	 * @param[in] InVertexFactory Vertex factory
	 * @param[in] InMaterial Material
	 * @param[in] InDepthBias Depth bias
	 */
	FMeshDrawingPolicy( class FVertexFactory* InVertexFactory, class FMaterial* InMaterial, float InDepthBias = 0.f );

	/**
	 * Destructor
	 */
	virtual ~FMeshDrawingPolicy();

	/**
	 * Set render state for drawing
	 * 
	 * @param[in] InDeviceContextRHI RHI device context
	 */
	virtual void SetRenderState( class FBaseDeviceContextRHI* InDeviceContextRHI );

	/**
	 * Set shader parameters
	 * 
	 * @param[in] InDeviceContextRHI RHI device context
	 */
	virtual void SetShaderParameters( class FBaseDeviceContextRHI* InDeviceContextRHI );

	/**
	 * Draw mesh
	 * 
	 * @param[in] InDeviceContextRHI RHI device context
	 * @param[in] InMeshBatch Mesh batch to draw
	 * @param[in] InSceneView Scene view
	 */
	virtual void Draw( class FBaseDeviceContextRHI* InDeviceContextRHI, const struct FMeshBatch& InMeshBatch, const class FSceneView& InSceneView ) PURE_VIRTUAL( FMeshDrawingPolicy::Draw, );

protected:
	FMaterialRef				material;			/**< Material */
	FVertexFactoryRef			vertexFactory;		/**< Vertex factory */
	FShaderRef					vertexShader;		/**< Vertex shader */
	FShaderRef					pixelShader;		/**< Pixel shader */
	float						depthBias;			/**< Depth bias */
};

#endif // !DRAWINGPOLICY_H