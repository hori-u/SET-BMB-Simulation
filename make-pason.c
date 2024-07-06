#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_RANDOM_COUNT 40

int main() {
    FILE *inputFile, *outputFile;
    char inputFileName[] = "p-p800.txt";     // 入力ファイル名
    char outputFileName[] = "p-p800-40-40.txt";   // 出力ファイル名
    char line[1024];
    char *commaPosition;
    int randomNumber;
    //int numbers[MAX_RANDOM_COUNT];
    int randomCount = 0;
    int cnt1 = 0;
    int cnt2 = 0;

    // 乱数ジェネレータを初期化
    srand(time(NULL));

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

    while (fgets(line, sizeof(line), inputFile)) {
        // ','を見つける
        commaPosition = strchr(line, ',');

        if (commaPosition != NULL) {
            // ','をヌル文字に置き換える
            *commaPosition = '\0';

            // 前部分を出力ファイルにコピー
            fprintf(outputFile, "%s,", line);

            // ','の後の部分が1なら1か2のランダムな整数、0ならそのまま0を付け加える
            int value;
            if (sscanf(commaPosition + 1, "%d", &value) == 1) {
                if (value == 0) {
                    fprintf(outputFile, "0");
                } else {
                    randomNumber = (rand() % 2) + 1; // 1か2のランダムな整数
                    if ((randomNumber == 1 && cnt1 < MAX_RANDOM_COUNT)||(cnt1 < MAX_RANDOM_COUNT && cnt2 >= MAX_RANDOM_COUNT)) {
                        fprintf(outputFile, "%d", randomNumber);
                        cnt1++;
                    } else if ((randomNumber == 2 && cnt2 < MAX_RANDOM_COUNT)||(cnt2 < MAX_RANDOM_COUNT && cnt1 >= MAX_RANDOM_COUNT)) {
                        fprintf(outputFile, "%d", randomNumber);
                        cnt2++;
                    } else {
                        fprintf(outputFile, "0");
                    }
                }
            }
            fprintf(outputFile, "\n");
        } else {
            // ','が見つからなかった場合
            fprintf(outputFile, "%s", line);
        }
    }

    fclose(inputFile);
    fclose(outputFile);

    // 結果を表示
    printf("出力ファイル '%s' にコピーしました。\n", outputFileName);
    printf("cnt1 = %d\ncnt2 = %d\n", cnt1, cnt2);
    //for (int i = 0; i < randomCount; i++) {
    //    printf("%d ", numbers[i]);
    //}
    //printf("\n");

    return 0;
}
