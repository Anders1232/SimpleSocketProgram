/*
	ArgumentAnalizer, a simple argument analizer
	Copyright (C) 2016  Francisco Anderson Bezerra Rodrigues

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include<string.h>

#ifndef ARGUMENT_ANALIZER_H
#define ARGUMENT_ANALIZER_H

#define SEARCH_FAILED (-1)

#ifdef __cplusplus
namespace ArgumentAnalizer
{
extern "C" {
#endif


	//função responsável por procurar por um argumento no 
	int FindArgument(char const * wantedArtgument, int argc, char **argv)
	{
		for(int cont =0 ; cont < argc ; cont++)
		{
			if(!strcmp(wantedArtgument, argv[cont]))
			{
				return cont;
			}
		}
		return SEARCH_FAILED;
//		throw new string("Argument does not exists!");
	}
#ifdef __cplusplus
}//extern C
}//namesapce
#endif

#endif
