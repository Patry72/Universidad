cmd_/home/dso/ejemplo-timer-tasklet/modules.order := {   echo /home/dso/ejemplo-timer-tasklet/ejemplo_timer.ko; :; } | awk '!x[$$0]++' - > /home/dso/ejemplo-timer-tasklet/modules.order
