#pragma once

#include <Windows.h>

#define STR_MERGE_IMPL(a,b) a##b
#define STR_MERGE(a,b) STR_MERGE_IMPL(a,b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMEBER_N(type, name, offset) struct{unsigned char MAKE_PAD(offset); type name;}

struct vec3{ float x, y, z; };

class Ent
{
public:
	union
	{
		DEFINE_MEMEBER_N( int, m_iHealth, 0xA8 );
		DEFINE_MEMEBER_N( int, team, 0xB0 );
		DEFINE_MEMEBER_N( vec3, origin, 0x28C );
	};
};

class IClientEntityList
{
public:
	// Get IClientNetworkable interface for specified entity
	virtual void * GetClientNetworkable( int entnum ) = 0;
	virtual void* GetClientNetworkableFromHandle( int hEnt ) = 0;
	virtual void* GetClientUnknownFromHandle( int hEnt ) = 0;

	// NOTE: This function is only a convenience wrapper.
	// It returns GetClientNetworkable( entnum )->GetIClientEntity().
	virtual void* GetClientEntity( int entnum ) = 0;
	virtual void* GetClientEntityFromHandle( int hEnt ) = 0;

	// Returns number of entities currently in use
	virtual int					NumberOfEntities( bool bIncludeNonNetworkable ) = 0;

	// Returns highest index actually used
	virtual int					GetHighestEntityIndex( void ) = 0;

	// Sizes entity list to specified size
	virtual void				SetMaxEntities( int maxents ) = 0;
	virtual int					GetMaxEntities( ) = 0;
};

using tCreateInterface = void* (__cdecl*)(const char* name, int* returnCode);

void* GetInterface( const wchar_t* dllName, const char* interfaceName )
{
	tCreateInterface CreateInterface{ (tCreateInterface)(GetProcAddress( GetModuleHandle( dllName ), "CreateInterface" )) };

	int returnCode{ 0 };
	void* Interface{ CreateInterface( interfaceName, &returnCode ) };

	return Interface;
}