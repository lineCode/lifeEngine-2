/**
 * @file
 * @addtogroup Engine Engine
 *
 * Copyright Broken Singularity, All Rights Reserved.
 * Authors: Yehor Pohuliaka (zombiHello)
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include <unordered_map>
#include <string>

#include "Math/Math.h"
#include "Misc/EngineGlobals.h"
#include "System/Package.h"
#include "RHI/BaseShaderRHI.h"
#include "Render/Shaders/ShaderManager.h"
#include "Render/Shaders/Shader.h"
#include "Render/Texture.h"
#include "Core.h"

/**
 * @ingroup Engine Engine
 * @brief Reference to FMaterial
 */
typedef TRefCountPtr< class FMaterial >				FMaterialRef;

/**
 * @ingroup Engine
 * Flags usage material
 */
enum EMaterialUsage
{
	MU_None				= 0,				/**< Material not used on the all meshes */
	MU_StaticMesh		= 1 << 0,			/**< Material used on the static meshes */
	MU_AllMeshes		= MU_StaticMesh		/**< Material used on all type meshes */
};

/**
 * @ingroup Engine
 * Material
 */
class FMaterial : public FAsset
{
public:
	/**
	 * Constructor
	 */
	FMaterial();

	/**
	 * Destructor
	 */
	~FMaterial();

	/**
	 * Serialize to archive
	 *
	 * @param[in] InArchive Archive
	 */
	virtual void Serialize( class FArchive& InArchive ) override;

	/**
	 * Set pixel shader
	 *
	 * @param[in] InShaderMetaType Meta info about shader
	 */
	FORCEINLINE void SetShader( const class FShaderMetaType& InShaderMetaType )
	{
		check( InShaderMetaType.GetFrequency() < SF_NumDrawFrequencies );
		shadersType[ InShaderMetaType.GetFrequency() ] = ( class FShaderMetaType* ) &InShaderMetaType;
		isNeedUpdateShaderMap = true;
	}

	/**
	 * Set scalar parameter value
	 *
	 * @param[in] InParameterName Parameter name
	 * @param[in] InValue Value
	 */
	FORCEINLINE void SetScalarParameterValue( const std::wstring& InParameterName, float InValue )
	{
		scalarParameters[ InParameterName ] = InValue;
	}

	/**
	 * Set texture parameter value
	 *
	 * @param[in] InParameterName Parameter name
	 * @param[in] InValue Value
	 */
	FORCEINLINE void SetTextureParameterValue( const std::wstring& InParameterName, class FTexture2D* InValue )
	{
		textureParameters[ InParameterName ] = InValue;
	}

	/**
	 * Set two sided mode
	 * 
	 * @param[in] InIsTwoSided Enable two sided mode
	 */
	FORCEINLINE void SetTwoSided( bool InIsTwoSided )
	{
		isTwoSided = InIsTwoSided;
	}

	/**
	 * Set wireframe mode
	 * 
	 * @param[in] InIsWireframe Enable wireframe mode
	 */
	FORCEINLINE void SetWireframe( bool InIsWireframe )
	{
		isWireframe = InIsWireframe;
	}

	/**
	 * Set use material on static meshes
	 * 
	 * @param[in] InIsUseOnStaticMeshes Is use this material on static meshes
	 */
	FORCEINLINE void UsageOnStaticMesh( bool InIsUseOnStaticMeshes )
	{
		if ( InIsUseOnStaticMeshes )
		{
			usage |= MU_StaticMesh;
		}
		else
		{
			usage &= ~MU_StaticMesh;
		}

		isNeedUpdateShaderMap = true;
	}

	/**
	 * Set usage flags
	 * 
	 * @param[in] InUsageFlags Usage flags of material (see EMaterialUsage)
	 */
	FORCEINLINE void SetUsageFlags( uint32 InUsageFlags )
	{
		usage = InUsageFlags;
		isNeedUpdateShaderMap = true;
	}

	/**
	 * Get usage flags
	 * @return Return usage flags
	 */
	FORCEINLINE uint32 GetUsageFlags() const
	{
		return usage;
	}

	/**
	 * Get pixel shader
	 *
	 * @param[in] InVertexFactoryHash Vertex factory hash
	 * @param[in] InShaderFrequency Shader frequency
	 * @return Return pointer to pixel shader
	 */
	FShaderRef GetShader( uint32 InVertexFactoryHash, EShaderFrequency InShaderFrequency );

	/**
	 * Get scalar parameter value
	 *
	 * @param[in] InParameterName Parameter name
	 * @param[out] OutValue Return value
	 * @return Return true if finded, else return false
	 */
	bool GetScalarParameterValue( const std::wstring& InParameterName, float& OutValue ) const;

	/**
	 * Get texture parameter value
	 *
	 * @param[in] InParameterName Parameter name
	 * @param[out] OutValue Return value
	 * @return Return true if finded, else return false
	 */
	bool GetTextureParameterValue( const std::wstring& InParameterName, FTexture2DRef& OutValue ) const;

	/**
	 * Is enabled two sided mode
	 * @return Return true if two sided mode enabled, else return false
	 */
	FORCEINLINE bool IsTwoSided() const
	{
		return isTwoSided;
	}

	/**
	 * Is enabled wireframe mode
	 * @return Return true if wireframe mode enabled, else return false
	 */
	FORCEINLINE bool IsWireframe() const
	{
		return isWireframe;
	}

private:
	/**
	 * Cache of shader map
	 */
	void CacheShaderMap();

	/**
	 * Get array shader instances for mesh type
	 * 
	 * @param[in] InVertexFactoryHash Vertex factory hash
	 * @return Return array shader instances for mesh type
	 */
	std::vector< FShaderRef > GetMeshShaders( uint32 InVertexFactoryHash ) const;

	/**
	 * Typedef mesh shader map
	 */
	typedef std::unordered_map< uint32, std::vector< FShaderRef > >				FMeshShaderMap;

	bool												isNeedUpdateShaderMap;					/**< Is need update shader map */
	bool												isTwoSided;								/**< Is two sided material */
	bool												isWireframe;							/**< Is wireframe material */
	uint32												usage;									/**< Usage flags (see EMaterialUsage) */
	class FShaderMetaType*								shadersType[ SF_NumDrawFrequencies ];	/**< Array shader types for material */
	FMeshShaderMap										shaderMap;								/**< Shader map for material */
	std::unordered_map< std::wstring, float >			scalarParameters;						/**< Array scalar parameters */
	std::unordered_map< std::wstring, FTexture2DRef >	textureParameters;						/**< Array texture parameters */
};

//
// Serialization
//

FORCEINLINE FArchive& operator<<( FArchive& InArchive, FMaterialRef& InValue )
{
	if ( InArchive.IsSaving() )
	{
		InArchive << ( InValue ? InValue->GetAssetReference() : FAssetReference() );
	}
	else
	{
		FAssetReference			assetReference;
		InArchive << assetReference;
		if ( assetReference.IsValid() )
		{
			InValue = GPackageManager->FindAsset( assetReference.pathPackage, assetReference.hash );
		}
	}

	return InArchive;
}

FORCEINLINE FArchive& operator<<( FArchive& InArchive, const FMaterialRef& InValue )
{
	check( InArchive.IsSaving() );
	InArchive << ( InValue ? InValue->GetAssetReference() : FAssetReference() );
	return InArchive;
}

#endif // !MATERIAL_H