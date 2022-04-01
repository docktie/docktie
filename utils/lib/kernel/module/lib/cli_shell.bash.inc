##~ Shells into an active service name. Uses 'bash'
##~
##~ Usage: docktie_cli shell <service_name>
##~
cli_shell() {
     local container=$1

     local container_alive=$(container_running $container)

     # Downgrade to old Bourne Shell if 'bash' not detected.
     local designated_shell='bash'
     if [[ "$($DOCKER_COMPOSE_BIN exec $container which bash)" = "" ]]; then
         designated_shell='sh'
     fi
     ## TODO: Add ash from busybox if there's no shell detected per issue #20
     ## https://busybox.net/downloads/binaries/1.35.0-x86_64-linux-musl/busybox_ADD_SHELL

     if [[ $container_alive -eq 1 ]]; then
         echo '+----------------------------------------------+'
         echo "| Shell-ing into service '$container'          "
         echo "|    Use 'exit' to get off.                    "
         echo '+----------------------------------------------+'
         $DOCKER_COMPOSE_BIN exec $container $designated_shell
     else
         echo "[INFO] Cannot shell into '$container' as it is not running..."
     fi
} ## END cli_shell()
