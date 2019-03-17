#pragma warning (disable:4996)
#include"string_tools.h"
#include"dictionary.h"
#include<stdlib.h>


//fp�� �� �� �б� or \n(enter)�� �Է��� ���� ������ ��� �Է� ����. �� �Է¹��� ������ ���� return
//������ ������ �� �� ���� -1 return
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



//�����迭������, ������ �迭������, ��ū ���й���[' ','(' ��]�� �޾� �� �迭��  ù ��ū�� �߶� ����.
//��ū ���й��ڰ� ���� ��� ���� ������ ����
//��ū���й��� �ڸ� index�� \0 �ְ�, ���� index�� return
int getToken(char *data, char *copy, char delim, int dataIndex) {
	int copyIndex = 0;
	while (data[dataIndex] != delim && data[dataIndex] != '\0') {
		copy[copyIndex] = data[dataIndex];
		copyIndex++;
		dataIndex++;
	}

	copy[copyIndex] = '\0';

	return dataIndex + 1;
	//�̴�� ������ commandLineIndex�� ��ū���й��ڸ� ����Ű�� ����. �׷��Ƿ� return�� �� +1�ؼ� return
}


//���� data�迭�� index���� ������ �а� �����Ҵ��Ͽ� ��ȯ
char* getNextAll(char *data, int dataIndex) {
	char *tmp = (char *)malloc(COMMAND_SIZE * sizeof(char));
	int i = 0;

	//ó������ ��� ������ ���鹮�ڴ� ���� �ʴ´�.
	while (data[dataIndex] == ' ') {
		dataIndex++;
	}
	//�迭�� ������ ������ ����.
	while (data[dataIndex] != '\0') {
		tmp[i] = data[dataIndex];
		i++;
		dataIndex++;
	}
	tmp[i] = '\0';

	return tmp;
}
