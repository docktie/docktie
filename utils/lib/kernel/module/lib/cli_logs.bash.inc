##~ Usage: docktie_cli logs <service_name> <verbosity>
##~    where:
##~       service_name     Any active service name of your project or 'all' or '-' (default)
##~       verbosity        Remove extra header to show what service log is being shown
##~                          Valid values are 'silent' or 'quiet'
##~ 
##~ Shows the logs of an active service.
##~
cli_logs() {
   local service=$1
   local verbose=${2,,}   ## value allowed: 'silent' or 'quiet' (defaults to non-silent)

   if [[ -z $service ]] || [[ "$service" =~ all|- ]]; then
       if [[ "$verbose" =~ silent|quiet ]]; then
           $DOCKER_COMPOSE_BIN logs
       else
           for each_service in $(echo $running_services | tr "_" "\n"); do
               echo '+----------------------------------------------+'
               echo "| Log for service '$each_container'             "
               echo '+----------------------------------------------+'
               $DOCKER_COMPOSE_BIN logs $each_service
           done|more
       fi
   else
       service_alive=$(container_running $service)
       if [[ $service_alive -eq 1 ]]; then
           echo '+----------------------------------------------+'
           echo "| Log for service '$service'                 "
           echo '+----------------------------------------------+'
           $DOCKER_COMPOSE_BIN logs $service
       else
           echo "[INFO] Cannot show logs for service '$service' as it is not running..."
       fi
   fi
} ## END: cli_logs()
