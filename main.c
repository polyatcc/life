#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* inttostr(int a)
{
    int b = a;
    int k = 0;
    while (b > 0)
    {
        b = b / 10;
        k++;
    }
    char* c = (char*)malloc(sizeof(char) * (k + 1));
    int i;
    for (i = k - 1; i >= 0; i--)
    {
        if (a % 10 == 0){
            c[i] = '0';
        }
        else if (a % 10 == 1){
            c[i] = '1';
        }
        else if (a % 10 == 2){
            c[i] = '2';
        }
        else if (a % 10 == 3){
            c[i] = '3';
        }
        else if (a % 10 == 4){
            c[i] = '4';
        }
        else if (a % 10 == 5){
            c[i] = '5';
        }
        else if (a % 10 == 6){
            c[i] = '6';
        }
        else if (a % 10 == 7){
            c[i] = '7';
        }
        else if (a % 10 == 8){
            c[i] = '8';
        }
        else if (a % 10 == 9){
            c[i] = '9';
        }
        //c[i] = a % 10 + '0';
        a = a / 10;
    }
    c[k] = '\0';
    return c;
}

int strtoint(char* c){
    int i = 0;
    int ans = 0;
    while (c[i] != '\0'){
        if (c[i] == '0'){
            ans = ans * 10;
        }
        else if (c[i] == '1'){
            ans = ans * 10 + 1;
        }
        else if (c[i] == '2'){
            ans = ans * 10 + 2;
        }
        else if (c[i] == '3'){
            ans = ans * 10 + 3;
        }
        else if (c[i] == '4'){
            ans = ans * 10 + 4;
        }
        else if (c[i] == '5'){
            ans = ans * 10 + 5;
        }
        else if (c[i] == '6'){
            ans = ans * 10 + 6;
        }
        else if (c[i] == '7'){
            ans = ans * 10 + 7;
        }
        else if (c[i] == '8'){
            ans = ans * 10 + 8;
        }
        else if (c[i] == '9'){
            ans = ans * 10 + 9;
        }
        i++;
    }
    return ans;
}

int main(int argc, char* argv[]) {
    char *name_in = argv[2];
    char *dir_out = argv[4];
    int j = 0;

    FILE *file = fopen(name_in, "rb");
    fseek(file, 0, SEEK_SET);
    int size, offset, width, height;
    char skip;
    int skip_int;
    char name_out[50];
    name_out[0] = 'g';
    name_out[1] = 'e';
    name_out[2] = 'n';
    name_out[3] = '\0';
    char count[50];
    for (int i = 0; i < 2; ++i) {
        fread(&skip, 1, 1, file);
    }
    fread(&size, 4, 1, file);
    for (int i = 0; i < 4; ++i) {
        fread(&skip, 1, 1, file);
    }
    fread(&offset, 4, 1, file);
    fread(&skip_int, 4, 1, file);
    fread(&width, 4, 1, file);
    fread(&height, 4, 1, file);
    //printf("%d", offset);
    int **a = (int **) malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        a[i] = (int *) malloc(width * sizeof(int));
    }

    int **b = (int **) malloc(height * sizeof(int *));
    for (int i = 0; i < height; i++) {
        b[i] = (int *) malloc(width * sizeof(int));
    }

    unsigned short int d;
    for (int i = height - 1; i >= 0; --i) {
        for (int j = 0; j < width; ++j) {
            fread(&d, 1, 1, file); //24-битная кодировка
            fread(&d, 1, 1, file);
            fread(&d, 1, 1, file);
            //printf("%d ", d);
            if (d == 255) { //если не двухцветная
                a[i][j] = 255;
            } else
                a[i][j] = 0;
        }
    }

    char c;
    int max_iter = strtoint(argv[6]);
    int dump_freq = strtoint(argv[8]);
    for (int k = 1; k < max_iter; ++k) {
        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                b[i][j] = a[i][j];
                unsigned short int neighbour = 0;
                if (a[(i - 1 + height) % height][(j - 1 + width) % width] == 0) {
                    neighbour++;
                }
                if (a[(i - 1 + height) % height][(j + width) % width] == 0) {
                    neighbour++;
                }
                if (a[(i - 1 + height) % height][(j + 1 + width) % width] == 0) {
                    neighbour++;
                }
                if (a[(i + height) % height][(j - 1 + width) % width] == 0) {
                    neighbour++;
                }
                if (a[(i + height) % height][(j + 1 + width) % width] == 0) {
                    neighbour++;
                }
                if (a[(i + 1 + height) % height][(j - 1 + width) % width] == 0) {
                    neighbour++;
                }
                if (a[(i + 1 + height) % height][(j + width) % width] == 0) {
                    neighbour++;
                }
                if (a[(i + 1 + height) % height][(j + 1 + width) % width] == 0) {
                    neighbour++;
                }
                if (a[i][j] == 0) {
                    if ((neighbour > 3) || (neighbour < 2)) {
                        b[i][j] = 255;
                    }
                } else { //она белая
                    if (neighbour == 3) {
                        b[i][j] = 0;
                    }
                }
            }
        }

        for (int i = 0; i < height; ++i) {
            for (int j = 0; j < width; ++j) {
                a[i][j] = b[i][j];
            }
        }

        if (k % dump_freq == 0) {
            strcpy(count, dir_out);
            strcat(count, "\\"); //переход в папку
            strcat(count, name_out);
            strcat(count, "_");
            char *s = inttostr(k + 1);
            int i = 0;
            /*while (s[i] != '\0'){
                printf("%c", s[i]);
                ++i;
            }*/
            //printf("\n");
            strcat(count, inttostr(k + 1));
            strcat(count, ".bmp");
            FILE *file_out = fopen(count, "wb");
            fseek(file, 0, SEEK_SET);
            fseek(file_out, 0, SEEK_SET);
            for (int i = 0; i < offset; i++) {
                fread(&c, 1, 1, file);
                fwrite(&c, 1, 1, file_out);
            }
            fseek(file, offset, SEEK_SET);
            fseek(file_out, offset, SEEK_SET);
            for (int i = height - 1; i >= 0; i--) {
                //printf("%d ", i);
                for (int j = 0; j < width; j++) {
                    fwrite(&b[i][j], 1, 1, file_out);
                    fwrite(&b[i][j], 1, 1, file_out);
                    fwrite(&b[i][j], 1, 1, file_out);
                }
            }
        }
    }
    return 0;
}
