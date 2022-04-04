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
    export DOCKTIE_COMMON_PARENTDIR="$(dirname $(pwd))"      ## for docktie-ext use
    DOCKTIE_DIR="${DOCKTIE_COMMON_PARENTDIR}/docktie"
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

export PROJECT_ROOTDIR="${DOCKTIE_COMMON_PARENTDIR}/${PROJECT_ALIAS}" ## for docktie-ext use

## Implement https://github.com/icasimpan/docktie/issues/11
##           Add relative custom location of docker-compose.yml per project
##
DOCKERCOMPOSE_FULLPARENT_DIR="$(pwd)"
[[ "$RELATIVE_DOCKERCOMPOSE_DIR" != "" ]] && DOCKERCOMPOSE_FULLPARENT_DIR="$(pwd)/${RELATIVE_DOCKERCOMPOSE_DIR}"

## Make sure docker-compose is installed since it's needed by the wrapper utils/docker-compose
##
if [[ "$DOCKTIE_INIT" != "true" ]]; then
   DOCKTIE_DOCKER_COMPOSE_BIN="$(which docker-compose 2>/dev/null)"
   if [[ "$DOCKTIE_DOCKER_COMPOSE_BIN" = "" ]]; then
       echo '[ERROR] docker-compose not found. Please make sure to install it first.'
       echo
       exit 1
   else
       export DOCKTIE_DOCKER_COMPOSE_BIN
   fi
fi

BOOTSTRAP_DOCKTIE() {
   if [[ "$DOCKTIE_INIT" = "true" ]]; then
      echo "ERROR: DockTie Dev Helper already initialized for '${PROJECT_NAME}'."
      echo "       Type 'exit' if you want to re-initialize."
      echo
   else
      export DOCKTIE_INIT=true
      TOOLS_DIR=$DOCKTIE_DIR/utils/bin
      echo "NOTE: DockTie Dev Helper initialized."
      echo "      Utils($TOOLS_DIR) prioritized in \$PATH"
      echo
      export PATH=$TOOLS_DIR:$PATH
      export DOCKER_CONTAINER_PREFIX=$DOCKER_CONTAINER_PREFIX  ## needed in ps1 change visibility done in next line
      bash --rcfile <(cat $CONF_DIR/ps1_changer)
      ## Below will run after 'exit' command
      echo "DockTie Dev Helper exited."
      echo
   fi
} ## END: BOOTSTRAP_DOCKTIE()

## Docker container(s) running...
##
export DOCKTIE_DOCKER_COMPOSE_FULLPATH="${DOCKERCOMPOSE_FULLPARENT_DIR}/docker-compose.yml"
if [[ "$(docker-compose -f ${DOCKTIE_DOCKER_COMPOSE_FULLPATH} ps | tail -n +2 | grep -cv exit)" -gt 0 ]]; then
  BOOTSTRAP_DOCKTIE

## Docker container(s) NOT running...
##
else
   if [[ "$AUTOSTART_PROJECT" = "1" ]]; then
      $DOCKTIE_DOCKER_COMPOSE_BIN -f $DOCKTIE_DOCKER_COMPOSE_FULLPATH up -d
      BOOTSTRAP_DOCKTIE
   else
       echo "ERROR: ${PROJECT_NAME} container(s) not running."
       echo '       These commands might help:'
       echo '          docker-compose up -d'
       echo '          docker-compose start'
       echo
       echo "       You may also try AUTOSTART_PROJECT=1 in ${DOCKTIE_ENV}"
       echo '       if project can start without manual intervention.'
       echo
       exit 1
   fi
fi
