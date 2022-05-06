##~ Shows shell environment variables
##~
##~ Usage: docktie_cli env <env_type>
##~   where <env_type> is either
##~     project     project specific defined in *_env
##~     docktie     platform specific defined in init.sh
##~     all         shows both project and docktie (default)
##~
cli_env() {
      local env_type=${1:-'all'}

      env_type=${env_type^^} ## Convert to uppercase. NOTE: Bash 4+ specific

      ## validate input. MUST be as defined in help context above
      ##
      if [[ "$env_type" =~ ALL|PROJECT|DOCKTIE ]]; then
          case $env_type in
             ALL)
                 env|grep "^PROJECT\|^DOCKTIE"|sort
             ;;
             PROJECT)
                 env|grep "^PROJECT"|sort
             ;;
             DOCKTIE)
                 env|grep "^DOCKTIE"|sort
             ;;
             *)
                echo "Oops, something is wrong."
             ;;
          esac
      else
          echo "ERROR: Invalid parameter. See 'docktie_cli help env'"
      fi

} ## END: cli_env()
