/* 
Proje ortak þekilde yapýlmýþtýr. 
Düþen kutular k, 2k, 3k... þeklinde kullanýlmýþtýr. Orta ksýmlarý buna göre dinamik belirlenmiþtir.

200101010 - Muharrem DEMIR
210101157 - Berkay DURSUN
200101007 - Murat EKER
*/



#include "icb_gui.h"
#include <stdlib.h>
#include <time.h>
#include <math.h>


int keypressed;
ICBYTES m;


void ICGUI_Create() {
    ICG_MWTitle("Sliding Box with Falling Boxes");
    ICG_MWSize(900, 900);
}

bool checkCollision(int bulletX, int bulletY, int boxX, int boxY, int boxWidth, int boxHeight) {
    return (bulletX >= boxX && bulletX <= boxX + boxWidth &&
        bulletY >= boxY && bulletY <= boxY + boxHeight);
}

int determineHitDirection(int bulletX, int boxX, int boxWidth) {
    int leftZone = boxX + (boxWidth / 3);
    int rightZone = boxX + 2 * (boxWidth / 3);

    if (bulletX < leftZone) return -1;
    if (bulletX > rightZone) return 1;
    return 0;
}

VOID* SlidingBox(PVOID lpParam) {

    int* arr = (int*)lpParam; //variables[0]
    int* boxX = arr;
    int* boxY = arr + 1;


    while (*(arr + 30) == 1) {
        FillRect(m, *boxX, *boxY, 20, 10, 0x000000);

        if (keypressed == 37 && *boxX > 0) *boxX -= 5;
        if (keypressed == 39 && *boxX < 780) *boxX += 5;

        FillRect(m, *boxX, *boxY, 20, 10, 0xFF0000);
        DisplayImage(*(arr + 31), m);
        Sleep(30);
    }
    return NULL;
}

VOID* BulletMovement(PVOID lpParam) {
    int bulletDirection = -10;


    int* arr = (int*)lpParam; //variables[0]
    int* boxX = arr;
    int* boxY = arr + 1;
    int* bulletX = arr + 2;
    int* bulletY = arr + 3;
    int* fallingBoxX = arr + 5;
    int* fallingBoxY = arr + 10;
    int* fallingBoxWidth = arr + 15;
    int* isDestroyed = arr + 20;
    int* moveDirection = arr + 25;


    while (*(arr + 30) == 1) { //thraed_continue
        if (*(arr + 4) == 0 && keypressed == 32) {
            // Mermi aktif deðilken ve boþluk tuþuna basýldýðýnda mermiyi ateþle
            *(arr + 4) = 1;
            *bulletX = *boxX + 8;
            *bulletY = *boxY - 10;
            keypressed = 0;  // Yeni mermi ateþlenmesin diye keypressed sýfýrlanýr
        }

        if (*(arr + 4) == 1) {
            FillRect(m, *bulletX, *bulletY, 5, 10, 0x000000);
            *bulletY += bulletDirection;

            for (int i = 0; i < 5; i++) {
                if (*(isDestroyed + i) == 0 && checkCollision(*bulletX, *bulletY, *(fallingBoxX + i), *(fallingBoxY + i), *(fallingBoxWidth + i), 10)) {
                    int hitDirection = determineHitDirection(*bulletX, *(fallingBoxX + i), *(fallingBoxWidth + i));

                    FillRect(m, *(fallingBoxX + i), *(fallingBoxY + i), *(fallingBoxWidth + i), 10, 0x000000);
                    *(arr + 4) = 0;

                    if (hitDirection == 0) {
                        *(isDestroyed + i) = 1;
                        *(fallingBoxX + i) = rand() % (800 - *(fallingBoxWidth + i));
                        *(fallingBoxY + i) = 0;
                    }
                    else {
                        *(moveDirection + i) = -hitDirection;
                        *(fallingBoxY + i) -= 5;
                    }
                    break;
                }
            }

            if (*bulletY <= 0) {
                *(arr + 4) = 0;
            }

            if (*(arr + 4) == 1) {
                FillRect(m, *bulletX, *bulletY, 5, 10, 0x0000FF);
            }
            DisplayImage(*(arr + 31), m);
            Sleep(30);
        }
    }
    return NULL;
}

VOID* FallingBoxes(PVOID lpParam) {

    int* arr = (int*)lpParam; //variables[0]

    int* boxX = arr;
    int* boxY = arr + 1;
    int* fallingBoxX = arr + 5;
    int* fallingBoxY = arr + 10;
    int* fallingBoxWidth = arr + 15;
    int* isDestroyed = arr + 20;
    int* moveDirection = arr + 25;

    while (*(arr + 30) == 1) {
        for (int i = 0; i < 5; i++) {
            if (*(isDestroyed + i) == 0) {
                FillRect(m, *(fallingBoxX + i), *(fallingBoxY + i), *(fallingBoxWidth + i), 10, 0x000000);

                if (*(moveDirection + i) == 0) {
                    *(fallingBoxY + i) += 5;
                }
                else {
                    *(fallingBoxY + i) -= 5;
                    *(fallingBoxX + i) += *(moveDirection + i) * 3;

                    if (*(fallingBoxY + i) <= 0 || *(fallingBoxX + i) <= 0 || *(fallingBoxX + i) >= 800 - *(fallingBoxWidth + i)) {
                        *(moveDirection + i) = 0;
                        *(fallingBoxY + i) = 0;
                        *(fallingBoxX + i) = rand() % (800 - *(fallingBoxWidth + i));
                    }
                }

                if (*(fallingBoxY + i) > 800) {
                    *(fallingBoxY + i) = 0;
                    *(fallingBoxX + i) = rand() % (800 - *(fallingBoxWidth + i));
                    *(moveDirection + i) = 0;
                }

                FillRect(m, *(fallingBoxX + i), *(fallingBoxY + i), *(fallingBoxWidth + i), 10, 0x00FF00);
            }
            else if (*(fallingBoxY + i) == 0) {
                *(isDestroyed + i) = 0;
            }
        }
        DisplayImage(*(arr + 31), m);
        Sleep(50);
    }
    return NULL;
}

void butonfonk() {
    DWORD dw;
    m = 0;


    static int variables[32] = {
        /* 0  */ 300,           // boxX 
        /* 1  */ 680,           // boxY  
        /* 2  */ 0,             // bulletX
        /* 3  */ 0,             // bulletY
        /* 4  */ 0,             // bulletActive (false = 0)
        /* 5  */ 0, 0, 0, 0, 0, // fallingBoxX[5]
        /* 10 */ 0, -100, -200, -300, -400, // fallingBoxY[5]
        /* 15 */ 30, 40, 50, 60, 70, // fallingBoxWidth[5]
        /* 20 */ 0, 0, 0, 0, 0, // isDestroyed[5] (false = 0)
        /* 25 */ 0, 0, 0, 0, 0, // moveDirection[5]
        /* 30 */ 0,             // thread_continue (false = 0)
        /* 31 */ 0              //FRM1  
    };

    variables[31] = ICG_FrameMedium(5, 40, 800, 700);



    if (variables[30] == 0) {
        variables[30] = 1;

        srand(time(NULL));


        for (int i = 0; i < 5; i++) {
            variables[5 + i] = rand() % (800 - variables[15 + i]);
            variables[20 + i] = 0;
            variables[25 + i] = 0;
        }
        

        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SlidingBox, &variables[0], 0, &dw);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)BulletMovement, &variables[0], 0, &dw);
        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)FallingBoxes, &variables[0], 0, &dw);
        SetFocus(ICG_GetMainWindow());
    }
    else {
        variables[30] = 0;
    }
}

void WhenKeyPressed(int k) {
    keypressed = k;
}

void ICGUI_main() {
    ICG_Button(5, 5, 120, 25, "START / STOP", butonfonk);
    ICG_SetOnKeyPressed(WhenKeyPressed);
    CreateImage(m, 800, 800, ICB_UINT);
}