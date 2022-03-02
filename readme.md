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

~$ git clone https://github.com/icasimpan/docktie.git
```

2. Configure

Assuming your project is named `cooltool`, create corresponding `_env` file
```
~$ pwd
/my/work/projects/
~$ cd docktie
~$ cp conf/env.sample conf/cooltool_env ## change the variables accordingly.
```

3. And initialize:

```
~$ pwd
/my/work/projects/cooltool
~$ ../docktie/init.sh
NOTE: DockTie Dev Helper initialized.
      Tools are currently inside /my/work/projects/docktie/utils

(docktie) ~$
```
You will get a visual indicator "(docktie)" in your CLI prompt once you successfully initialized the tool.

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
(docktie) ~$ exit
exit
DockTie Dev Helper exited.

~$
```

# Utils Customization

The `utils` are inside "utils" directory. Focus only on the core script `utils/kernel/core`.
This was initially made for a laravel project.

Follow the pattern in core and add the corresponding service. For example, if you have
a `postgres` service:

```
~$ pwd
/my/work/projects/docktie/utils
~$ ln -s shell postgres
## ...
## ...
## ...
## Then, in 'core' case statement before the '*)', create the following:
##         postgres)
##           docker_service_name='postgres'
##           command_within_service="$docker_service_name"
##        ;;
##
```

Notice that in majority of utils, only shell (utils/shell) is actually calling the core (utils/kernel/core).
This is because, only the script name matters.

```
~$ nl -ba utils/shell
     1  #!/bin/bash
     2
     3  $(dirname $0)/kernel/core $(basename $0) $*
```

In line 3 above,
```
$(basename $0)
```
is the script name which works for symbolic links too. This makes maintenance easier and can be automated later
when the enhancement issue "#1 (Possible decoupling of services handled)" is fully implemented.

Test and enjoy!

# Limitation

Your project must be described in docker-compose format. Pure Dockerfile is not supported yet.

# Pull Requests

Send in Pull Request(s) if you have ideas or saw bugs. Thanks in advance :)
