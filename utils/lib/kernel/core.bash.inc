core() {
        autoload_functions "kernel/module/cli"

        ## To allow exempting 'docktie_cli getx' per https://github.com/docktie/docktie/issues/27
	script_name=$1
        script_subcmd=$2
	shift

	## Environment checking dependent on init.sh being run ...
	if [[ "$DOCKTIE_INIT" = "true" ]]; then
		DOCKER_COMPOSE_BIN="docker-compose -f $DOCKTIE_DOCKER_COMPOSE_FULLPATH"
	elif [[ "$script_name" != "docktie_cli" ]] || [[ "$script_subcmd" != "getx" ]]; then
		echo 'ERROR: DockTie needs to be initialized.'
		exit 1
	fi

	## Every util except "docktie_cli" needs a config...
	SCRIPT_CONF="$DOCKTIE_UTILS_CONF/${script_name}.conf"
	if [[ "$script_name" != "docktie_cli" ]]; then
		if [[ -e "$SCRIPT_CONF" ]]; then
			. $SCRIPT_CONF
		else
			echo "ERROR: Unconfigured util name ($script_name). See '${DOCKTIE_UTILS_CONF}/artisan.conf' for reference in creating one."
			echo
			exit 1;
		fi
	fi

	## ------
	## main()
	## ------
	case $script_name in
	   "composer")
		   $DOCKER_COMPOSE_BIN run --rm $docker_service_name $*
	   ;;

	   "docktie_cli")
		   cli $*
	   ;;

	   *)
		   if [[ $# -eq 0 ]]; then
			  echo "ERROR: parameter to '$script_name' missing."
			  echo
			  exit 1
		   else
			  DOCKER_CONTAINER="$DOCKER_COMPOSE_BIN exec $docker_service_name"
			  $DOCKER_CONTAINER $command_within_service $*
		   fi
	   ;;
	esac
} ## END: core()
