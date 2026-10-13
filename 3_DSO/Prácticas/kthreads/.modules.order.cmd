cmd_/home/dso/kthreads/modules.order := {   echo /home/dso/kthreads/ejemplo_kthread.ko; :; } | awk '!x[$$0]++' - > /home/dso/kthreads/modules.order
