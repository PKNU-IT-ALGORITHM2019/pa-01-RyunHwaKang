#pragma warning (disable:4996)
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include"dictionary.h"


void main() {
	//�ڷ� �޸� �����Ҵ�
	intDirectory();

	//command�Է� + �з� + ����
	process_command();

	//�ڷ� �޸� free
	freeDirectory();
}