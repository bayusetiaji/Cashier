/*
 * xcurse.h
 * Shorter version of SetConsoleCursorPosition() and SetConsoleTextAttribute()
 *
 * bayusetiaji@amikom.ac.id
 * 2018
*/

#ifndef XCURSE_H
#define XCURSE_H

#include <Windows.h>

namespace xc
{
	const HANDLE oHandle = GetStdHandle(STD_OUTPUT_HANDLE); 
	
	void xy(const short x, const short y)
	{
		COORD coord = {x, y};
		
		SetConsoleCursorPosition(oHandle, coord);
	}
	
	void color(const short fcol, const short bcol)
	{
		const short fc = fcol < 0  ? 0 :
						 fcol > 15 ? 15:
						 fcol;
						 
		const short bc = bcol < 0  ? 0 :
						 bcol > 15 ? 15:
						 bcol;
						 
		SetConsoleTextAttribute(oHandle, bc * 16 + fc);
	}
}

#endif