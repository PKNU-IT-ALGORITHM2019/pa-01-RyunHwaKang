#pragma warning (disable:4996)
#include"string_tools.h"
#include"dictionary.h"
#include<stdlib.h>


//fp의 한 줄 읽기 or \n(enter)의 입력이 있을 때까지 계속 입력 받음. 총 입력받은 문자의 개수 return
//마지막 파일의 끝 일 때는 -1 return
int readLine(FILE *fp, char str[], int n) {
	char ch;
	int i = 0;

	while ((ch = fgetc(fp)) != '\n' && ch != EOF)
		if (i < n)
			str[i++] = ch;
	str[i] = '\0';
	if ((ch == EOF) && (i == 0))
		return -1;
	return i;
}



//원본배열포인터, 복사할 배열포인터, 토큰 구분문자[' ','(' 등]을 받아 그 배열을  첫 토큰을 잘라 복사.
//토큰 구분문자가 없을 경우 파일 끝에서 끝남
//토큰구분문자 자리 index에 \0 넣고, 다음 index를 return
int getToken(char *data, char *copy, char delim, int dataIndex) {
	int copyIndex = 0;
	while (data[dataIndex] != delim && data[dataIndex] != '\0') {
		copy[copyIndex] = data[dataIndex];
		copyIndex++;
		dataIndex++;
	}

	copy[copyIndex] = '\0';

	return dataIndex + 1;
	//이대로 나오면 commandLineIndex는 토큰구분문자를 가르키고 있음. 그러므로 return할 때 +1해서 return
}


//받은 data배열의 index부터 끝까지 읽고 동적할당하여 반환
char* getNextAll(char *data, int dataIndex) {
	char *tmp = (char *)malloc(COMMAND_SIZE * sizeof(char));
	int i = 0;

	//처음부터 계속 나오는 공백문자는 읽지 않는다.
	while (data[dataIndex] == ' ') {
		dataIndex++;
	}
	//배열의 끝까지 읽으며 복사.
	while (data[dataIndex] != '\0') {
		tmp[i] = data[dataIndex];
		i++;
		dataIndex++;
	}
	tmp[i] = '\0';

	return tmp;
}
