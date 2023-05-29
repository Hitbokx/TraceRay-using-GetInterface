// dllmain.cpp : Defines the entry point for the DLL application.
#include <Windows.h>
#include <iostream>
#include "tf2sdk.h"
#include "Netvars.h"

DWORD WINAPI myThreadProc( HMODULE hInstDLL )
{
	AllocConsole( ); // To allocate console for logging
	FILE* f;
	freopen_s( &f, "CONOUT$", "w", stdout );
	std::cout << "Yo! Hitbokx here!\n";

	IClientEntityList* ClientEntityList{ (IClientEntityList*)GetInterface( L"client.dll", "VClientEntityList003" ) };

	IBaseClientDLL* baseClientDLL{ (IBaseClientDLL*)GetInterface( L"client.dll", "VClient017" ) };

	//"VClient017" changes, so do a string search in IDA

	while ( !(GetAsyncKeyState( VK_END ) & 1) )
	{
		ClientClass* clientClass{ baseClientDLL->GetAllClasses( ) };

		//if ( clientClass->m_pCreateFn )
		//{
		uintptr_t healthOffset{ (uintptr_t)(GetNetvarOffset( "DT_BasePlayer", "m_iHealth", clientClass )) };

		uintptr_t ent2{ (uintptr_t)(ClientEntityList->GetClientEntity( 1 )) };
		int* health = (int*)(ent2 + healthOffset);

		std::cout << "Ent2 health = " << *health << '\n';
		//}

		//std::cout << "Ent2 team = " << *health << '\n';

		Sleep( 100 );
	}
	//unload of Dll and reource deallocation code.

	if ( f )
		fclose( f );
	FreeConsole( );
	FreeLibraryAndExitThread( hInstDLL, 0 );//to call DLL_PROCESS_DETACH and resource deallocation
	return 0;
}

BOOL WINAPI DllMain( HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved )
{
	switch ( fdwReason )
	{
		case DLL_PROCESS_ATTACH:

			CloseHandle( CreateThread( nullptr, 0, LPTHREAD_START_ROUTINE( myThreadProc ), hinstDLL, 0, nullptr ) );
			break;

		case DLL_PROCESS_DETACH:
			//perform any necesary clean-up
			break;

	}
	return TRUE; //Succesful DLL_PROCESS_ATTACH
}