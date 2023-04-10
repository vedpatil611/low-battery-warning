# low-battery-warning
## Light weight low battery warning popup for linux

Lowbattery warning will give low battery warning popup when battery is less than 20%. However you can simple edit macro value to whatever percentage you want and recompile it get notified at any specifed percentage.
<br/>
Current service script is only available for systemd.

## Prerequisite
* libnotify (make dependency)

## Steps to compile and use
* `sudo make clean install`
* `systemctl enable lowbattery.service`
* `systemctl start lowbattery.service`

## Arch Linux
Arch users can easily install form aur: https://aur.archlinux.org/packages/low-battery-warning-git
