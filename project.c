//////////////////////////////////////////////////////////////////////////
// File Name	: project.c												//
// Date	 		: 2014.06.04											//
// Compiler 	: gcc 4.8.4												//
// Os	 		: LinuxMint												//
// Author		: Kim kyung min											//
//----------------------------------------------------------------------//
// ver			: 1.0.1													//
// Description	: Address Book											//
//////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"
Person *person_head;
extern void argc_check(int,char* argv[]);	// �ٸ� ������ argc_check �Լ� ���� ����.
int main(int argc, char* argv[]){
	argc_check(argc,argv);	// func.c������ function call ( argc �� �Է� ����, argv�� �Է� ����(�迭���·� ����)
	return 0;
}

