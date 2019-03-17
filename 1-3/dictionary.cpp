#pragma warning (disable:4996)
#include"dictionary.h"
#include"string_tools.h"
#include<stdio.h>
#include<string.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

Vocabulary** dictionary = NULL;			//Vocabulary���� �����͹迭
int wordCount = 0;	//��ü �ܾ��� ����
int dictionaryIndex = 0;



//�ڷᱸ��(Vocabulary����ü �����͹迭)�� �޸� �Ҵ�
void intDirectory() {
	dictionary = (Vocabulary**)malloc(INIT_CAPACITY * sizeof(Vocabulary *));
}

//command �ν�, �з� : read, size, find, exit
void process_command() {
	char commandLine[COMMAND_SIZE];
	char *extra = NULL;
	char command[COMMAND_SIZE];

	while (1) {
		printf("$ ");

		//Ű���� �Է� : �Է� ������ ��� �Է� ��ٸ�.
		if (readLine(stdin, commandLine, COMMAND_SIZE) <= 0)
			continue;

		//commandLineIndex : �Է��� commandLine�� �ε���
		int commandLineIndex = getToken(commandLine, command, ' ', 0);

		//		printf("command[%s]\n", command);


		if (strcmp(command, "read") == 0) {
			//�����̸��� ���� �迭 �����Ͽ� �����Ҵ�
			extra = getNextAll(commandLine, commandLineIndex);
			read(extra);
			free(extra);
			continue;
		}
		else if (strcmp(command, "size") == 0) {
			printf("%d\n", wordCount);
			continue;
		}
		else if (strcmp(command, "find") == 0) {
			extra = getNextAll(commandLine, commandLineIndex);	//ã�� �ܾ� ������ �о extra�� ����
			printf("[%s]\n", extra);
			findControl(extra);		//ã�� �ܾ� ��� �����ϴ� �Լ�
			free(extra);
			continue;
		}
		else if (strcmp(command, "exit") == 0) {
			break;
		}
		else {//�������� �ʴ� command. ���Է�
			printf("Invalid command\n");
			continue;
		}

	}
}


//�ڷ��� �����Ҵ� ����
void freeDirectory() {
	int i = 0;
	while (i < dictionaryIndex) {
		free(dictionary[i]->word);
		for (int j = 0; j < dictionary[i]->explainCount; j++) {
			free(dictionary[i]->explains[j]);
			free(dictionary[i]->wordClass[j]);
		}
		free(dictionary[i]->explains);
		free(dictionary[i]->wordClass);
		i++;
	}
	free(dictionary);
}


void read(char *fileName) {
	char buf1[BUFFER_SIZE];	//�� �� �о���̴� �迭 & �ܾ��
	char *word;	//�ܾ�
	char *wordClass;//�ܾ� ǰ��

	FILE *fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("Open failed.\n");
		return;
	}

	while (1) {

		//EOF�̸� -1 return �ϹǷ� ���� ��
		if (readLine(fp, buf1, BUFFER_SIZE) < 0)
			break;
		//\n�� ������ �� cotinue;
		if (strlen(buf1) == 0)
			continue;

		char *tmp1 = strdup(buf1);
		word = strtok(tmp1, "(");
		word[strlen(word) - 1] = '\0';

		char *tmp2 = strdup(buf1);
		wordClass = strdup(strtok(tmp2, ")"));
		wordClass[strlen(wordClass) + 1] = '\0';
		wordClass[strlen(wordClass)] = ')';

		//		printf("<%s><%s>/%d/%d\n", word, wordClass, wordCount, dictionaryIndex);

		vocabularyControl(word, wordClass, buf1);
		wordCount++;

		free(tmp1);
		free(tmp2);

	}
	fclose(fp);

}

//������ �ڷᱸ���� ���� ��Ʈ�ѷ�
void vocabularyControl(char *word, char *wordClass, char *explain) {
	if (wordCount == 0) {	//ù �ܾ�
		dictionary[dictionaryIndex] = makeVocabulary(word, wordClass, explain);
		dictionaryIndex++;
		return;
	}
	int i = 1;
	while (dictionaryIndex - i >= 0 && i <= 10) {
		if (strcmp(dictionary[dictionaryIndex - i]->word, word) == 0) {
			//�ٸ��ܾ�,���ĵǾ� �����Ƿ� �� �ܾ�� ������ �˻����ָ� ��
			//���� �ܾ�
			addExplain(wordClass, explain, dictionaryIndex - i);
			return;
		}
		i++;
	}
	//�ٸ� �ܾ�
	dictionary[dictionaryIndex] = makeVocabulary(word, wordClass, explain);
	dictionaryIndex++;
}

//Vocabulary��ü ����, �ʱ�ȭ
Vocabulary *makeVocabulary(char *word, char *wordClass, char *explain) {
	Vocabulary *tmp = (Vocabulary *)malloc(sizeof(Vocabulary));
	tmp->explains = (char **)malloc(VOLUME * sizeof(char*));
	tmp->wordClass = (char **)malloc(VOLUME * sizeof(char*));
	tmp->explainCount = 0;
	tmp->explains[0] = strdup(explain);	//����
	tmp->word = strdup(word);	//�ܾ�
	tmp->wordClass[0] = strdup(wordClass);	//�ܾ� + ǰ��
	tmp->explainCount++;	//���� count����

	return tmp;
}


//���� �ܾ��� �ε����� explain, wordClass �߰�. explainCount++
void addExplain(char *wordClass, char *explain, int addIndex) {
	int i = dictionary[addIndex]->explainCount;
	dictionary[addIndex]->explains[i] = strdup(explain);	//����
	dictionary[addIndex]->wordClass[i] = strdup(wordClass);	//��� + ǰ��
	dictionary[addIndex]->explainCount++;	//���� count����
}

//find�� ã�Ƽ� index��ȯ
void findControl(char *target) {
	char *cmpTarget = changeForCmp(target);

	int findIndex = find(cmpTarget, 0, dictionaryIndex);

	if (findIndex < 0) {
		printf("Not found.\n");
		printf("Front word doesn't exist.\n");
		printf("- - - - - - - -\n");
		printf("%s\n", dictionary[0]->wordClass[0]);
	}
	else if (cmpWords(dictionary[findIndex]->word, cmpTarget) == 0) {
		printf("Found %d items.\n", dictionary[findIndex]->explainCount);
		for (int i = 0; i < dictionary[findIndex]->explainCount; i++)
			printf("%s\n", dictionary[findIndex]->explains[i]);
	}
	else {
		printf("Not found.\n");
		printf("%s\n", dictionary[findIndex]->wordClass[(dictionary[findIndex]->explainCount) - 1]);
		printf("- - -\n");
		printf("%s\n", dictionary[findIndex + 1]->wordClass[0]);
	}

	free(cmpTarget);

}


/*O(logN)�� �Ǿ���Ѵ�.
ã�� �ܾ� ������ ���, �� �ܾ��� �迭 �ε��� ��ȯ
�������� ���� ���, �� �ܾ��� �ٷ� �� �ܾ��� �迭 �ε����� ��ȯ
�ٷ� ���� �ܾ �������� ������ -1��ȯ
*/
int find(char *target, int begin, int end) {
	if (begin > end) {
		if (end < 0)
			return -1;		//target�� ������ �۴ٴ� ���̹Ƿ� middle-1 ~ end�� �ϸ� [-1 ~ 0]�� �����̹Ƿ� end�� -1
		else
			return end;		//��� �ϵ� end�� target�� �� index�̴�.
	}
	else {
		int middle = (begin + end) / 2;
		if (cmpWords(dictionary[middle]->word, target) == 0)
			return middle;
		else if (cmpWords(dictionary[middle]->word, target) > 0)
			return find(target, begin, middle - 1);
		else
			return find(target, middle + 1, end);
	}
}

//���ο� ���ڿ��� �����Ҵ�, �빮�ڸ� �ҹ��ڷ� ��ȯ�Ͽ� ���� �� ��ȯ
char *changeForCmp(char *data) {
	char *str = (char *)malloc(WORD_SIZE * sizeof(char));
	int i = 0, dataIndex = 0;
	int end = strlen(data);
	while (i < end) {
		if (data[dataIndex] >= 'A' && data[dataIndex] <= 'Z')
			str[i++] = tolower(data[dataIndex++]);
		else if (data[dataIndex] >= 'a' && data[dataIndex] <= 'z')
			str[i++] = data[dataIndex++];
		else
			dataIndex++;
	}
	str[i] = '\0';

	return str;
}

//�� �ܾ ��� �ҹ��ڷ� ��ȯ�Ͽ� ���ϰ�, strcmp���� ��ȯ
int cmpWords(char *data1, char *data2) {
	char *cmpData1 = changeForCmp(data1);
	char *cmpData2 = changeForCmp(data2);

	int cmp = strcmp(cmpData1, cmpData2);

	free(cmpData1);
	free(cmpData2);

	return cmp;
}