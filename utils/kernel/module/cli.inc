cli() {

    echo "<TBD> Implement 'docktie_cli' here (utils/kernel/module/cli.inc)"
    echo " Parameter(s) I got:"
    for x in $*; do
        echo $x
    done
    echo '------------------------'
  
    local sub_cmd=${1,,}; shift ## Get 1st paramenter and convert to lowercase
                                ## Then, pop-off from $*

    DOCKER_COMPOSE_BIN="docker-compose -f $DOCKTIE_DOCKER_COMPOSE_FULLPATH"

    ## TODO: Check for '_' in service names. If it exist, replace it with something else...
    ##       There are a lot of services status, see from 'docker-compose ps --help'
    ##           --status stringArray   Filter services by status. Values: [paused | restarting | removing | running | dead | created | exited]
    ##       Only show the status if it has output
    ##
    running_services="$($DOCKER_COMPOSE_BIN ps --services --status running|tr '\n' '_')"

    ## TODO: Check if this will not give false positive for partial match of container name.
    ##       e.g. 'mysql' MUST NOT return 1 for 'mysql5'
    container_running() {
        container=$1
        echo $running_services | tr "_" "\n" | grep -c ^${container}$
    }


    __cp() {
        src=$1
        dest=$2

        $DOCKER_COMPOSE_BIN cp $src $dest
    } ## END: __cp()


    ## all containers parameter is possible
    ## TODO: Add option for "all containers" to not show header for easy grepping.
    __logs() {
        container=$1

        if [[ -z $container ]]; then
            for each_container in $(echo $running_services | tr "_" "\n"); do
                echo '+----------------------------------------------+'
                echo "| Log for service '$each_container'             "
                echo '+----------------------------------------------+'
                $DOCKER_COMPOSE_BIN logs $each_container
            done|more
        else
            container_alive=$(container_running $container)
            if [[ $container_alive -eq 1 ]]; then
                echo '+----------------------------------------------+'
                echo "| Log for service '$container'                 "
                echo '+----------------------------------------------+'
                $DOCKER_COMPOSE_BIN logs $container
            else
                echo "[INFO] Cannot show logs for container '$container' as it is not running..."
            fi
        fi
    }

    __shell() {
         container=$1
 
         container_alive=$(container_running $container)
         if [[ $container_alive -eq 1 ]]; then
             echo '+----------------------------------------------+'
             echo "| Shell-ing into service '$container'          "
             echo "|    Use 'exit' to get off.                    "
             echo '+----------------------------------------------+'
             $DOCKER_COMPOSE_BIN exec $container sh
         else
             echo "[INFO] Cannot shell into '$container' as it is not running..."
         fi
    }

    __services() {
          echo 'All running services as follows:'
          for each_container in $(echo $running_services | tr "_" "\n"); do
              echo " * $each_container"
          done
    }

    case $sub_cmd in
        help)
          :
        ;;

        list)
          :
        ;;
 
        ## works!
        shell)
          __shell $*
        ;; 

        ## TODO: show all docker services
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
           :
        ;;
    esac
 
} ## END: cli()
