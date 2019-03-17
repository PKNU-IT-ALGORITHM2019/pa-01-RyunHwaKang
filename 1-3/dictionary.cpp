#pragma warning (disable:4996)
#include"dictionary.h"
#include"string_tools.h"
#include<stdio.h>
#include<string.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

Vocabulary** dictionary = NULL;			//Vocabulary들의 포인터배열
int wordCount = 0;	//전체 단어의 개수
int dictionaryIndex = 0;



//자료구조(Vocabulary구조체 포인터배열)의 메모리 할당
void intDirectory() {
	dictionary = (Vocabulary**)malloc(INIT_CAPACITY * sizeof(Vocabulary *));
}

//command 인식, 분류 : read, size, find, exit
void process_command() {
	char commandLine[COMMAND_SIZE];
	char *extra = NULL;
	char command[COMMAND_SIZE];

	while (1) {
		printf("$ ");

		//키보드 입력 : 입력 없으면 계속 입력 기다림.
		if (readLine(stdin, commandLine, COMMAND_SIZE) <= 0)
			continue;

		//commandLineIndex : 입력한 commandLine의 인덱스
		int commandLineIndex = getToken(commandLine, command, ' ', 0);

		//		printf("command[%s]\n", command);


		if (strcmp(command, "read") == 0) {
			//파일이름을 받을 배열 복사하여 동적할당
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
			extra = getNextAll(commandLine, commandLineIndex);	//찾을 단어 끝까지 읽어서 extra에 저장
			printf("[%s]\n", extra);
			findControl(extra);		//찾은 단어 출력 조절하는 함수
			free(extra);
			continue;
		}
		else if (strcmp(command, "exit") == 0) {
			break;
		}
		else {//존재하지 않는 command. 재입력
			printf("Invalid command\n");
			continue;
		}

	}
}


//자료의 동적할당 해제
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
	char buf1[BUFFER_SIZE];	//한 줄 읽어들이는 배열 & 단어설명
	char *word;	//단어
	char *wordClass;//단어 품사

	FILE *fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("Open failed.\n");
		return;
	}

	while (1) {

		//EOF이면 -1 return 하므로 파일 끝
		if (readLine(fp, buf1, BUFFER_SIZE) < 0)
			break;
		//\n만 존재할 때 cotinue;
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

//정보들 자료구조에 저장 컨트롤러
void vocabularyControl(char *word, char *wordClass, char *explain) {
	if (wordCount == 0) {	//첫 단어
		dictionary[dictionaryIndex] = makeVocabulary(word, wordClass, explain);
		dictionaryIndex++;
		return;
	}
	int i = 1;
	while (dictionaryIndex - i >= 0 && i <= 10) {
		if (strcmp(dictionary[dictionaryIndex - i]->word, word) == 0) {
			//다른단어,정렬되어 있으므로 앞 단어와 같은지 검사해주면 됨
			//같은 단어
			addExplain(wordClass, explain, dictionaryIndex - i);
			return;
		}
		i++;
	}
	//다른 단어
	dictionary[dictionaryIndex] = makeVocabulary(word, wordClass, explain);
	dictionaryIndex++;
}

//Vocabulary객체 생성, 초기화
Vocabulary *makeVocabulary(char *word, char *wordClass, char *explain) {
	Vocabulary *tmp = (Vocabulary *)malloc(sizeof(Vocabulary));
	tmp->explains = (char **)malloc(VOLUME * sizeof(char*));
	tmp->wordClass = (char **)malloc(VOLUME * sizeof(char*));
	tmp->explainCount = 0;
	tmp->explains[0] = strdup(explain);	//설명
	tmp->word = strdup(word);	//단어
	tmp->wordClass[0] = strdup(wordClass);	//단어 + 품사
	tmp->explainCount++;	//설명 count증가

	return tmp;
}


//같은 단어의 인덱스에 explain, wordClass 추가. explainCount++
void addExplain(char *wordClass, char *explain, int addIndex) {
	int i = dictionary[addIndex]->explainCount;
	dictionary[addIndex]->explains[i] = strdup(explain);	//설명
	dictionary[addIndex]->wordClass[i] = strdup(wordClass);	//명사 + 품사
	dictionary[addIndex]->explainCount++;	//설명 count증가
}

//find로 찾아서 index반환
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


/*O(logN)이 되어야한다.
찾는 단어 존재할 경우, 그 단어의 배열 인덱스 반환
존재하지 않을 경우, 그 단어의 바로 앞 단어의 배열 인덱스를 반환
바로 앞의 단어가 존재하지 않으면 -1반환
*/
int find(char *target, int begin, int end) {
	if (begin > end) {
		if (end < 0)
			return -1;		//target이 언제나 작다는 것이므로 middle-1 ~ end를 하면 [-1 ~ 0]의 범위이므로 end가 -1
		else
			return end;		//어떻게 하든 end가 target의 앞 index이다.
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

//새로운 문자열을 동적할당, 대문자만 소문자로 변환하여 저장 후 반환
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

//두 단어를 모두 소문자로 변환하여 비교하고, strcmp값을 반환
int cmpWords(char *data1, char *data2) {
	char *cmpData1 = changeForCmp(data1);
	char *cmpData2 = changeForCmp(data2);

	int cmp = strcmp(cmpData1, cmpData2);

	free(cmpData1);
	free(cmpData2);

	return cmp;
}