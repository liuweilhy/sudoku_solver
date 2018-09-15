#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
#include "sudoku.h"

Sudoku::Sudoku(char clue[N][N] /*= NULL*/)
{
	head = end = NULL;
	num = n = 0;
	Initialize(clue);
}

Sudoku::~Sudoku()
{
	DeleteTable();
}

//用提示数初始化数独
void Sudoku::Initialize(char clue[N][N] /*= NULL*/)
{
	if (clue)
	{
		memcpy(this->clue, clue, sizeof(this->clue));
		flag = Check();
	}
	else
	{
		memset(this->clue, 0, sizeof(this->clue));
		flag = 0;
	}
	memcpy(solution, this->clue, sizeof(solution));
}

//输出初始值或计算结果
//who为真返回结果，为假返回初始值
//使用二维数组作为返回值的作法欠妥当，但的确很方便
char (* Sudoku::Output(char output[N][N], bool who /*= true*/))[N]
{
	if(who)
		memcpy(output, solution, sizeof(solution));
	else
		memcpy(output, clue, sizeof(clue));
	return output;
}

//数独的合法性检查
//返回值：1表示合法，0表示当前提示数小于最少提示数，
//返回值：-1表示有非法数，-2表示数独未初始化，-3表示有重复提示
int Sudoku::Check()
{
	int iLegal = 0;	//合法提示数的个数
	if (clue == NULL)
		return -2;
	for (int i=0; i<N; i++)
	{
		for (int j=0; j<N; j++)
		{
			//有非法数
			if(clue[i][j]<0 || clue[i][j]>N)
				return -1;
			if(clue[i][j]>0 && clue[i][j]<=N)
			{
				//有重复数
				if(Isrepetitive(clue[i][j],i,j,clue))
					return -3;
				//合法提示数个数加1
				iLegal++;
			}
		}
	}
	if (iLegal>=CLUE_MIN)
		return 1;	//如果合法提示数的个数不小于最少提示数目，则数独合法
	return 0;
}

bool Sudoku::InitializeTable()		//初始化节点链表
{
	struct node *p = NULL;			//临时节点指针
	head = end = new struct node;	//建立一个空节点
	head->next = NULL;
	head->prev = NULL;
	for (int i=0; i<N; i++)
	{
		for (int j=0; j<N; j++)
		{
			if(clue[i][j] != 0)
				continue;
			//如果该位置为空，建立链表
			//将该位置候选数设为1～N
			p = new struct node;
			p->i = i;
			p->j = j;
			p->num = N;
			for(int k=0; k<N; k++)
				p->candidate[k] = k+1;
			//链接链表
			end->next = p;
			p->prev = end;
			p->next = NULL;
			end = p;
		}
	}
	//重新调整链表的头指针，删除首空节点
	p = head;
	head = head->next;
	delete p;
	if(head == NULL)
		return false;
	else
		head->prev = NULL;
	return true;
}

//删除链表
void Sudoku::DeleteTable()
{
	struct node *p = head;
	while(head)
	{
		p = head->next;
		delete head;
		head = p;
	}
	end = NULL;
}

//插入法排序单个节点，在当前位置向前插入
//候选值多的排在后面
void Sudoku::Insert(struct node* current)
{
	//不对空节点和head节点排序
	if(!current || !(current->prev))
		return;
	struct node *af = current->prev;	//插入节点指针
	struct node *pr = current->prev;	//前驱节点指针
	struct node *ne = current->next;	//后继节点指针
	//排在后面
	while(af)
	{
		//若前驱节点的候选数不大于当前节点候选数
		if(af->num <= current->num)
		{
			//如果节点没动，则返回
			if(af==pr)
				return;
			else
				break;
		}
		else
			af = af->prev;
	}
	//将当前节点至于前驱节点之后
	pr->next = current->next;
	if(ne)
		ne->prev = current->prev;
	else
		end = pr;
	current->prev = af;
	if(af)
	{
		current->next = af->next;
		af->next->prev =current;
		af->next = current;
	}
	else
	{
		current->next = head;
		head->prev = current;
		head = current;
	}
}

//插入法排序整个链表
//候选值多的排在后面
void Sudoku::Insert()
{
	struct node *p = head->next;
	while(p)
	{
		//依次排序
		Insert(p);
		p=p->next;
	}
}

//摒除法清理链表
bool Sudoku::Renounce()
{
	struct node *p = head;
	char ip, jp;
	while(p)
	{
		ip = p->i;
		jp = p->j;
		//行摒除
		for (int j=0; j<N; j++)
		{
			if(clue[ip][j] && p->candidate[clue[ip][j]-1])
			{
				p->candidate[clue[ip][j]-1] = 0;
				p->num--;
			}
		}
		//列摒除
		for (int i=0; i<N; i++)
		{
			if(clue[i][jp] && p->candidate[clue[i][jp]-1])
			{
				p->candidate[clue[i][jp]-1] = 0;
				p->num--;
			}
		}
		//宫摒除
		for (int i=ip-ip%M; i<ip-ip%M+M; i++)
		{
			for (int j=jp-jp%M; j<jp-jp%M+M; j++)
				if(clue[i][j] && p->candidate[clue[i][j]-1])
				{
					p->candidate[clue[i][j]-1] = 0;
					p->num--;
				}
		}
		//如果数被摒除没了
		if(p->num<=0)
			return false;
		p=p->next;
	}
	return true;
}

//检验是否有重复
bool Sudoku::Isrepetitive(char a, int ip, int jp, char array[N][N])
{
	//检查行
	for (int j=0; j<N; j++)
	{
		if(array[ip][j] == a && j!=jp)
			return true;
	}
	//检查列
	for (int i=0; i<N; i++)
	{
		if(array[i][jp] == a && i!=ip)
			return true;
	}
	//检查宫
	for (int i=ip-ip%M; i<ip-ip%M+M; i++)
	{
		for (int j=jp-jp%M; j<jp-jp%M+M; j++)
			if(array[i][j] == a && i!=ip && j!=jp)
				return true;
	}
	return false;
}

//递归求解方法一，只求解一个，速度较快
/*bool Sudoku::Recursion(struct node *p)
{
//如果链表已遍历结束，则返回true
if(p==NULL)
{
num = 1ul;
return true;
}
//递归启动
for(int i=0; i<N; i++)
{
if(p->candidate[i]==0)
continue;
if(Isrepetitive(p->candidate[i],p->i,p->j,solution))
continue;
solution[p->i][p->j]=p->candidate[i];
if(Recursion(p->next))
return true;
else
solution[p->i][p->j]=0;
}
return false;
}
*/

//递归求解方法二，求解所有解，速度较慢
//当参数n为0时，可以得出解的个数（不超过unsigned long上限）
//当n为非0时，可以得出第n个解
bool Sudoku::Recursion(struct node *p)
{
	//如果链表已遍历结束
	if(p==NULL)
	{
		//解的数目+1
		num++;
		//临时数组赋值给解
		memcpy(solution, temp, sizeof(solution));
		if(num == n)
		{
			if(num == 0ul)
				num--;
			return true;
		}
		else
			return false;
	}
	//递归启动
	for(int i=0; i<N; i++)
	{
		if(p->candidate[i]==0)
			continue;
		if(Isrepetitive(p->candidate[i],p->i,p->j,temp))
			continue;
		temp[p->i][p->j]=p->candidate[i];
		if(Recursion(p->next))
			return true;
		else
			temp[p->i][p->j]=0;
	}
	return false;
}

//用上述两种方法求解数独
//参数n为0时，返回解的总数，总数不超过unsigned long的最大值
//参数n为非0时，如果第n个解存在则返回n，否则返回解的总数
unsigned long Sudoku::Solve(unsigned long n /*= 0ul*/)
{
	//flag值为-1表示提示数中有非法数字，0表示小于最少提示数，1表示成功
	if (flag < 0)
		return 0;
	//初始化链表
	if(!InitializeTable())
		return 0;
	//摒除法清理链表
	Renounce();
	//插入法排序
	Insert();
	//清空解的个数
	num = 0ul;
	//置n值
	this->n = n;
	//把解调至初始状态
	memcpy(solution, clue, sizeof(solution));
	memcpy(temp, clue, sizeof(temp));
	//递归求解
	Recursion(head);
	//返回解的个数
	return num;
}
