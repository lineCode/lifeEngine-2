#include "System/EditorConstraints.h"

FEditorConstraints::FEditorConstraints()
	: currentGridSize( 4 )
{
	// Set default grid sizes, powers of two
	for ( uint32 index = 0; index < MaxGridSizes; ++index )
	{
		gridSizes[ index ] = ( float )( 1 << index );
	}
}