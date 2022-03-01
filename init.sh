## DockTie - (Dock)er Shor(tie) simplifies running commands within docker container(s).
##
#!/bin/bash

## Enforce calling "init.sh" from within project directory.
##
DOCKTIE_DIR=$(dirname $0)
if [[ "$DOCKTIE_DIR" != "../docktie" ]]; then
    echo "ERROR: 'init.sh' must be called from within project directory."
    exit 1
else
    DOCKTIE_DIR="$(dirname $(pwd))/docktie"
fi

## Read project config
##
PROJECT_ALIAS=${1:-"$(basename $(pwd))"}

CONF_DIR="$DOCKTIE_DIR/conf"
ENV_FILE="$CONF_DIR/${PROJECT_ALIAS}_env"
if [[ ! -e $ENV_FILE ]]; then
    echo 'ERROR: Invalid parameter.'
    echo 'Valid parameter(s) as follows:'
    for alias_name in $(ls -1 $CONF_DIR/*_env|awk -F'/' '{print $NF}'|sed 's/_env$//g'); do
        echo "  * $alias_name"
    done
    exit 1
fi

export DOCKTIE_ENV="$ENV_FILE"
. $DOCKTIE_ENV

## Docker container(s) running...
export DOCKTIE_DOCKER_COMPOSE_FULLPATH="$(pwd)/${PROJECT_CONTAINER_PREFIX}/docker-compose.yml"
if [[ "$(docker-compose -f ${DOCKTIE_DOCKER_COMPOSE_FULLPATH} ps | tail -n +2 | grep -cv exit)" -gt 0 ]]; then
   if [[ "$DOCKTIE_INIT" = "true" ]]; then
      echo "ERROR: DockTie Dev Helper already initialized."
      echo "       Type 'exit' if you want to re-initialize."
      echo
   else
      export DOCKTIE_INIT=true
      TOOLS_DIR=$DOCKTIE_DIR/utils
      echo "NOTE: DockTie Dev Helper initialized."
      echo "      Tools are currently inside $TOOLS_DIR"
      echo
      export PATH=$TOOLS_DIR:$PATH
      bash
      ## Below will run after 'exit' command
      echo "DockTie Dev Helper exited."
      echo
   fi

## Docker container(s) NOT running...
else
   echo "ERROR: ${PROJECT_NAME} container(s) not running."
   echo '       These commands might help:'
   echo '          docker-compose up -d'
   echo '          docker-compose start'
   echo
   exit 1
fi
