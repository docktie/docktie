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

(docktie > cooltool) ~$
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
(docktie > cooltool) ~$ exit
exit
DockTie Dev Helper exited.

~$
```

# Utils Customization

## Utils Structure
The `utils` are inside "utils" directory.

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

Now on the util kernel side, `utils/kernel/core`. This is also including files from it's config directory.
So `utils/shell` has a corresponding `utils/kernel/conf/shell.conf`.

## Utils Example
For a concrete example, say you want to add a `postgres` util.

All you have to do is:

1. Add the symlink
```
~$ pwd
/my/work/projects/docktie/utils
~$ ln -s shell postgres
~$
```

2. And the kernel config file `utils/kernel/conf/postgres.conf` with below contents:
```
docker_service_name='postgres'
command_within_service="$docker_service_name"
```

Test and enjoy!

# Limitation

Your project must be described in docker-compose format. Pure Dockerfile is not supported yet.

# Pull Requests

Send in Pull Request(s) if you have ideas or saw bugs. Thanks in advance :)
