#include <time.h>

int main()
{
    float startTime = (float)clock()/CLOCKS_PER_SEC;
    /* code here */
    float endTime = (float)clock()/CLOCKS_PER_SEC;

    printf("Time taken: %f\n", endTime - startTime);
}
