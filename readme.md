# What is DockTie

Is a simple wrapper script for commands inside docker container(s).

So for instance, what you would normally run as:

```
     docker-compose exec node node --v
```
becomes
```
     node --version
```

after tool is configured and initialized.

# How to Use

1. Clone the code from same parent directory as your project.

```
~$ pwd
/my/work/projects/

~$ git clone https://github.com/docktie/docktie.git
```

2. Configure

Assuming your project is named `cooltool`, create corresponding `_env` file
```
~$ pwd
/my/work/projects/
~$ cd docktie
~$ cp conf/env.sample conf/cooltool_env ## change the variables accordingly.
```
NOTE: check `conf/env.sample`. Depending on your site structure, you may need to define `RELATIVE_DOCKERCOMPOSE_DIR`.

3. And initialize:

```
~$ pwd
/my/work/projects/cooltool
~$ ../docktie/init.sh
NOTE: DockTie Dev Helper initialized.
      Utils(/my/work/projects/docktie/utils) prioritized in $PATH

(docktie > cooltool) ~$
```
You will get a visual indicator like "(docktie > cooltool)" in your CLI prompt once you successfully initialized.

NOTE: It has to be run from within your project. Otherwise, you will see this error:
```
~$ pwd
/my/work/projects/docktie
~$ ./init.sh
ERROR: 'init.sh' must be called from within project directory.
```

# When you're done
Simply run the `exit` command:
```
(docktie > cooltool) ~$ exit
exit
DockTie Dev Helper exited.

~$
```

# Utils Customization

## Utils Structure
The `utils` are inside "utils/bin" directory.

Notice that in majority of utils, only docktie_cli (utils/docktie_cli) is actually calling the core (lib/kernel/core.bash.inc).
This is because, only the script name matters.

```
~$ nl -ba utils/bin/docktie_cli
     1  #!/bin/bash
     2
     3  ## -------- source common library ---------
     4  PROJECT_ROOTDIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )"/../ && pwd )"
     5  . $PROJECT_ROOTDIR/etc/controller.bash.inc
     6
     7
     8  ## list of functions to auto-load
     9  autoload_functions "kernel/core"
    10
    11  ## ..............................
    12  ## main utility tool starts below
    13  ## ..............................
    14  core $(basename $0) $*
```

In line 3 above,
```
$(basename $0)
```
is the script name which works for symbolic links too. This makes maintenance easier and can be automated later.

Now on the utils kernel side `lib/kernel/core.bash.inc`, except for docktie_cli, a corresponding config is
also expected and sourced/included. So `utils/bin/artisan` MUST have a corresponding `etc/docktie/utils/artisan.conf`.


## Utils Example
For a concrete example, say you want to add a `psql` util located on `postgres` container/service.

All you have to do is:

1. Add the symlink
```
~$ pwd
/my/work/projects/docktie/utils/bin
~$ ln -s docktie_cli postgres
~$
```

2. And the kernel config file `etc/docktie/postgres.conf` with below contents:
```
docker_service_name='postgres'
command_within_service="psql"
```

Test and enjoy!

# Limitation

Your project must be described in docker-compose format. Pure Dockerfile is not supported yet.

# Pull Requests

Send in Pull Request(s) if you have ideas or saw bugs. Thanks in advance :)

# Credits

As tool grows, so does its dependency hell. It was thus refactored for easier maintenance and followed the [SHCF](https://github.com/icasimpan/shcf.git)-way.
