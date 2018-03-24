#include "stdio.h"
#include "stdlib.h"
#define MaxVertexNum 300
typedef struct node {
	int adjvertex;
	struct node *next;
}EdgeNode;
typedef struct vnode {
	int data;
	EdgeNode *firstnode;
}VertexNode;
typedef struct {//�ڽӱ�ṹ�壬�ṹ�ڼ�����������������ͳ���Թ�
	VertexNode adjlist[MaxVertexNum];
	int vertexNum;
	int rowNum;
	int colNum;
}ALGraph;
typedef struct {//�ڽӾ���洢�ṹ�����ڽӾ�����ʽ�洢�Թ�
	int arcs[MaxVertexNum][MaxVertexNum];
	int rowNum;
	int colNum;
}MGraph;
/*����һ���ڽӾ�����ʽ���Թ���0��ʾ��·��ͨ��1��ʾ��·��ͨ*/
void CreateMGraph(MGraph *G)
{
	printf("Please input the length and wide of the maze:\n");
	scanf("%d,%d",&(G->rowNum),&(G->colNum));
	printf("Please input the data of the maze:\n");
	for (int i = 0; i < G->rowNum; i++)
		for (int j = 0; j < G->colNum; j++)
		{
			scanf("%d", &G->arcs[i][j]);
			getchar();
		}
}
/*���ڽӾ���ת��Ϊ�ڽӱ�ͨ���ڽӱ�ı���ʵ���Թ�·����ѯ*/
ALGraph *MatrixtToList(MGraph *mg)
{
	int i, j;
	ALGraph *G;
	G = (ALGraph *)malloc(sizeof(ALGraph));
	EdgeNode *p;/*�����㣬������������ͬ*/
	G->vertexNum = (mg->colNum)*(mg->rowNum);
	G->colNum = mg->colNum;
	G->rowNum = mg->rowNum;
	for (i = 0; i<G->vertexNum; i++)/*ͼ�н��ĳ�ʼ��*/
	{
		G->adjlist[i].data =mg->arcs[i / mg->colNum][i%mg->colNum];
		G->adjlist[i].firstnode = NULL;
	}
	for(i=0;i<mg->rowNum;i++)/*�Ӹý��İ˸���������������ֱ������ڽӱ�ṹ*/
		for (j = 0; j < mg->colNum; j++)
		{
			if (i - 1 >= 0 && mg->arcs[i - 1][j] == 0)
			{
				p = (EdgeNode *)malloc(sizeof(EdgeNode));
				p->adjvertex = (i - 1)*mg->colNum + j;
				p->next = G->adjlist[i*mg->colNum+j].firstnode;
				G->adjlist[i*mg->colNum + j].firstnode = p;
			}
			if (i + 1 <= mg->rowNum && mg->arcs[i + 1][j] == 0)
			{
				p = (EdgeNode *)malloc(sizeof(EdgeNode));
				p->adjvertex = (i + 1)*mg->colNum + j;
				p->next = G->adjlist[i*mg->colNum + j].firstnode;
				G->adjlist[i*mg->colNum + j].firstnode = p;
			}
			if (j - 1 >= 0 && mg->arcs[i][j-1] == 0)
			{
				p = (EdgeNode *)malloc(sizeof(EdgeNode));
				p->adjvertex = i * mg->colNum + j - 1;
				p->next = G->adjlist[i*mg->colNum + j].firstnode;
				G->adjlist[i*mg->colNum + j].firstnode = p;
			}
			if (j + 1 <=mg->colNum && mg->arcs[i][j + 1] == 0)
			{
				p = (EdgeNode *)malloc(sizeof(EdgeNode));
				p->adjvertex = i * mg->colNum + j + 1;
				p->next = G->adjlist[i*mg->colNum + j].firstnode;
				G->adjlist[i*mg->colNum + j].firstnode = p;
			}
			if (i - 1 >= 0 && j - 1 >=0 && mg->arcs[i-1][j - 1] == 0)
			{
				p = (EdgeNode *)malloc(sizeof(EdgeNode));
				p->adjvertex = (i - 1) * mg->colNum + j - 1;
				p->next = G->adjlist[i*mg->colNum + j].firstnode;
				G->adjlist[i*mg->colNum + j].firstnode = p;
			}
			if (i - 1 >= 0 && j + 1 <=mg->colNum && mg->arcs[i - 1][j + 1] == 0)
			{
				p = (EdgeNode *)malloc(sizeof(EdgeNode));
				p->adjvertex = (i - 1) * mg->colNum + j + 1;
				p->next = G->adjlist[i*mg->colNum + j].firstnode;
				G->adjlist[i*mg->colNum + j].firstnode = p;
			}
			if (i + 1 <= mg->rowNum && j - 1 >= 0 && mg->arcs[i + 1][j - 1] == 0)
			{
				p = (EdgeNode *)malloc(sizeof(EdgeNode));
				p->adjvertex = (i + 1) * mg->colNum + j - 1;
				p->next = G->adjlist[i*mg->colNum + j].firstnode;
				G->adjlist[i*mg->colNum + j].firstnode = p;
			}
			if (i + 1 <= mg->rowNum && j + 1 <= mg->colNum && mg->arcs[i + 1][j + 1] == 0)
			{
				p = (EdgeNode *)malloc(sizeof(EdgeNode));
				p->adjvertex = (i + 1) * mg->colNum + j + 1;
				p->next = G->adjlist[i*mg->colNum + j].firstnode;
				G->adjlist[i*mg->colNum + j].firstnode = p;
			}
		}
	return G;
}
void TraverALGraph(ALGraph *G, int record[MaxVertexNum], int entry, int exit)
/*���ڽӱ���й�����ȱ����������entry��ʼ��������exit�ǽ����㷨����record������ؽ��*/
{
	EdgeNode *p;
	int visited[MaxVertexNum];
	for (int i = 0; i < G->vertexNum; i++)
	{
		visited[i] = 0;
		record[i] = -1;
	}/*��ʼ����־���飬һ��ʼֵrecord����ֵΪ-1�����ʱ�־����ֵ��Ϊ0*/
	int u, w, front, rear, Queue[MaxVertexNum];/*�����������*/
	front = rear = -1;
	visited[entry] = 1;
	Queue[(++rear) % MaxVertexNum] = entry;/*v�����*/
	while (rear != front)
	{
		u = Queue[(++front) % MaxVertexNum];/*������*/
		for (p = G->adjlist[u].firstnode; p; p = p->next)
		{
			w = p->adjvertex;
			if (visited[w]==0)
			{
				visited[w] = 1;
				record[w] = Queue[front];/*��־�����¼���ʽ�����ڽӱ�adjlist����λ��*/
				Queue[(++rear) % MaxVertexNum] = w;/*u����δ���ʵ��ڽӽ��w�����*/
			}
			if (p->adjvertex == exit)/*�������־���ڳ���exit��ʹ��goto�������㷨*/
				goto end;
		}
	}
end:;
}
void output(int record[], int entry, int exit,ALGraph *G)
/*�������ϻ��ݷ���ӡ�Թ�·��*/
{
	int i = 0, t;
	if (entry == exit && G->adjlist[exit].data == 0)
		printf("���Ѿ���һ������!\n");
	else
		if (record[exit] != -1)
		{
			t = exit;
			{printf("���Թ��г���,�Թ����·���������:\n");
			while (t != entry)
			{
				printf("[%d,%d]<-", t / G->colNum + 1, t%G->colNum + 1);
				t = record[t];
				if (++i % 5 == 0)
					printf("\n");
			}
			printf("[%d,%d]\n", t / G->colNum + 1, t%G->colNum + 1);
			}
		}
		else
			printf("û��·����ڴ�!\n");
}
int main()
{
	int entry_row, entry_col, exit_row, exit_col, entry, exit;
	int record[MaxVertexNum];
	MGraph mg;
	CreateMGraph(&mg);
	ALGraph *G = MatrixtToList(&mg);
	printf("�������Թ���ڴ����꣨*��*����");
	scanf("%d,%d", &entry_row, &entry_col);
	printf("�������Թ����ڴ����꣨*��*����");
	scanf("%d,%d", &exit_row, &exit_col);
	entry = (--entry_row)*(G->colNum) + (--entry_col);
	exit = (--exit_row)*(G->colNum) + (--exit_col);
    TraverALGraph(G,record, entry, exit);
    output(record, entry, exit,G);
}
