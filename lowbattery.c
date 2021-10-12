#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>

#define LOW_BATTERY_WARNING_THRESHOLD 20

char* readfile(char* base, char* file)
{
    char path[512];
    char *line = (char*) malloc(512 * sizeof(char));
    memset(line, 0, 512 * sizeof(char));

    snprintf(path, 512, "%s/%s", base, file);
    FILE* fp = fopen(path, "r");

    if(fp == NULL)
        return NULL;
    if(fgets(line, 511, fp) == NULL)
        return NULL;

    fclose(fp);

    return line;
}

int main() 
{
    int cap;
    int flag = 0;
    for (;;sleep(1)) 
    {
        char* cp = readfile("/sys/class/power_supply/BAT1", "capacity");
        sscanf(cp, "%d", &cap);
        free(cp);
 
        cp = readfile("/sys/class/power_supply/BAT1", "status");
        if(!strncmp(cp, "Discharging", 11))
        {
            if(cap <= LOW_BATTERY_WARNING_THRESHOLD && (flag & 1) == 0)
            {
                flag |= 1;
                char* signal = "notify-send -u critical \"Battery Low\"";
                system(signal);
            }
        } 
        else if(!strncmp(cp, "Charging", 8))
        {
            if((flag & 1))
            {
                flag = 0;
            }
        }
        free(cp);
    }

    return 0;
}
