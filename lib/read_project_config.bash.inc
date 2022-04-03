read_project_config() {
    local PROJECT_ALIAS="$1"

    ENV_FILE="$DOCKTIE_INIT_CONFDIR/${PROJECT_ALIAS}_env"
    if [[ ! -e $ENV_FILE ]]; then
        echo 'ERROR: Invalid parameter.'
        echo 'Valid parameter(s) as follows:'
        for alias_name in $(ls -1 "$DOCKTIE_INIT_CONFDIR/*_env"|awk -F'/' '{print $NF}'|sed 's/_env$//g'); do
            echo "  * $alias_name"
        done
        exit 1
    fi

    export DOCKTIE_ENV="$ENV_FILE"
    . $DOCKTIE_ENV
} ## END: read_project_config()
