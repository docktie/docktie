## NOTE: kernel module 'cli' (answering for 'docktie_cli' name) is sourced/included by 'core.bash.inc'.
cli() {
    local sub_cmd=${1:-"help"}; shift ## Get 1st parameter (defaults to 'help'). Then, pop-off from $*
          sub_cmd=${sub_cmd,,}        ## ...ensure parameter is lowercase.

    ## DOCKTIE_COMMON_PARENTDIR is usually initialized when ../docktie/init.sh is run.
    ## But since `docktie_cli getx` is allowed to run outside docktie_env, we need to
    ## define it here
    if [[ "$DOCKTIE_COMMON_PARENTDIR" = "" ]]; then
        DOCKTIE_COMMON_PARENTDIR=$(dirname $(dirname $TOOL_ROOTDIR))
    fi

    ## 'docktie_cli getx' don't need a container to run ...
    ##
    if [[ "$sub_cmd" != "getx" ]]; then
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
     fi

    # 'docktie_cli getx' was allowed to run outside docktie environment.
    #  Therefore, it is imperative for autoload_functions to be called
    #  Only when needed as below. Otherwise, errors like 'unknown flag: --services'
    #  are seen
    #
    local lib_relpath='kernel/module/lib'
    case $sub_cmd in
        help)
          autoload_functions "${lib_relpath}/cli_help"
          cli_help $*
        ;;

        list)
          autoload_functions "${lib_relpath}/cli_list"
          cli_list $*
        ;;
 
        ## works!
        shell|sh)
          autoload_functions "${lib_relpath}/cli_shell"
          cli_shell $*
        ;; 

        ##    Default is show all (broken into running and NOT running)
        ##    Other option 'running' & 'not_running'
        services)
          autoload_functions "${lib_relpath}/cli_services"
          cli_services $* 
        ;;

        ## works!
        cp|copy)
           autoload_functions "${lib_relpath}/cli_cp"
           cli_cp $*
        ;;

        ## works!
        logs|log)
           autoload_functions "${lib_relpath}/cli_logs"
           cli_logs $*
        ;;

        getx)
           autoload_functions "${lib_relpath}/cli_getx"
           cli_getx $*
        ;;

        env)
           autoload_functions "${lib_relpath}/cli_env"
           cli_env $*
        ;;
        *)
           echo "Oops, unknown 'docktie_cli' sub-command"
        ;;
    esac
 
} ## END: cli()
