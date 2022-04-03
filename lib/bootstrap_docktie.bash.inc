bootstrap_docktie() {
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
      bash --rcfile <(cat $DOCKTIE_INIT_CONFDIR/ps1_changer)
      ## Below will run after 'exit' command
      echo "DockTie Dev Helper exited."
      echo
   fi
} ## END: bootstrap_docktie()
