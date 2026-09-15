#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_RECORDS 200
#define MONTHS 12

typedef struct {
    int year;
    double monthly[MONTHS];
    double annual;
} RainfallRecord;

RainfallRecord records[MAX_RECORDS];
int recordCount = 0;

long long mergeComparisons = 0;
long long mergeSwaps = 0;
long long quickComparisons = 0;
long long quickSwaps = 0;

typedef struct {
    double min;
    double max;
    int minYear;
    int maxYear;
} MinMax;

int readCSV(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: cannot open file %s\n", filename);
        return -1;
    }

    char line[1024];
    if (!fgets(line, sizeof(line), fp)) {
        fclose(fp);
        return -1;
    }

    int count = 0;
    while (fgets(line, sizeof(line), fp)) {
        char *token = strtok(line, ",");
        if (!token) continue;

        int year = atoi(token);
        double months[MONTHS];
        for (int i = 0; i < MONTHS; i++) {
            token = strtok(NULL, ",");
            if (!token) break;
            months[i] = atof(token);
        }
        token = strtok(NULL, ",");
        double annual = token ? atof(token) : 0.0;

        records[count].year = year;
        for (int i = 0; i < MONTHS; i++)
            records[count].monthly[i] = months[i];
        records[count].annual = annual;
        count++;
        if (count >= MAX_RECORDS) break;
    }

    fclose(fp);
    recordCount = count;
    return count;
}

void displayAll() {
    printf("\n%-6s %6s %6s %6s %6s %6s %6s %6s %6s %6s %6s %6s %6s %8s\n",
           "YEAR", "JAN", "FEB", "MAR", "APR", "MAY", "JUN", "JUL",
           "AUG", "SEP", "OCT", "NOV", "DEC", "ANNUAL");
    for (int i = 0; i < recordCount; i++) {
        printf("%-6d", records[i].year);
        for (int m = 0; m < MONTHS; m++)
            printf(" %6.1f", records[i].monthly[m]);
        printf(" %8.1f\n", records[i].annual);
    }
}

void findMinMaxLinear(int month, int *minYear, int *maxYear,
                      double *minVal, double *maxVal) {
    if (recordCount == 0) return;
    *minVal = records[0].monthly[month];
    *maxVal = records[0].monthly[month];
    *minYear = records[0].year;
    *maxYear = records[0].year;
    for (int i = 1; i < recordCount; i++) {
        if (records[i].monthly[month] < *minVal) {
            *minVal = records[i].monthly[month];
            *minYear = records[i].year;
        }
        if (records[i].monthly[month] > *maxVal) {
            *maxVal = records[i].monthly[month];
            *maxYear = records[i].year;
        }
    }
}

MinMax findMinMaxDC(int left, int right, int month) {
    MinMax res;
    if (left == right) {
        res.min = records[left].monthly[month];
        res.max = records[left].monthly[month];
        res.minYear = records[left].year;
        res.maxYear = records[left].year;
        return res;
    }
    int mid = (left + right) / 2;
    MinMax leftRes = findMinMaxDC(left, mid, month);
    MinMax rightRes = findMinMaxDC(mid + 1, right, month);

    res.min = (leftRes.min < rightRes.min) ? leftRes.min : rightRes.min;
    res.minYear = (leftRes.min < rightRes.min) ? leftRes.minYear : rightRes.minYear;
    res.max = (leftRes.max > rightRes.max) ? leftRes.max : rightRes.max;
    res.maxYear = (leftRes.max > rightRes.max) ? leftRes.maxYear : rightRes.maxYear;
    return res;
}

void doPartA() {
    int month;
    printf("Enter month (1-12): ");
    scanf("%d", &month);
    if (month < 1 || month > 12) {
        printf("Invalid month.\n");
        return;
    }
    int monthIdx = month - 1;

    int minYear, maxYear;
    double minVal, maxVal;
    findMinMaxLinear(monthIdx, &minYear, &maxYear, &minVal, &maxVal);
    printf("\n--- Linear ---\n");
    printf("Minimum: year %d, rainfall %.1f\n", minYear, minVal);
    printf("Maximum: year %d, rainfall %.1f\n", maxYear, maxVal);

    MinMax result = findMinMaxDC(0, recordCount - 1, monthIdx);
    printf("\n--- Divide & Conquer ---\n");
    printf("Minimum: year %d, rainfall %.1f\n", result.minYear, result.min);
    printf("Maximum: year %d, rainfall %.1f\n", result.maxYear, result.max);
}

void merge(RainfallRecord arr[], int left, int mid, int right,
           RainfallRecord temp[]) {
    int i = left, j = mid + 1, k = left;

    while (i <= mid && j <= right) {
        mergeComparisons++;
        if (arr[i].annual <= arr[j].annual) {
            temp[k++] = arr[i++];
            mergeSwaps++;
        } else {
            temp[k++] = arr[j++];
            mergeSwaps++;
        }
    }
    while (i <= mid) {
        temp[k++] = arr[i++];
        mergeSwaps++;
    }
    while (j <= right) {
        temp[k++] = arr[j++];
        mergeSwaps++;
    }

    for (i = left; i <= right; i++) {
        arr[i] = temp[i];
        mergeSwaps++;
    }
}

void mergeSortRec(RainfallRecord arr[], int left, int right,
                  RainfallRecord temp[]) {
    if (left < right) {
        int mid = (left + right) / 2;
        mergeSortRec(arr, left, mid, temp);
        mergeSortRec(arr, mid + 1, right, temp);
        merge(arr, left, mid, right, temp);
    }
}

void swapRecords(RainfallRecord *a, RainfallRecord *b) {
    RainfallRecord temp = *a;
    *a = *b;
    *b = temp;
    quickSwaps++;
}

int partition(RainfallRecord arr[], int low, int high) {
    int randIdx = low + rand() % (high - low + 1);
    swapRecords(&arr[randIdx], &arr[high]);

    double pivot = arr[high].annual;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        quickComparisons++;
        if (arr[j].annual <= pivot) {
            i++;
            if (i != j) {
                swapRecords(&arr[i], &arr[j]);
            }
        }
    }
    if (i + 1 != high) {
        swapRecords(&arr[i + 1], &arr[high]);
    }
    return i + 1;
}

void quickSortRec(RainfallRecord arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSortRec(arr, low, pi - 1);
        quickSortRec(arr, pi + 1, high);
    }
}

void writeSortedToFile(const char *filename, RainfallRecord arr[], int n) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Cannot write to %s\n", filename);
        return;
    }
    fprintf(fp, "YEAR,JAN,FEB,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC,ANNUAL\n");
    for (int i = 0; i < n; i++) {
        fprintf(fp, "%d", arr[i].year);
        for (int m = 0; m < MONTHS; m++)
            fprintf(fp, ",%.1f", arr[i].monthly[m]);
        fprintf(fp, ",%.1f\n", arr[i].annual);
    }
    fclose(fp);
}

void doPartB() {
    if (recordCount == 0) {
        printf("No data to sort.\n");
        return;
    }

    RainfallRecord *mergeArr = (RainfallRecord*)malloc(recordCount * sizeof(RainfallRecord));
    RainfallRecord *quickArr = (RainfallRecord*)malloc(recordCount * sizeof(RainfallRecord));
    RainfallRecord *temp = (RainfallRecord*)malloc(recordCount * sizeof(RainfallRecord));

    if (!mergeArr || !quickArr || !temp) {
        printf("Memory allocation failed.\n");
        free(mergeArr); free(quickArr); free(temp);
        return;
    }

    memcpy(mergeArr, records, recordCount * sizeof(RainfallRecord));
    memcpy(quickArr, records, recordCount * sizeof(RainfallRecord));

    mergeComparisons = 0;
    mergeSwaps = 0;
    quickComparisons = 0;
    quickSwaps = 0;

    mergeSortRec(mergeArr, 0, recordCount - 1, temp);
    writeSortedToFile("merge_sorted.csv", mergeArr, recordCount);

    srand((unsigned)time(NULL));
    quickSortRec(quickArr, 0, recordCount - 1);
    writeSortedToFile("quick_sorted.csv", quickArr, recordCount);

    printf("\n--- Sorting statistics ---\n");
    printf("Algorithm         Comparisons   Swaps\n");
    printf("Merge Sort        %-12lld %-8lld\n", mergeComparisons, mergeSwaps);
    printf("Randomized Quick  %-12lld %-8lld\n", quickComparisons, quickSwaps);

    free(mergeArr);
    free(quickArr);
    free(temp);
}

RainfallRecord sortedRecords[MAX_RECORDS];
int sortedCount = 0;

int lower_bound(RainfallRecord arr[], int n, double val) {
    int l = 0, r = n;
    while (l < r) {
        int mid = (l + r) / 2;
        if (arr[mid].annual < val)
            l = mid + 1;
        else
            r = mid;
    }
    return l;
}

int upper_bound(RainfallRecord arr[], int n, double val) {
    int l = 0, r = n;
    while (l < r) {
        int mid = (l + r) / 2;
        if (arr[mid].annual <= val)
            l = mid + 1;
        else
            r = mid;
    }
    return l;
}

void doPartC() {
    if (sortedCount == 0) {
        RainfallRecord *tempArr = (RainfallRecord*)malloc(recordCount * sizeof(RainfallRecord));
        RainfallRecord *temp = (RainfallRecord*)malloc(recordCount * sizeof(RainfallRecord));
        if (!tempArr || !temp) {
            printf("Memory error.\n");
            free(tempArr); free(temp);
            return;
        }
        memcpy(tempArr, records, recordCount * sizeof(RainfallRecord));
        mergeSortRec(tempArr, 0, recordCount - 1, temp);
        memcpy(sortedRecords, tempArr, recordCount * sizeof(RainfallRecord));
        sortedCount = recordCount;
        free(tempArr);
        free(temp);
    }

    double low, high;
    printf("Enter lower bound of annual rainfall: ");
    scanf("%lf", &low);
    printf("Enter upper bound of annual rainfall: ");
    scanf("%lf", &high);

    int l = lower_bound(sortedRecords, sortedCount, low);
    int r = upper_bound(sortedRecords, sortedCount, high);

    if (l >= r) {
        printf("No records found in the given range.\n");
        return;
    }

    printf("\nRecords with annual rainfall between %.1f and %.1f:\n", low, high);
    printf("%-6s %8s\n", "YEAR", "ANNUAL");
    for (int i = l; i < r; i++) {
        printf("%-6d %8.1f\n", sortedRecords[i].year, sortedRecords[i].annual);
    }
}

int main() {
    if (readCSV("A2.1_rainfall.csv") <= 0) {
        printf("No data loaded. Check file.\n");
        return 1;
    }

    int choice;
    do {
        printf("\n========== MENU ==========\n");
        printf("1. Display all records\n");
        printf("2. Find max/min rainfall for a month (linear & D&C)\n");
        printf("3. Sort by annual rainfall (Merge & Quick)\n");
        printf("4. Search annual rainfall range (Binary Search)\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayAll();
                break;
            case 2:
                doPartA();
                break;
            case 3:
                doPartB();
                break;
            case 4:
                doPartC();
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (choice != 0);

    return 0;
}