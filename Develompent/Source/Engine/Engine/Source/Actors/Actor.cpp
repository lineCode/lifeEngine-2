#include "Actors/Actor.h"
#include "Components/ActorComponent.h"

IMPLEMENT_CLASS( AActor )

#if WITH_EDITOR
FActorVar::FActorVar()
	: type( AVT_Unknown )
	, value( nullptr )
{}

FActorVar::FActorVar( const FActorVar& InCopy )
{
	*this = InCopy;
}

void FActorVar::Clear()
{
	if ( !value )
	{
		return;
	}

	switch ( type )
	{
	case AVT_Int:		delete static_cast< int32* >( value );			break;
	case AVT_Float:		delete static_cast< float* >( value );			break;
	case AVT_Bool:		delete static_cast< bool* >( value );			break;
	case AVT_Vector2D:	delete static_cast< FVector2D* >( value );		break;
	case AVT_Vector3D:	delete static_cast< FVector* >( value );		break;
	case AVT_Vector4D:	delete static_cast< FVector4D* >( value );		break;
	case AVT_RectInt:	delete static_cast< FRectInt32* >( value );		break;
	case AVT_RectFloat:	delete static_cast< FRectFloat* >( value );		break;
	case AVT_Color:		delete static_cast< FColor* >( value );			break;
	case AVT_String:	delete static_cast< std::wstring* >( value );	break;
	case AVT_Material:	delete static_cast< FMaterialPtr* >( value );	break;
	}

	value = nullptr;
	type = AVT_Unknown;
}
#endif // WITH_EDITOR

AActor::AActor()
	: bIsStatic( false )
	, bNeedReinitCollision( false )
{}

AActor::~AActor()
{
	ResetOwnedComponents();
}

void AActor::BeginPlay()
{
	for ( uint32 index = 0, count = ( uint32 )ownedComponents.size(); index < count; ++index )
	{
		ownedComponents[ index ]->BeginPlay();
	}
}

void AActor::Tick( float InDeltaTime )
{
	for ( uint32 index = 0, count = ( uint32 )ownedComponents.size(); index < count; ++index )
	{
		ownedComponents[ index ]->TickComponent( InDeltaTime );
	}

	// Reinit collision if need
	if ( bNeedReinitCollision )
	{
		TermPhysics();
		InitPhysics();
		bNeedReinitCollision = false;
	}
}

void AActor::Serialize( class FArchive& InArchive )
{
	Super::Serialize( InArchive );
	InArchive << bIsStatic;

	for ( uint32 index = 0, count = ( uint32 )ownedComponents.size(); index < count; ++index )
	{
		ownedComponents[ index ]->Serialize( InArchive );
	}
}

void AActor::InitPhysics()
{
	if ( collisionComponent )
	{
		collisionComponent->InitPrimitivePhysics();
	}
}

void AActor::TermPhysics()
{
	if ( collisionComponent )
	{
		collisionComponent->TermPrimitivePhysics();
	}
}

void AActor::SyncPhysics()
{
	if ( collisionComponent )
	{
		collisionComponent->SyncComponentToPhysics();
	}
}

#if WITH_EDITOR
bool AActor::InitProperties( const std::vector<FActorVar>& InActorVars, class LCookPackagesCommandlet* InCooker )
{
	return true;
}
#endif // WITH_EDITOR

LActorComponentRef AActor::CreateComponent( LClass* InClass, const tchar* InName )
{
	check( InClass );

	LObject*				newObject = InClass->CreateObject();
	check( newObject->IsA< LActorComponent >() );

	LActorComponent*		component = newObject->Cast< LActorComponent >();
	check( component );

	bool		bIsASceneComponent		= component->IsA< LSceneComponent >();

	// If created component is a LSceneComponent and RootComponent not setted - set it!
	if ( !rootComponent && bIsASceneComponent )
	{
		rootComponent = component->Cast< LSceneComponent >();
	}
	else if ( rootComponent && bIsASceneComponent )
	{
		( ( LSceneComponent* )component )->SetupAttachment( rootComponent );
	}

	component->SetName( InName );
	AddOwnedComponent( component );
	return component;
}

void AActor::AddOwnedComponent( class LActorComponent* InComponent )
{
	check( InComponent && InComponent->GetOwner() != this );

	// If already owned exist in component - remove component from owner
	{
		AActor*		owner = InComponent->GetOwner();
		if ( owner )
		{
			owner->RemoveOwnedComponent( InComponent );
		}
	}

	// Add component
	InComponent->SetOwner( this );
	ownedComponents.push_back( InComponent );
}

void AActor::RemoveOwnedComponent( class LActorComponent* InComponent )
{
	check( InComponent && InComponent->GetOwner() == this );
	for ( uint32 index = 0, count = ( uint32 )ownedComponents.size(); index < count; ++index )
	{
		LActorComponent*		component = ownedComponents[ index ];
		if ( component == InComponent )
		{
			if ( component == collisionComponent )
			{
				collisionComponent->TermPrimitivePhysics();
				collisionComponent = nullptr;
			}
			else if ( component == rootComponent )
			{
				check( false && "Need implement change root component" );
			}

			InComponent->SetOwner( nullptr );
			ownedComponents.erase( ownedComponents.begin() + index );
			return;
		}
	}
}

void AActor::ResetOwnedComponents()
{
	for ( uint32 index = 0, count = ( uint32 )ownedComponents.size(); index < count; ++index )
	{
		if ( collisionComponent == ownedComponents[ index ] )
		{
			collisionComponent->TermPrimitivePhysics();
		}
		ownedComponents[ index ]->SetOwner( nullptr );
	}

	ownedComponents.clear();
	collisionComponent	= nullptr;
	rootComponent		= nullptr;
}