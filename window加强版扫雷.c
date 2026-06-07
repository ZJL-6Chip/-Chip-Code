#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>

#define MAX_ROW 24
#define MAX_COL 30
#define RECORD_FILE "minesweeper_records.txt"

/* 游戏格子结构体 */
typedef struct {
    int isMine;
    int isRevealed;
    int isFlagged;
    int neighborMines;
} Cell;

/* 游戏难度配置 */
typedef struct {
    int row;
    int col;
    int mines;
    char name[10];
} Difficulty;

/* 全局变量定义 */
Cell board[MAX_ROW][MAX_COL];
int curRow, curCol, totalMines;
int cursorX = 0, cursorY = 0;
int gameState = 0;  /* 0: 游戏中, 1: 胜利, -1: 失败 */
int firstClick = 1; /* 首次点击保护 */
int flagsCount = 0;

/* 计时相关 */
DWORD startTime;
int elapsedTime = 0;

Difficulty levels[3] = {
    {9, 9, 10, "初级"},
    {16, 16, 40, "中级"},
    {16, 30, 99, "高级"}
};

/* 隐藏控制台光标 */
void hideCursor() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

/* 改变控制台文本颜色 */
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

/* 难度选择界面 */
int selectDifficulty() {
    int choice = 1;
    char ch;
    
    system("cls");
    printf("=================================\n");
    printf("      欢迎来到经典标准扫雷       \n");
    printf("=================================\n");
    printf("请选择游戏难度:\n\n");
    printf(" 1. 初级 ( 9 x  9 , 10个雷)\n");
    printf(" 2. 中级 (16 x 16 , 40个雷)\n");
    printf(" 3. 高级 (16 x 30 , 99个雷)\n\n");
    printf("请输入选项 (1-3): ");
    
    while (1) {
        ch = getch();
        if (ch >= '1' && ch <= '3') {
            choice = ch - '1';
            break;
        }
    }
    curRow = levels[choice].row;
    curCol = levels[choice].col;
    totalMines = levels[choice].mines;
    return choice;
}

/* 基础地图初始化 */
void initBoard() {
    int i, j;
    for (i = 0; i < curRow; i++) {
        for (j = 0; j < curCol; j++) {
            board[i][j].isMine = 0;
            board[i][j].isRevealed = 0;
            board[i][j].isFlagged = 0;
            board[i][j].neighborMines = 0;
        }
    }
    cursorX = curCol / 2;
    cursorY = curRow / 2;
    gameState = 0;
    firstClick = 1;
    flagsCount = 0;
    elapsedTime = 0;
}

/* 生成雷区（在第一次点击后调用，确保首发安全） */
void generateMines(int startY, int startX) {
    int planted = 0;
    int i, j, dr, dc;
    
    while (planted < totalMines) {
        int r = rand() % curRow;
        int c = rand() % curCol;
        
        /* 不能在首踩点及周围8个格子上生成雷 */
        if (abs(r - startY) <= 1 && abs(c - startX) <= 1) continue;
        
        if (!board[r][c].isMine) {
            board[r][c].isMine = 1;
            planted++;
        }
    }

    /* 计算数字 */
    for (i = 0; i < curRow; i++) {
        for (j = 0; j < curCol; j++) {
            if (board[i][j].isMine) continue;
            int count = 0;
            for (dr = -1; dr <= 1; dr++) {
                for (dc = -1; dc <= 1; dc++) {
                    int nr = i + dr, nc = j + dc;
                    if (nr >= 0 && nr < curRow && nc >= 0 && nc < curCol && board[nr][nc].isMine) {
                        count++;
                    }
                }
            }
            board[i][j].neighborMines = count;
        }
    }
    startTime = GetTickCount(); /* 正式开始计时 */
}

/* 刷新渲染界面 */
void drawBoard() {
    COORD coord = {0, 0};
    int i, j;
    int colors[] = {0, 9, 10, 12, 13, 14, 11, 4, 5};
    
    /* 将光标重置到控制台左上角，避免旧版环境下闪烁 */
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);

    /* 计算当前耗时 */
    if (gameState == 0 && !firstClick) {
        elapsedTime = (GetTickCount() - startTime) / 1000;
        if (elapsedTime > 999) elapsedTime = 999;
    }

    /* 顶部状态栏 */
    setColor(11);
    printf("===================================================\n");
    setColor(7);
    printf("  [ 🚩 剩余雷数: %02d ]          [ ⏱️ 游戏时间: %03d 秒 ]  \n", totalMines - flagsCount, elapsedTime);
    setColor(11);
    printf("===================================================\n\n");
    setColor(7);

    /* 绘制棋盘 */
    for (i = 0; i < curRow; i++) {
        for (j = 0; j < curCol; j++) {
            if (i == cursorY && j == cursorX) {
                setColor(11); /* 青色光标 */
                printf("[");
            } else {
                printf(" ");
            }

            if (board[i][j].isRevealed) {
                if (board[i][j].isMine) {
                    setColor(12); printf("*"); 
                } else if (board[i][j].neighborMines == 0) {
                    setColor(8); printf(".");  
                } else {
                    setColor(colors[board[i][j].neighborMines]);
                    printf("%d", board[i][j].neighborMines);
                }
            } else if (board[i][j].isFlagged) {
                setColor(14); printf("F"); 
            } else {
                setColor(7); printf("#");  
            }

            if (i == cursorY && j == cursorX) {
                setColor(11); printf("]");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
    printf("\n");
    setColor(8);
    printf("操作说明: WASD移动 | 键入[Space]翻开 | 键入[F]插旗\n");
    setColor(7);
}

/* 连通区连锁展开 (DFS) */
void revealCell(int r, int c) {
    int dr, dc;
    if (r < 0 || r >= curRow || c < 0 || c >= curCol || board[r][c].isRevealed || board[r][c].isFlagged) return;

    board[r][c].isRevealed = 1;

    if (board[r][c].neighborMines == 0 && !board[r][c].isMine) {
        for (dr = -1; dr <= 1; dr++) {
            for (dc = -1; dc <= 1; dc++) {
                revealCell(r + dr, c + dc);
            }
        }
    }
}

/* 检查是否获胜 */
void checkWin() {
    int safeUnrevealed = 0;
    int i, j;
    for (i = 0; i < curRow; i++) {
        for (j = 0; j < curCol; j++) {
            if (!board[i][j].isMine && !board[i][j].isRevealed) {
                safeUnrevealed++;
            }
        }
    }
    if (safeUnrevealed == 0) gameState = 1;
}

/* 游戏失败时翻开全盘 */
void revealAllMines() {
    int i, j;
    for (i = 0; i < curRow; i++) {
        for (j = 0; j < curCol; j++) {
            if (board[i][j].isMine) board[i][j].isRevealed = 1;
        }
    }
}

/* 排行榜系统 */
void handleLeaderboard(int levelIdx, int score) {
    char name[20];
    FILE *file;
    
    if (score >= 999) return;
    
    system("cls");
    setColor(10);
    printf("🎉 新纪录！你成功通关了%s难度！\n", levels[levelIdx].name);
    printf("你的成绩是: %d 秒\n\n", score);
    setColor(7);
    
    printf("请输入你的大名留在排行榜: ");
    scanf("%19s", name);

    /* 追加到文件 */
    file = fopen(RECORD_FILE, "a");
    if (file != NULL) {
        fprintf(file, "%s %s %d\n", levels[levelIdx].name, name, score);
        fclose(file);
    }

    /* 显示当前难度的历史记录 */
    printf("\n🏆 [%s难度] 历史荣誉榜:\n", levels[levelIdx].name);
    printf("---------------------------------\n");
    
    file = fopen(RECORD_FILE, "r");
    if (file != NULL) {
        char lName[20], pName[20];
        int lScore;
        int rank = 1;
        while (fscanf(file, "%s %s %d", lName, pName, &lScore) != EOF) {
            if (strcmp(lName, levels[levelIdx].name) == 0) {
                printf(" 第 %d 名: %s - %d秒\n", rank++, pName, lScore);
                if(rank > 5) break; 
            }
        }
        fclose(file);
    }
    printf("---------------------------------\n");
    printf("按任意键退出游戏...");
    getch();
}

int main() {
    int levelIdx;
    
    srand((unsigned int)time(NULL));
    hideCursor();

    levelIdx = selectDifficulty();
    initBoard();
    system("cls"); 

    /* 核心游戏循环 */
    while (gameState == 0) {
        drawBoard();
        
        if (kbhit()) {
            char ch = getch();
            switch (ch) {
                case 'w': case 'W': if (cursorY > 0) cursorY--; break;
                case 's': case 'S': if (cursorY < curRow - 1) cursorY++; break;
                case 'a': case 'A': if (cursorX > 0) cursorX--; break;
                case 'd': case 'D': if (cursorX < curCol - 1) cursorX++; break;
                case 'f': case 'F':
                    if (!board[cursorY][cursorX].isRevealed) {
                        board[cursorY][cursorX].isFlagged = !board[cursorY][cursorX].isFlagged;
                        flagsCount += board[cursorY][cursorX].isFlagged ? 1 : -1;
                    }
                    break;
                case ' ':
                    if (!board[cursorY][cursorX].isFlagged && !board[cursorY][cursorX].isRevealed) {
                        if (firstClick) {
                            generateMines(cursorY, cursorX);
                            firstClick = 0;
                        }

                        if (board[cursorY][cursorX].isMine) {
                            gameState = -1;
                            revealAllMines();
                        } else {
                            revealCell(cursorY, cursorX);
                            checkWin();
                        }
                    }
                    break;
            }
        }
        Sleep(50); 
    }

    /* 游戏结束处理 */
    drawBoard();
    if (gameState == 1) {
        handleLeaderboard(levelIdx, elapsedTime);
    } else {
        setColor(12);
        printf("\n💥 轰！你踩到雷了。游戏结束！\n");
        setColor(7);
        printf("按任意键退出...");
        getch();
    }

    return 0;
}