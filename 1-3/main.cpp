#pragma warning (disable:4996)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include"dictionary.h"


void main() {
	//자료 메모리 동적할당
	intDirectory();

	//command입력 + 분류 + 실행
	process_command();

	//자료 메모리 free
	freeDirectory();
}