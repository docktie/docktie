## NOTE: kernel module 'cli' (answering for 'docktie_cli' name) is sourced/included by 'core.bash.inc'.
cli() {
    local sub_cmd=${1,,}; shift ## Get 1st paramenter and convert to lowercase
                                ## Then, pop-off from $*

    DOCKER_COMPOSE_BIN="docker-compose -f $DOCKTIE_DOCKER_COMPOSE_FULLPATH"

    ## TODO: Check for '_' in service names. If it exist, replace it with something else...
    ##
    running_services="$($DOCKER_COMPOSE_BIN ps --services --status running | tr '\n' '_')"
    exited_services="$( $DOCKER_COMPOSE_BIN ps --services --status exited  | tr '\n' '_')"

    ## TODO: Check if this will not give false positive for partial match of container name.
    ##       e.g. 'mysql' MUST NOT return 1 for 'mysql5'
    container_running() {
        container=$1

        retval=0
        if [[ $container != "" ]]; then
            retval=$(echo $running_services | tr "_" "\n" | grep -c ^${container}$)
        fi
 
        echo $retval
    }

    lib2include="
      kernel/module/lib/cli_cp
      kernel/module/lib/cli_logs
      kernel/module/lib/cli_shell
      kernel/module/lib/cli_services
      kernel/module/lib/cli_help
      kernel/module/lib/cli_list
      kernel/module/lib/cli_getx
    "
    autoload_functions "$lib2include"

    case $sub_cmd in
        help)
          cli_help $*
        ;;

        list)
          cli_list $*
        ;;
 
        ## works!
        shell|sh)
          cli_shell $*
        ;; 

        ##    Default is show all (broken into running and NOT running)
        ##    Other option 'running' & 'not_running'
        services)
          cli_services $* 
        ;;

        ## works!
        cp|copy)
           cli_cp $*
        ;;

        ## works!
        logs|log)
           cli_logs $*
        ;;

        getx)
           cli_getx $*
        ;;

        *)
           echo "Oops, unknown 'docktie_cli sub-command"
        ;;
    esac
 
} ## END: cli()
