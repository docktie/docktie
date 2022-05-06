##~ Shows help text of each available command.
##~
##~ Usage: docktie_cli help <cmd>
##~  where <cmd> is as follows:
##~    cp           copies files from current directory to container and vice versa
##~    logs         show container logs
##~    services     show project service names
##~    shell        shell-in to a one of your project containers
##~    list         shows all utilities available
##~    env          shows relevant environment variables
##~
cli_help() {
  autoload_functions "get_cmd_helptext"

  cmd2check=${1:-"help"}

  get_cmd_helptext ${DOCKTIE_KERNEL_MODULE_LIB}/cli_${cmd2check}.bash.inc
} ## END: cli_help()
