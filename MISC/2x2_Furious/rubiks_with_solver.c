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
int challenge_mode = 0; // 0 = solver mode, 1 = challenge mode

char solution[1000], empty_file[1000], rep[100];

int main()
{
    srand(time(NULL)); // Needed for random generation

    puts("(Press 'q' to quit the program)");

    // Main menu selection
    char menu_choice = 0;
    while(menu_choice != '1' && menu_choice != '2')
    {
        clear();
        printf("\n=== RUBIK'S CUBE PROGRAM ===\n\n");
        printf("Choose an option:\n\n");
        printf("1. CTF Challenge Mode (solve in under 10 seconds)\n");
        printf("2. Cube Solver Mode (input your cube configuration)\n\n");
        printf("Your choice: ");
        menu_choice = get_char();
    }

    clear();
    printf("\n| RUBIK'S CUBE 2X2X2 |\n\n");
    gamemode = 0;

    if(menu_choice == '1')
    {
        // Challenge mode
        challenge_mode = 1;
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
    }
    else
    {
        // Solver mode
        challenge_mode = 0;
        printf("=== CUBE SOLVER MODE ===\n\n");
        printf("Enter your cube configuration and get the solution!\n\n");
        
        reset_cube(0);

        ask_scramble();
        
        printf("\nYour cube configuration has been set.\n");
        printf("\nPress 'i' to solve, or use other controls to modify the cube.\n\n");
        
        display_controls();
        lancer_jeu_solver();
    }

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
    if(challenge_mode == 0) {
        printf("  i = solve, q = quit\n\n");
    }
    else {
        printf("  q = quit\n\n");
    }
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

void lancer_jeu_solver()
{
    int jeu = 0, player_move = 0;

    printf("\nYour move: ");
    fflush(stdout);

    while(jeu == 0)
    {
        player_move = get_char();
        printf("Cube Move: ");
        execute_move(player_move);

        printf("\nYour move: ");
        fflush(stdout);
        // sleep(100);
    }
}

void execute_move(int player_move)
{
    switch(player_move)
    {
    // case 's':
    //     scramble(20);
    //     break;
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
    // case 'e':
    //     printf("E\n");
    //     mouv_E();
    //     afficherCube();
    //     break;
    // case 'm':
    //     printf("M\n");
    //     mouv_M();
    //     afficherCube();
    //     break;
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
    // case 'E':
    //     printf("E'\n");
    //     mouv_E(); mouv_E(); mouv_E();
    //     afficherCube();
    //     break;
    // case 'M':
    //     printf("M'\n");
    //     mouv_M(); mouv_M(); mouv_M();
    //     afficherCube();
    //     break;
    case 'i':
        if(challenge_mode != 0) {
            printf("Unknown move\n");
            break;
        }
        for(int i=0; i<1000; i++) solution[i] = 0;
        time_t t1 = time(NULL);
        IA();
        time_t t2 = time(NULL);
        float temps = (float)(t2 - t1) / CLOCKS_PER_SEC;
        printf("AI solve time = %.3f seconds\n", temps);
        display_solve();
        break;
    // case 'a':
    //     reset_cube();
    //     printf("Cube reset\n");
    //     afficherCube();
    //     break;
    // case 'p':
    //     for(int i=0; i<1000; i++) solution[i] = 0;
    //     printf("Solution cleared\n");
    //     break;
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

void ask_scramble() 
{
    clear();
    afficherCube();

    char c;
    int col = 2, line = 12; // For 2x2x2 cube

    printf("Configure your cube by selecting colors for each square.\n\n");

    for(int j=0; j<line; j++)
    {
        for(int i=0; i<col; i++)
        {
            printf("Select color for position [%d][%d]:\n\n", j, i);
            printf("Available colors:\n");
            printf("red (r), green (g), orange (o)\n");
            printf("blue (b), white (w), yellow (y)\n\n");
            printf("Your choice: ");
            
            c = 0;
            while(c != 'r' && c != 'g' && c != 'o' && c != 'b' && c != 'w' && c != 'y')
                c = get_char();
            
            cube[j][i] = getColor(c);
            clear();
            afficherCube();
            printf("\nProgress: %d/%d squares configured\n\n", (j*col + i + 1), (line*col));
        }
    }
    
    printf("Cube configuration complete!\n\n");
}

