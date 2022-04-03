enforce_projectdir_docktie_init() {
    DOCKTIE_DIR=$(dirname $0)
    if [[ "$DOCKTIE_DIR" != "../docktie" ]]; then
        echo "ERROR: 'init.sh' must be called from within project directory."
        exit 1
    fi
} ## END: enforce_projectdir_docktie_init()
