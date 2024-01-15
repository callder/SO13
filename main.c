#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MEM_SIZE 1000


int Map[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0,};
char printCell(int choose){
    if(choose == 0){
        return ' ';
    }
    if(choose == 1){
        return 'X';
    }
    if(choose == 2){
        return 'O';
    }
}
bool isWin(int pside){
    return ((Map[0] == pside ) && (Map[1] == pside) && (Map[2] == pside)) || ((Map[3] == pside ) && (Map[4] == pside) && (Map[5] == pside)) ||
           ((Map[6] == pside ) && (Map[7] == pside) && (Map[8] == pside)) || ((Map[0] == pside ) && (Map[3] == pside) && (Map[6] == pside)) ||
           ((Map[1] == pside ) && (Map[4] == pside) && (Map[7] == pside)) || ((Map[2] == pside ) && (Map[5] == pside) && (Map[8] == pside)) ||
           ((Map[0] == pside ) && (Map[4] == pside) && (Map[8] == pside)) || ((Map[2] == pside ) && (Map[4] == pside) && (Map[6] == pside));
}
void showGameMap(){

    printf(" %c | %c | %c\n", printCell(Map[0]), printCell(Map[1]), printCell(Map[2]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", printCell(Map[3]), printCell(Map[4]), printCell(Map[5]));
    printf("---+---+---\n");
    printf(" %c | %c | %c\n", printCell(Map[6]), printCell(Map[7]), printCell(Map[8]));
}
void showCoordinates(){
    printf("For hitting cell write number of cell\n");
    printf(" 1 | 2 | 3\n");
    printf("---+---+---\n");
    printf(" 4 | 5 | 6\n");
    printf("---+---+---\n");
    printf(" 7 | 8 | 9\n");
}
bool isDraw(){
    for(int i = 0; i<9;i++){
        if(Map[i] == 0){
            return false;
        }
    }
    return true;
}



int main(int argc, char ** argv) {
    if(argc != 2){
        fprintf(stderr, "Use %s  <game_name> \n",argv[0]);
        exit(EXIT_FAILURE);
    }

    HANDLE mapFile = OpenFileMapping(
            FILE_MAP_ALL_ACCESS,
            FALSE,
            argv[1]
    );
    bool imCreator = false;
    if(mapFile == NULL){
        mapFile = CreateFileMapping(
                INVALID_HANDLE_VALUE,
                NULL,
                PAGE_READWRITE,
                0,
                MEM_SIZE,
                argv[1]
        );
        if(mapFile != NULL){
            printf("Game created! id: %s\n",argv[1]);
            imCreator = true;
        }else{
            perror("gameCreation");
            exit(EXIT_FAILURE);

        }
    }else{
        printf("Successfully connected to game %s !\n",argv[1]);

    }
    LPVOID *buffer = MapViewOfFile(
            mapFile,
            FILE_MAP_ALL_ACCESS,
            0,
            0,
            MEM_SIZE
    );
    if(buffer == NULL){
        perror("MVOF");
        exit(EXIT_FAILURE);
    }

    if(imCreator){
        printf("Waiting for second player...\n");
        strcpy(buffer, "WAIT");
        int Player_X  = 1;
        while(strcmp(buffer, "WAIT") == 0){
            Sleep(10);
        }
        showCoordinates();

        while(!isWin(Player_X) && !isWin(2) && !isDraw()) {
            char hit[3];
            int Target;
            printf("Current map\n");
            showGameMap();
            printf("Your turn:\n");
            do{
                scanf_s("%d",&Target);
                if(Target<1 || Target > 9 || Map[Target - 1] != 0){
                    printf("Wrong cell\n");
                }
            }while(Target < 1 || Target>9 || Map[Target - 1] != 0);

            Map[Target - 1] = Player_X;
            if(isWin(Player_X) || isDraw()){
                break;
            }
            showGameMap();

            itoa(Target, (char *) hit, 10);
            hit[1] = 'X';
            hit[2] = '\0';
            strcpy(buffer, hit );

            printf("Waiting for second player move...\n");

            while (strcmp(buffer, hit) == 0){
                Sleep(10);
            }

            strcpy(hit, buffer);
            int target = atoi(hit);

            Map[target - 1] = 2;
            if(strcmp(buffer, "WIN") == 0 || strcmp(buffer, "LOST") == 0 || strcmp(buffer, "DRAW") == 0){
                break;
            }


        }
        showGameMap();
        if(isWin(Player_X)){
            strcpy(buffer, "WIN");
            printf("Congratulations!\n");
        }else if(isWin(2)){
            strcpy(buffer, "LOST");
            printf("Lost\n");
        }else if(strcmp(buffer, "WIN") == 0){
            printf("Lost\n");
        }else if(strcmp(buffer, "LOST") == 0){
            printf("Congratulations!\n");
        }else {
            strcpy(buffer, "DRAW");
            printf("Draw!\n");
        }


    }else{
        printf("<Enter> to connect\n");
        getchar();
        printf("Connected\n");
        strcpy(buffer, "READY");
        int Player_O  = 2;
        while(strcmp(buffer, "READY") == 0){
            Sleep(10);
        }
        showCoordinates();

        while(!isWin(Player_O) && !isWin(1) && !isDraw()) {
            char hit[3];

            strcpy(hit, buffer);
            int target = atoi(hit);

            Map[target - 1] = 1;
            if(strcmp(buffer, "WIN") == 0 || strcmp(buffer, "LOST") == 0 || strcmp(buffer, "DRAW") == 0){
                break;
            }
            int Target;
            printf("Current map\n");
            showGameMap();
            printf("Your turn:\n");

            do{
                scanf_s("%d",&Target);
                if(Target < 1 || Target > 9 || Map[Target - 1] != 0){
                    printf("Wrong cell\n");
                }
            }while(Target < 1 || Target > 9 || Map[Target - 1] != 0);

            Map[Target - 1] = Player_O;

            itoa(Target, (char *) hit, 10);
            hit[1] = 'O';
            hit[2] = '\0';
            strcpy(buffer, hit);
            if(isWin(Player_O) || isDraw() || isWin(1)){
                break;
            }
            printf("Waiting for second player move...\n");
            while (strcmp(buffer, hit) == 0){
                Sleep(10);
            }

        }
        showGameMap();
        if(isWin(Player_O)){
            strcpy(buffer, "WIN");
            printf("Congratulations!\n");
        }else if(isWin(1)){
            strcpy(buffer, "LOST");
            printf("Lost\n");
        }else if(strcmp(buffer, "WIN") == 0){
            printf("Lost\n");
        }else if(strcmp(buffer, "LOST") == 0){
            printf("Congratulations!\n");
        }
        else {
            strcpy(buffer, "DRAW");
            printf("Draw!\n");
        }


    }

    if(!UnmapViewOfFile(buffer)) {
        fprintf(stderr, "Error during unmap view of file\n");
        CloseHandle(mapFile);
        return 3;
    }
    CloseHandle(mapFile);

    return 0;
}