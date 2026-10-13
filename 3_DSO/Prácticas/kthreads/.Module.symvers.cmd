cmd_/home/dso/kthreads/Module.symvers := sed 's/\.ko$$/\.o/' /home/dso/kthreads/modules.order | scripts/mod/modpost -m -a  -o /home/dso/kthreads/Module.symvers -e -i Module.symvers   -T -
