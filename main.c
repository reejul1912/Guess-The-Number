#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <conio.h>
#include <string.h>

#pragma comment(lib,"winmm.lib")

static void window_size()
{
    SetConsoleTitle("Guess The Number!");
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = {0, 0, 49, 14};
    COORD bufferSize = {49, 14};
    SetConsoleScreenBufferSize(hOut, bufferSize);
    SetConsoleWindowInfo(hOut, TRUE, &windowSize);
    HWND console = GetConsoleWindow();
    long style = GetWindowLong(console, GWL_STYLE);
    style &= ~WS_MAXIMIZEBOX;
    style &= ~WS_THICKFRAME;
    SetWindowLong(console, GWL_STYLE, style);
}

static void text_size(int size)
{
    HANDLE text = GetStdHandle(STD_OUTPUT_HANDLE);   
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 0;   
    cfi.dwFontSize.Y = size;  
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Consolas");
    SetCurrentConsoleFontEx(text, FALSE, &cfi);
}

static void load_sounds()
{
    mciSendString("close all", NULL, 0, NULL);
    
    mciSendString("open \"bg_music.mp3\" type mpegvideo alias bgm", NULL, 0, NULL);
    mciSendString("open \"win.mp3\" type mpegvideo alias win", NULL, 0, NULL);
    mciSendString("open \"lose.mp3\" type mpegvideo alias lose", NULL, 0, NULL);
    mciSendString("open \"menu.mp3\" type mpegvideo alias menu_click", NULL, 0, NULL);
}

static void play_sound(const char* action)
{
    if (strcmp(action, "menu_click") == 0) {
        mciSendString("play menu_click from 0", NULL, 0, NULL);
    }
    else if (strcmp(action, "start_bgm") == 0) {
        mciSendString("play bgm repeat", NULL, 0, NULL);
    }
    else if (strcmp(action, "stop_bgm") == 0) {
        mciSendString("stop bgm", NULL, 0, NULL);
    }
    else if (strcmp(action, "win") == 0) {
        mciSendString("play win from 0", NULL, 0, NULL);
    }
    else if (strcmp(action, "lose") == 0) {
        mciSendString("play lose from 0", NULL, 0, NULL);
    }
}

static void show_game_menu()
{
    HANDLE text = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(text, 9);
    printf("\n\n\n");
    printf("\t   ~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
    printf("\t        GUESS THE NUMBER!\n");
    printf("\t   ~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
    SetConsoleTextAttribute(text, 11);
    printf("\t     [Press any key to play]\n");
    SetConsoleTextAttribute(text, 7);
    printf("\n\n\t    Version 1.0 by Reejul Kant");
    _getch();
    play_sound("menu_click");
}

static int difficulty_menu()
{
    int choose;
    
    while(1)
    {
        HANDLE text = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(text, 11);
        printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
        printf(" Choose Difficulty:\n");
        printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
        SetConsoleTextAttribute(text, 10);
        printf(" [1] Easy (Any number from 1 to 50)\n");
        SetConsoleTextAttribute(text, 12);
        printf(" [2] Hard (Any number from 1 to 200)\n");
        SetConsoleTextAttribute(text, 7);
        printf(" Enter 1 or 2: ");
        
        if (scanf("%d", &choose) == 1 && (choose == 1 || choose == 2))
        {
            play_sound("menu_click");
            break;
        }
        else
        {
            printf("\nPlease choose either 1 or 2!");
            while(getchar() != '\n');
            system("cls");
        }
    }
    return choose; 
}

static void win_animation()
{
    const char* line = "~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n";
    int line_size = strlen(line);
    
    for(int i = 0; i < line_size; i++)
    {
        printf("%c", line[i]);
        fflush(stdout);
        Sleep(10);
    }
}

int main(void)
{
    char replay;
    HANDLE text = GetStdHandle(STD_OUTPUT_HANDLE);
    srand(time(NULL));
    
    window_size();
    text_size(24);
    load_sounds();
    
    do 
    {
        system("cls");
        play_sound("start_bgm");
        show_game_menu();
        Sleep(300); 
    
        int number = 0, guess = 0, diff = 0, attempt = 0, lost = 0;
    
        system("cls");    
        int difficulty = difficulty_menu();
        
        if (difficulty == 1)
        {
            number = rand() % 50 + 1;
        }
        else if (difficulty == 2)
        {
            number = rand() % 200 + 1; 
        }
        
        while(1)
        {
            system("cls");
            
            SetConsoleTextAttribute(text, 7);
            printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
            printf(" I'm thinking of a number. Can you guess it?\n");
            printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
            
            SetConsoleTextAttribute(text, 11);
            printf(" Guesses left: %d\n", 10 - attempt);
            
            if (attempt >= 4 && attempt < 7) 
            {
                SetConsoleTextAttribute(text, 14); 
                printf(" Hehe. You can't beat me!\n\n");
            }
            else if (attempt >= 7 && attempt < 9)
            {
                SetConsoleTextAttribute(text, 14);
                printf(" Give up already...\n\n");
            }
            else if (attempt == 9) 
            {
                SetConsoleTextAttribute(text, 12); 
                printf(" LAST CHANCE!\n\n");
            }
            else
            {
                printf("\n\n");
            }
            
            SetConsoleTextAttribute(text, 7);
            printf(" Enter: ");
            if (scanf("%d", &guess) == 1)
            {
            	attempt++;
                play_sound("menu_click");
                diff = number - guess;
                
                if (diff == 0)
                {
                    play_sound("stop_bgm");
                    play_sound("win");
                    
                    system("cls");
                    SetConsoleTextAttribute(text, 10);
                    win_animation();
                    printf(" You guessed it right. It is %d :)", number);
                    break;
                }
                
                if (attempt >= 10 && diff != 0)
                {
                    lost = 1;
                    break;
                }
        
                SetConsoleTextAttribute(text, 12); 
                if (diff > 0)
                {
                    if (diff == 1)                     printf(" You are VERY close!! Almost there! ");    
                    else if (diff > 1 && diff <= 5)    printf(" You're very close!! Go higher! ");
                    else if (diff > 5 && diff <= 10)   printf(" A bit low. Getting close! Go higher! ");
                    else if (diff > 10 && diff <= 20)  printf(" That's low! Pretty far. Go higher! ");
                    else                               printf(" Too low!! TOO far! Go higher! ");
                }
                else if (diff < 0)
                {
                    if (diff == -1)                      printf(" You are VERY close!! Almost there! ");    
                    else if (diff < -1 && diff >= -5)    printf(" You're very close!! Go lower! ");
                    else if (diff < -5 && diff >= -10)   printf(" A bit high! Getting close! Go lower! ");
                    else if (diff < -10 && diff >= -20)  printf(" That's high! Pretty far. Go lower! ");
                    else                                 printf(" Too high! TOO far! Go lower! ");
                }   
                Sleep(2000);
            }
            else
            {
                while(getchar() != '\n');
            }
        }
        
        play_sound("stop_bgm");
        
        if (lost)
        {
            play_sound("lose"); 
            SetConsoleTextAttribute(text, 12); 
            system("cls");
            printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-\n");
            printf(" GAME OVER\n");
            printf("~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-\n");
            printf("\n Out of attempts! You couldn't beat me.\n");
            SetConsoleTextAttribute(text, 10);
            printf(" The number I was thinking of was: %d\n", number);
        }
        else
        {
            SetConsoleTextAttribute(text, 10); 
            printf("\n~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~-~\n");
            if (attempt == 1) 
            {
                printf("\n You are surely a telepath!\n");
                printf(" You guessed it in the first try!!\n");
            }
            else if (attempt > 1 && attempt <= 6) 
            {
                printf("\n You are a great mind reader.\n");
                printf(" Only a few can guess it right so quickly!!\n");
            }
            else if (attempt > 6) 
            {
                printf("\n Well, you took a lot of guesses,\n");
                printf(" but at least you got it right!\n");
            }
        }
        
        SetConsoleTextAttribute(text, 7);
        printf("\n Would you like to play again? Enter [Y/N]: ");
        scanf(" %c", &replay);
        while(getchar() != '\n');
        
        mciSendString("stop win", NULL, 0, NULL);
        mciSendString("stop lose", NULL, 0, NULL);
        
    } while(replay == 'Y' || replay == 'y');
    
    mciSendString("close all", NULL, 0, NULL);
    return 0;
}
