#include <stdio.h>
#include <string.h>
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

    // Check for path where battery data is saved
    // Some laptops will have it set in BAT0 & some in BAT1
    char path[64] = { 0 };
    FILE* fp = fopen("/sys/class/power_supply/BAT0", "r");
    if (fp != NULL)
        strcat(path, "/sys/class/power_supply/BAT0");
    else
        strcat(path, "/sys/class/power_supply/BAT1");

    if (fp)
        fclose(fp);

    for (;;sleep(1)) 
    {
        // Read current battery capacity
        char* cp = readfile(path, "capacity");
        sscanf(cp, "%d", &cap);
        free(cp);
 
        // Read current battery status (Charing/Discharging)
        cp = readfile(path, "status");
        if(!strncmp(cp, "Discharging", 11))
        {
            // If discharing and batery cap is less than threshold then send low battey notif
            if(cap <= LOW_BATTERY_WARNING_THRESHOLD && (flag & 1) == 0 && notify_init("Low battery notification"))
            {
                // Set flag to one which means notification has been sent and won't bother you again
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
