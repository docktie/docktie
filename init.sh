## DockTie - (Dock)er Shor(tie) simplifies running commands within docker container(s).
##
#!/bin/bash

## -------- source common library ---------
SHCF_ROOTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
. $SHCF_ROOTDIR/etc/controller.bash.inc

## list of functions to auto-load
functions_list="
enforce_projectdir_docktie_init
read_project_config
bootstrap_docktie
"
autoload_functions "$functions_list"

## Enforce calling "init.sh" from within project directory.
##
enforce_projectdir_docktie_init

export DOCKTIE_COMMON_PARENTDIR="$(dirname $(pwd))"      ## for docktie-ext use
DOCKTIE_DIR="${DOCKTIE_COMMON_PARENTDIR}/docktie"

PROJECT_ALIAS="$(basename $(pwd))"
## Read project config
##
read_project_config $PROJECT_ALIAS

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

## Docker container(s) running...
##
export DOCKTIE_DOCKER_COMPOSE_FULLPATH="${DOCKERCOMPOSE_FULLPARENT_DIR}/docker-compose.yml"
if [[ "$(docker-compose -f ${DOCKTIE_DOCKER_COMPOSE_FULLPATH} ps | tail -n +2 | grep -cv exit)" -gt 0 ]]; then
  bootstrap_docktie

## Docker container(s) NOT running...
##
else
   if [[ "$AUTOSTART_PROJECT" = "1" ]]; then
      $DOCKTIE_DOCKER_COMPOSE_BIN up -d
      bootstrap_docktie
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
