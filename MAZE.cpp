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
typedef struct {//邻接表结构体，结构内加入行数，列数便于统计迷宫
	VertexNode adjlist[MaxVertexNum];
	int vertexNum;
	int rowNum;
	int colNum;
}ALGraph;
typedef struct {//邻接矩阵存储结构，以邻接矩阵形式存储迷宫
	int arcs[MaxVertexNum][MaxVertexNum];
	int rowNum;
	int colNum;
}MGraph;
/*创建一个邻接矩阵形式的迷宫，0表示此路可通，1表示此路不通*/
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
/*把邻接矩阵转化为邻接表，通过邻接表的遍历实现迷宫路径查询*/
ALGraph *MatrixtToList(MGraph *mg)
{
	int i, j;
	ALGraph *G;
	G = (ALGraph *)malloc(sizeof(ALGraph));
	EdgeNode *p;/*表阵结点，行数，列数相同*/
	G->vertexNum = (mg->colNum)*(mg->rowNum);
	G->colNum = mg->colNum;
	G->rowNum = mg->rowNum;
	for (i = 0; i<G->vertexNum; i++)/*图中结点的初始化*/
	{
		G->adjlist[i].data =mg->arcs[i / mg->colNum][i%mg->colNum];
		G->adjlist[i].firstnode = NULL;
	}
	for(i=0;i<mg->rowNum;i++)/*从该结点的八个方向进行搜索，分别建立其邻接表结构*/
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
/*对邻接表进行广度优先遍历，从入口entry开始，到出口exit是结束算法，有record数组带回结果*/
{
	EdgeNode *p;
	int visited[MaxVertexNum];
	for (int i = 0; i < G->vertexNum; i++)
	{
		visited[i] = 0;
		record[i] = -1;
	}/*初始化标志数组，一开始值record数组值为-1，访问标志数组值置为0*/
	int u, w, front, rear, Queue[MaxVertexNum];/*定义队列数组*/
	front = rear = -1;
	visited[entry] = 1;
	Queue[(++rear) % MaxVertexNum] = entry;/*v入队列*/
	while (rear != front)
	{
		u = Queue[(++front) % MaxVertexNum];/*出队列*/
		for (p = G->adjlist[u].firstnode; p; p = p->next)
		{
			w = p->adjvertex;
			if (visited[w]==0)
			{
				visited[w] = 1;
				record[w] = Queue[front];/*标志数组记录访问结点在邻接表adjlist数组位置*/
				Queue[(++rear) % MaxVertexNum] = w;/*u的尚未访问的邻接结点w入队列*/
			}
			if (p->adjvertex == exit)/*如果结点标志等于出口exit，使用goto语句结束算法*/
				goto end;
		}
	}
end:;
}
void output(int record[], int entry, int exit,ALGraph *G)
/*根据书上回溯法打印迷宫路径*/
{
	int i = 0, t;
	if (entry == exit && G->adjlist[exit].data == 0)
		printf("它已经是一个出口!\n");
	else
		if (record[exit] != -1)
		{
			t = exit;
			{printf("该迷宫有出口,迷宫最短路径输出如下:\n");
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
			printf("没有路径入口处!\n");
}
int main()
{
	int entry_row, entry_col, exit_row, exit_col, entry, exit;
	int record[MaxVertexNum];
	MGraph mg;
	CreateMGraph(&mg);
	ALGraph *G = MatrixtToList(&mg);
	printf("请输入迷宫入口处坐标（*，*）：");
	scanf("%d,%d", &entry_row, &entry_col);
	printf("请输入迷宫出口处坐标（*，*）：");
	scanf("%d,%d", &exit_row, &exit_col);
	entry = (--entry_row)*(G->colNum) + (--entry_col);
	exit = (--exit_row)*(G->colNum) + (--exit_col);
    TraverALGraph(G,record, entry, exit);
    output(record, entry, exit,G);
}
