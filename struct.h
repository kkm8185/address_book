//////////////////////////////////////////////////////////////////////////
// File Name	: struct.h												//
// Date	 		: 2014.06.04											//
// Compiler 	: gcc 4.8.4												//
// Os	 		: LinuxMint												//
// Author		: Kim kyung min											//
//----------------------------------------------------------------------//
// ver			: 1.0.1													//
// Description	: Address Book											//
//////////////////////////////////////////////////////////////////////////
#include <stdio.h>
typedef struct{
	char name[20];
	char phone[20];
	struct Person *next;
}Person;
// struct Person