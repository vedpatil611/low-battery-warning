#include <stdio.h>
#include <sys/file.h>
#include <errno.h>
#include <libnotify/notify.h>

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

    char file[256];
    strcat(strcpy(file, getenv("HOME")), "/.lowbattery_lock");
    int pid_file = open(file, O_CREAT | O_RDWR, 0666);
    int rc = flock(pid_file, LOCK_EX | LOCK_NB);
    if(rc) {
        if(EWOULDBLOCK == errno)				// another instance is already running
        {
            return 1;
        }
    }


    for (;;sleep(1)) 
    {
        char* cp = readfile("/sys/class/power_supply/BAT1", "capacity");
        sscanf(cp, "%d", &cap);
        free(cp);
 
        cp = readfile("/sys/class/power_supply/BAT1", "status");
        if(!strncmp(cp, "Discharging", 11))
        {
            if(cap <= LOW_BATTERY_WARNING_THRESHOLD && (flag & 1) == 0 && notify_init("Low battery notification"))
            {
                flag |= 1;
    
                NotifyNotification* notification = notify_notification_new("Battery Low", "Connect charger", NULL);
                notify_notification_set_urgency(notification, NOTIFY_URGENCY_CRITICAL);
                notify_notification_show(notification, NULL);

				g_object_unref(notification);
				notify_uninit();
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
