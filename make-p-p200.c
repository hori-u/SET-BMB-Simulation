#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main() {
    FILE *inputFile, *outputFile;
    char inputFileName[] = "p-p200.txt";
    char outputFileName[] = "p-p200-20-20-2.txt";
    char line[11];
    char *check;
    int cnt1 = 0;
    int cnt2 = 0;
    int MAX_KAKIN_USER = 30;

    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("ファイル '%s' を開けませんでした。\n", inputFileName);
        return 1;
    }

    outputFile = fopen(outputFileName, "w");
    if (outputFile == NULL) {
        printf("ファイル '%s' を作成できませんでした。\n", outputFileName);
        fclose(inputFile);
        return 1;
    }

    srand(time(NULL)); // 乱数ジェネレータを初期化

    while (fgets(line, 10, inputFile) != NULL) {
        int value;
        //check = NULL;
        //check = strtok(line, ",");
        char *commmaPosition = strchr(line, ',');

        if (commmaPosition != NULL){
            
            *commmaPosition = '\0';

            char *first = line;

            fprintf(outputFile, "%s", line);
        }
        char *second = commmaPosition + 1;
        int num = atoi(second);

        //if (sscanf(line, "%d,", &value) == 1) {
        int randomValue = rand() % 100;
        if (0 < randomValue <= 15 && cnt1 <= MAX_KAKIN_USER) {
            // 70%の確率で0
            fprintf(outputFile, ",%d\n", 1);
            cnt1++;
        } else if (16 < randomValue <= 30 && cnt2 <= MAX_KAKIN_USER) {
            // 20%の確率で1
            fprintf(outputFile, ",%d\n", 2);
            cnt2++;
        } else {
                            // 10%の確率で2
            fprintf(outputFile, ",%d\n", 0);
        }

    }

    fclose(inputFile);
    fclose(outputFile);

    printf("ファイル '%s' を作成しました。\n", outputFileName);
    printf("1: %d, 2: %d\n", cnt1, cnt2);

    return 0;
}
