##~ Shells into an active service name. Uses 'bash'
##~
##~ Usage: docktie_cli shell <service_name>
##~
cli_shell() {
     local container=$1

     container_alive=$(container_running $container)
     if [[ $container_alive -eq 1 ]]; then
         echo '+----------------------------------------------+'
         echo "| Shell-ing into service '$container'          "
         echo "|    Use 'exit' to get off.                    "
         echo '+----------------------------------------------+'
         $DOCKER_COMPOSE_BIN exec $container bash
     else
         echo "[INFO] Cannot shell into '$container' as it is not running..."
     fi
} ## END cli_shell()
