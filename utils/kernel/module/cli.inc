## NOTE: kernel module 'cli' (answering for 'docktie_cli' name) is being called from 'core'.
##    Therefore, $0 /path/to/utils/kernel/core
##
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
      __cp
      __logs
      __shell
      __services
      __help
      __list
      __getx
    "
    for each_lib in $lib2include; do
        . $(dirname $0)/module/lib/cli/${each_lib}
    done

    case $sub_cmd in
        help)
          __help $*
        ;;

        list)
          __list $*
        ;;
 
        ## works!
        shell)
          __shell $*
        ;; 

        ##    Default is show all (broken into running and NOT running)
        ##    Other option 'running' & 'not_running'
        services)
          __services $* 
        ;;

        ## works!
        cp)
           __cp $*
        ;;

        ## works!
        logs)
           __logs $*
        ;;

        getx)
           __getx $*
        ;;

        *)
           echo "Oops, unknown 'docktie_cli sub-command"
        ;;
    esac
 
} ## END: cli()
