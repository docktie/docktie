##~ Shows services depending on status or all of them.
##~
##~ Usage: docktie_cli services <status>
##~   where <status> is either
##~     running     shows all running services
##~     exited      shows all exited services
##~     all         shows both running and exited (default)
##~
cli_services() {
      local service_type=$*

      srv_state=$service_type
      if [[ -z $service_type ]] || [[ "$service_type" = "all" ]]; then
          srv_state="running exited"
      fi

      for each_state in $srv_state; do
          srv_list=$running_services
          [[ "$each_state" = 'exited' ]] && srv_list=$exited_services
          echo "All '$each_state' services as follows:"
          for each_container in $(echo $srv_list | tr "_" "\n"); do
              echo " * $each_container"
          done
      done
} ## END: cli_services()
