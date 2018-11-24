//数独计算器 by liuweilhy 2012年7月6日
#ifndef SUDOKU
#define SUDOKU

#include<memory.h>

//定义行列数
#define N 9
//定义单宫行列数
#define M 3
//定义数独有唯一解的最少提示数目，3阶数独（9×9）的最少提示数目为17
#define CLUE_MIN 17

//定义节点，双向链表结构为方便插入法排序
struct node
{
    char i;              //节点所在行
    char j;              //节点所在列
    char candidate[N];   //定义候选数
    int num;             //候选数目
    struct node* prev;   //指向前驱节点的指针
    struct node* next;   //指向后续节点的指针
};

//定义“数独”类
class Sudoku
{
private:
    char clue[N][N];      //数独的初始状态，为0表示“空”
    char solution[N][N];  //数独的解
    char temp[N][N];      //数独的解的临时存放数组
    unsigned long num;    //数独的解的个数
    unsigned long n;      //数独第n个解
    struct node *head;    //指向节点链表首地址的指针
    struct node *end;     //指向节点链表尾地址的指针
    int flag;             //数独状态标示

private:
    // 初始化节点链表
    bool initializeTable();
    // 删除链表
    void deleteTable();
    // 插入法排序单个节点
    void insert(struct node *current);
    // 插入法排序整个节点链表
    void insert();
    // 摒除法清理链表
    bool renounce();
    //检验是否有重复
    bool isRepeted(char, int, int, char[N][N]);
    //递归回溯求解，能求解所有
    bool recursion(struct node *);

public:
    Sudoku(char clue[N][N] = nullptr);
    ~Sudoku();
    // 初始化数独
    void initialize(char clue[N][N] = nullptr);
    // 输出初始值或计算结果
    char (* output(char output[N][N], bool who = true))[N];
    // 数独初始化后的合法性检查
    int check();
    // 解数独，n为0返回解的总数，n为非0返回n（如果第n个解存在）
    unsigned long solve(unsigned long n = 0ul);
    // 显示链表，调试程序用
    // friend void outTable(Sudoku it);
};

#endif
