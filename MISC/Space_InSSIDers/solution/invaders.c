#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define SPECIAL_COUNT 24

const char *special_chars = "!@;,.:/\\$^&\"'{}()[]+-|`_%";
const char *base = "invaders";

// 2⁸ = 256 variantes
void to_case_variant(char *dst, int variant_id) {
    for (int i = 0; i < 8; i++) {
        dst[i] = (variant_id & (1 << i)) ? (base[i] - 32) : base[i];
    }
    dst[8] = '\0';
}

int main() {
    FILE *f = fopen("passwords.txt", "w");
    if (!f) {
        perror("Erreur ouverture fichier");
        return 1;
    }

    #pragma omp parallel for collapse(3) schedule(dynamic)
    for (int variant = 0; variant < 256; variant++) {
        for (int num = 0; num < 100; num++) {
            for (int i = 0; i < SPECIAL_COUNT; i++) {
                for (int j = 0; j < SPECIAL_COUNT; j++) {
                    char buffer[64];
                    char word[16];
                    to_case_variant(word, variant);
                    sprintf(buffer, "%s%02d%c%c\n", word, num, special_chars[i], special_chars[j]);

                    #pragma omp critical
                    fputs(buffer, f);
                }
            }
        }
    }

    fclose(f);
    printf("Fichier 'passwords.txt' généré.\n");
    return 0;
}

