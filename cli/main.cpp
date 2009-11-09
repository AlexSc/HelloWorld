/*
 *  main.cpp
 *  
 *
 *  Created by Alex Scarborough on 11/9/09.
 *  Copyright 2009 GarageGames.com, Inc. All rights reserved.
 *
 */

#include "hello.h"
#include <stdio.h>

int main(int, const char**)
{
	hello Hello;
	printf("%s\n", Hello.i_say());
   return 0;
}
