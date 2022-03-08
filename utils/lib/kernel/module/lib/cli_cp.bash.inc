##~ Usage: docktie_cli cp <src> <dest>
##~
##~ Copies a file from source to destination.
##~
##~ Either src or dest can be local or container-located files.
##~  * Local file        relative to current location
##~  * Container-based   follow format - container_name:/path/to/dest
##~
cli_cp() {
    local src=$1
    local dest=$2

    autoload_functions "get_cmd_helptext"

    if [[ $# -eq 2 ]]; then
      $DOCKER_COMPOSE_BIN cp $src $dest
    else
       echo 'ERROR: Invalid usage.'
       get_cmd_helptext ${DOCKTIE_KERNEL_MODULE_LIB}/cli_cp.bash.inc
    fi
} ## END: cli_cp()
