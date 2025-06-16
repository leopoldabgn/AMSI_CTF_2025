#include "getch.h"
#include <stdio.h>
#include <unistd.h>
#include <termios.h>

int getch(void) {
    struct termios oldattr, newattr;
    int ch;
    
    // Flush tous les buffers avant de lire
    fflush(stdout);
    fflush(stderr);
    
    // Sauvegarde des attributs du terminal
    if (tcgetattr(STDIN_FILENO, &oldattr) == -1) {
        // Si on ne peut pas configurer le terminal (cas Docker/netcat)
        // On utilise une lecture simple
        return getchar();
    }
    
    newattr = oldattr;
    
    // Mode raw : pas de ligne buffering, pas d'echo
    newattr.c_lflag &= ~(ICANON | ECHO);
    newattr.c_cc[VMIN] = 1;  // Attend exactement 1 caractère
    newattr.c_cc[VTIME] = 0; // Pas de timeout (bloquant)
    
    // Application des nouveaux attributs
    tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
    
    // Lecture d'un seul caractère
    ch = getchar();
    
    // Restauration des attributs
    tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);
    
    return ch;
}