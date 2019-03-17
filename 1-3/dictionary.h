#ifndef DICTIONARY_H
#define DICTIONARY_H


#define COMMAND_SIZE 50
#define BUFFER_SIZE 5000
#define INIT_CAPACITY 200000
#define WORD_SIZE 30
#define VOLUME 200

//�ܾ� ����ü
typedef struct vocabulary {
	char *word;		//�ܾ�
	char **wordClass;	//�ܾ� + ǰ��
	char **explains;		//�ܾ��� ������� �����͹迭
	int explainCount;		//������ �� ����
}Vocabulary;

void intDirectory();
void process_command();
void freeDirectory();
char* getNextAll(char *data, int dataIndex);
void read(char *file_name);
void vocabularyControl(char *word, char *wordClass, char *explain);
Vocabulary *makeVocabulary(char *word, char *wordClass, char *explain);
void addExplain(char *wordClass, char *explain, int addIndex);
void findControl(char *target);
int find(char *target, int begin, int end);
char *changeForCmp(char *data);
int cmpWords(char *data1, char *data2);


#endif