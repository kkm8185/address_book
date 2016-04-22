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
extern void argc_check(int,char* argv[]);	// 다른 파일의 argc_check 함수 참조 설정.
int main(int argc, char* argv[]){
	argc_check(argc,argv);	// func.c파일의 function call ( argc 는 입력 갯수, argv는 입력 값들(배열형태로 저장)
	return 0;
}

