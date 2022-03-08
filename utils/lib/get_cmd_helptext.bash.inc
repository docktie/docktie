get_cmd_helptext() {
    local fullpath_file=$1

    grep ^##~ $fullpath_file | sed 's/^##~//'
} ## END: get_cmd_helptext()
