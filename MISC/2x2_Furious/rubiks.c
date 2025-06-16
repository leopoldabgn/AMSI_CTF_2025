#include <time.h>
#include <string.h>

#include "functions.h"

#ifdef OS_WINDOWS
#include <conio.h>
#include <windows.h>
#else
#include "getch.h"
#endif

// Cube state representation
int cube[H][W] = {        {6,6},
                          {6,6},
                    {2,2},{1,1},{4,4},{3,3},
                    {2,2},{1,1},{4,4},{3,3},
                          {5,5},
                          {5,5}};

// Game state variables
int difficulty = 0, position = 0, PLL_skip = 0, OLL_skip = 0, gamemode = 0;
int no_mouv;
int challenge_mode = 1; // Always challenge mode

char solution[1000], empty_file[1000], rep[100];

int main()
{
    srand(time(NULL)); // Needed for random generation

    puts("(Press 'q' to quit the program)");

    clear();
    printf("\n| RUBIK'S CUBE 2X2X2 |\n\n");

    // Challenge mode only
    challenge_mode = 1;
    gamemode = 0;
    printf("=== CTF CHALLENGE MODE ===\n\n");
    printf("Solve the cube in under 10 seconds to get the flag!\n\n");

    // Display game controls
    display_controls();

    reset_cube(1);
    afficherCube();

    printf("\nPress Enter to scramble and start the challenge...\n");
    fflush(stdout);

    while(getchar() != '\n'); // Wait for Enter

    scramble(20);
    lancer_jeu_challenge();

    return 0;
}

void display_controls()
{
    printf("=== GAME CONTROLS ===\n\n");
    printf("CW = Clockwise, CCW = Counter-Clockwise\n\n");
    printf("Moves:\n");
    printf("  r = R (right CW)     R = R' (right CCW)\n");
    printf("  u = U (top CW)       U = U' (top CCW)\n");
    printf("  f = F (front CW)     F = F' (front CCW)\n");
    printf("  l = L (left CW)      L = L' (left CCW)\n");
    printf("  d = D (bottom CW)    D = D' (bottom CCW)\n");
    printf("  b = B (back CW)      B = B' (back CCW)\n\n");
    printf("Other commands:\n");
    printf("  q = quit\n\n");
}

void lancer_jeu_challenge()
{
    int jeu = 0, player_move = 0;
    int temps;
    
    time_t start = time(NULL); // Start timing the challenge

    printf("\nYour move: ");
    fflush(stdout);

    while(jeu == 0)
    {
        // Check if cube is solved
        if(is_cube_solved())
        {
            temps = (int)(time(NULL) - start);
            
            printf("\n\n=== CUBE SOLVED! ===\n");
            printf("Time: %d seconds\n\n", temps);
            
            if(temps <= 10.0)
            {
                printf("🎉 CONGRATULATIONS! 🎉\n");
                printf("You solved the cube in under 10 seconds!\n");
                printf("Here is your flag: AMSI{RUB1K5_5P33D_MA5T3R!}\n\n");
            }
            else
            {
                printf("❌ TIME'S UP! ❌\n");
                printf("You took too long to solve the cube.\n");
                printf("Try again to get the flag!\n\n");
            }
            
            jeu = 1; // End the game
            break;
        }
        
        // Check if time limit exceeded BEFORE getting input
        temps = (int)(time(NULL) - start);
        if(temps >= 10)
        {
            printf("\n\n❌ TIME'S UP! ❌\n");
            printf("You exceeded the 10-second limit!\n");
            exit(EXIT_SUCCESS);
        }

        player_move = get_char();
        printf("Cube Move: ");
        execute_move(player_move);

        // Calculate and display updated time AFTER the move
        temps = (int)(time(NULL) - start);
        printf("\nTime: %d/10s\n\nYour move: ", temps);
        fflush(stdout);
        // sleep(100);
    }
}

void execute_move(int player_move)
{
    switch(player_move)
    {
    case 'd':
        printf("D\n");
        mouv_D();
        afficherCube();
        break;
    case 'u':
        printf("U\n");
        mouv_U();
        afficherCube();
        break;
    case 'f':
        printf("F\n");
        mouv_F();
        afficherCube();
        break;
    case 'r':
        printf("R\n");
        mouv_R();
        afficherCube();
        break;
    case 'l':
        printf("L\n");
        mouv_L();
        afficherCube();
        break;
    case 'b':
        printf("B\n");
        mouv_B();
        afficherCube();
        break;
    case 'D':
        printf("D'\n");
        mouv_D(); mouv_D(); mouv_D();
        afficherCube();
        break;
    case 'U':
        printf("U'\n");
        mouv_U(); mouv_U(); mouv_U();
        afficherCube();
        break;
    case 'F':
        printf("F'\n");
        mouv_F(); mouv_F(); mouv_F();
        afficherCube();
        break;
    case 'R':
        printf("R'\n");
        mouv_R(); mouv_R(); mouv_R();
        afficherCube();
        break;
    case 'L':
        printf("L'\n");
        mouv_L(); mouv_L(); mouv_L();
        afficherCube();
        break;
    case 'B':
        printf("B'\n");
        mouv_B(); mouv_B(); mouv_B();
        afficherCube();
        break;
    default:
        printf("Unknown move\n");
        break;
    }
}

void sleep(int time)
{
    #ifdef OS_WINDOWS
    // Sleep(time);
    #else
    // TODO: add Unix-compatible delay if needed
    #endif
}

void clear()
{
    #ifdef OS_WINDOWS
    system("cls");
    #else
    printf("\033[2J\033[H"); // Clear screen for Unix/Linux
    #endif 
}

int get_char()
{
    int value;
    #ifdef OS_WINDOWS
    value = getch();
    #else
    value = getch();
    #endif

    // Quit the program if 'q' is pressed
    if(value == 'q')
        exit(EXIT_SUCCESS);

    clear_buffer();
    return value;
}

void clear_buffer()
{
    while (getchar() != '\n');
}

void scramble(int nombreMax)
{
    int alea = 0;
    char choix[1] = {0};
    char melange[nombreMax];

    reset_cube(1);

    printf("\nScrambling...\n\n");

    for(int i=0; i<nombreMax; i++)
    {
        alea = nombreAleatoire(6);

        switch(alea)
        {
        case 1:
            mouv_F(); choix[0] = 'F'; break;
        case 2:
            mouv_U(); choix[0] = 'U'; break;
        case 3:
            mouv_R(); choix[0] = 'R'; break;
        case 4:
            mouv_L(); choix[0] = 'L'; break;
        case 5:
            mouv_D(); choix[0] = 'D'; break;
        case 6:
            mouv_B(); choix[0] = 'B'; break;
        }

        melange[i] = choix[0];
    }

    for(int i=0; i<1000; i++)
    {
        solution[i] = 0;
    }

    position = 0;

    // On affiche pas le melange, sinon il suffit de le refaire
    // dans l'autre sens... Et c'est terminé ! Et ca casse le chall.
    // On pourrait faire un Chall 1/2, sans le solver, mais avec le scramble affiché

    // Il suffirait donc de le refaire dans l'autre sens
    // Puis une partie 2/2 sans le scramble et donc il faut utilisé le solver.


    // Hmmm... Mais si il utilise un outil externe pour solve le cube il pourra faire les deux instantanément.

    // printf("Scramble: ");
    // for(int i = 0; i < nombreMax; i++) printf("%c", melange[i]);

    printf("\n\n");
    afficherCube();
    printf("\n");
}