##~ Used to download utils from external repo, currently located in
##~   https://github.com/docktie/docktie-ext
##~
##~ Usage: docktie_cli getx <tool_identity>
##~   where tool_identity    format '<brand>/<tool_name>/<version_string>'
##~                            * brand - e.g. mysql, postgres, etc
##~                            * tool_name (e.g. backup_db)
##~                            * version_string MUST be:
##~                                > @stable (default)
##~                                > @wip
##~                                > @v0.0.1 (or whatever version is available)
##~
cli_getx() {
  local identifier="$1" ## Consists of '<brand>/<tool_name>/<version_string>
                        ##   => brand - mysql, postgres, etc
                        ##   => Tool name (e.g. backups_db).
                        ##   => Version string MUST be like:
                        ##        * '@stable' (default)
                        ##        * '@wip'
                        ##        * '@v0.0.1'

  local BIN_DEST="$DOCKTIE_COMMON_PARENTDIR/docktie/utils/bin"

  ## Detect if there's a version string
  ver_string=$(echo $identifier|cut -d'@' -f2)

  ## Get non-version string
  tool_name=$(echo $identifier|cut -d'@' -f1)
  tool_alias=$(echo $tool_name|cut -d'/' -f2) ## tool's main filename only, no brand (e.g. backup.sh)
  ## ver_string default to 'stable' IF nothing mentioned
  [[ "$ver_string" = "$tool_name" ]] && ver_string='stable'

  tool_branch=$(git ls-remote $DOCKTIE_EXT_REPO|grep ${tool_name}/${ver_string}|awk '{print $NF}'|sed 's|^refs/heads/||')

  if [[ "$tool_branch" != "" ]]; then
      ##  Make sure only 1 is detected.
      if [[ $(echo $tool_branch|wc -w) -eq 1 ]]; then
         echo "Tool exist from repo: $tool_branch"
         local tempdir=$(mktemp -d /tmp/docktie_XXXX)
         git clone -q --branch $tool_branch $DOCKTIE_EXT_REPO $tempdir
         local thebin=$(find $tempdir|grep $tool_alias|grep -v .git)

         ## Ensure same file not yet in $BIN_DEST
         local thebin_basename=$(basename $thebin)
         local possibly_installed=$BIN_DEST/${thebin_basename}

         if [[ "$(ls -1 $possibly_installed 2>/dev/null)" != "" ]]; then
             hash0=$(sha256sum ${possibly_installed}|awk '{print $1}')
             hash1=$(sha256sum ${thebin}|awk '{print $1}')
             if [[ "$hash0" = "$hash1" ]]; then
                 echo "Oops, it's already installed."
             else
                 echo "Util with same name exist but content is not similar."
                 echo "To proceed: delete it first and rerun 'docktie_cli getx'"
                 echo
             fi
         else
             cp -v $thebin $BIN_DEST
         fi

         rm -rf $tempdir
      else
         echo "ERROR: Multiple tools detected."
         for x in $tool_branch; do
            echo "* ${x}"
         done
      fi
   else
      echo "No branch detected. Are you sure you don't have a typo?"
   fi
} ## END: cli_getx()
