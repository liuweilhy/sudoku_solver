﻿#include "sudoku.h"

Sudoku::Sudoku(char clue[N][N] /*= nullptrptr*/)
{
    head = end = nullptr;
    num = n = 0;
    initialize(clue);
}

Sudoku::~Sudoku()
{
    deleteTable();
}

//用提示数初始化数独
void Sudoku::initialize(char clue[N][N] /*= nullptrptr*/)
{
    if (clue)
    {
        memcpy(this->clue, clue, sizeof(this->clue));
        flag = check();
    }
    // 如果用空指针初始化，则全部置0
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
char (* Sudoku::output(char output[N][N], bool who /*= true*/))[N]
{
    if(who)
        memcpy(output, solution, sizeof(solution));
    else
        memcpy(output, clue, sizeof(clue));
    return output;
}

//数独初始化后的合法性检查
//返回值：1表示合法，0表示当前提示数小于最少提示数，
//返回值：-1表示有非法数，-2表示数独未初始化，-3表示有重复提示
int Sudoku::check()
{
    int iLegal = 0;    //合法提示数的个数
    if (clue == nullptr)
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
                if(isRepeted(clue[i][j],i,j,clue))
                    return -3;
                //合法提示数个数加1
                iLegal++;
            }
        }
    }
    if (iLegal>=CLUE_MIN)
        return 1;    //如果合法提示数的个数不小于最少提示数目，则数独合法
    return 0;
}

//初始化节点链表
bool Sudoku::initializeTable()
{
    struct node *p = nullptr;        //临时节点指针
    head = end = new struct node;    //建立一个空节点
    head->next = nullptr;
    head->prev = nullptr;
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
            p->prev = end;
            p->next = nullptr;
            end->next = p;
            end = p;
        }
    }
    //重新调整链表的头指针，删除首空节点
    p = head;
    head = head->next;
    delete p;
    if(head == nullptr)
        return false;
    else
        head->prev = nullptr;
    return true;
}

//删除链表
void Sudoku::deleteTable()
{
    struct node *p = head;
    while(head)
    {
        p = head->next;
        delete head;
        head = p;
    }
    end = nullptr;
}

//摒除法清理链表
bool Sudoku::renounce()
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
bool Sudoku::isRepeted(char a, int ip, int jp, char array[N][N])
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

//递归回溯算法，求解所有解，速度较慢
//当参数n为0时，可以得出解的个数（不超过unsigned long上限）
//当n为非0时，可以得出第n个解
bool Sudoku::recursion(struct node *p)
{
    // 设置递归终止条件
    if(p==nullptr)
    {
        // 解的数目+1
        num++;
        // 临时数组赋值给解
        memcpy(solution, temp, sizeof(solution));
        // 判断当前解是不是要求的第n个解，是则终止递归
        if(num == n)
        {
            return true;
        }
        // 否则继续寻找下一个解
        else
            return false;
    }
    // 递归启动
    for(int i=0; i<N; i++)
    {
        if(p->candidate[i]==0)
            continue;
        if(isRepeted(p->candidate[i],p->i,p->j,temp))
            continue;
        temp[p->i][p->j]=p->candidate[i];
        if(recursion(p->next))
            return true;
        else
            temp[p->i][p->j]=0;
    }
    // 回溯
    return false;
}

//参数n为0时，返回解的总数，总数不超过unsigned long的最大值
//参数n为非0时，如果第n个解存在则返回n，否则返回解的总数
unsigned long Sudoku::solve(unsigned long n /*= 0ul*/)
{
    //flag值为-1表示提示数中有非法数字，0表示小于最少提示数，1表示成功
    if (flag < 0)
        return 0;
    //初始化链表
    if(!initializeTable())
        return 0;
    //摒除法清理链表
    renounce();
    //清空解的个数
    num = 0ul;
    //置n值
    this->n = n;
    //把解调至初始状态
    memcpy(solution, clue, sizeof(solution));
    memcpy(temp, clue, sizeof(temp));
    //递归求解
    recursion(head);
    //返回解的个数
    return num;
}
