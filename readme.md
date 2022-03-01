# What is DockTie

Is a simple wrapper script for commands that resides in docker containers.

So for instance, what you would normally run as:

     `docker-compose exec node node --v`

becomes:

     `node --version`

after tool is configured and initialized.

# How to Use

1. Clone the code from same parent directory as your project.

```
~$ pwd
/my/work/project/

~$ git clone https://github.com/icasimpan/docktie.git
```

2. Configure

Assuming your project is named `cooltool`, create corresponding `_env` file
```
~$ pwd
/my/work/project/
~$ cd docktie
~$ cp conf/env.sample conf/cooltool_env ## change the variables accordingly.
```

3. And initialize:

```
~$ pwd
/my/work/project/cooltool
~$ ../docktie/init.sh
NOTE: DockTie Dev Helper initialized.
      Tools are currently inside /my/work/docktie/utils
```

NOTE: It has to be run from within your project. Otherwise, you will see this error:
```
~$ pwd
/my/work/project/docktie
~$ ./init.sh
ERROR: 'init.sh' must be called from within project directory.
```
To fix, go to your project directory just like what step 3 is showing above.

# When you're done
Simply run the command from anywhere:
```
exit
```

# Utils Customization

The `utils` are inside "utils" directory. Focus only on the core script `utils/core`.
This was made for a laravel project but can be configured for other services.

Follow the pattern in core and create corresponding service. For example, if you have 
a `postgres` service:

```
~$ pwd
/my/work/project/docktie/utils
~$ cp artisan postgres
## ...
## ...
## ...
## Then, in 'core' case statement before the '*)', create the following:
##         postgres)
##           docker_service_name='postgres'
##           command_within_service="$docker_service_name"
##        ;;
##
## Test and enjoy!
```

# Limitation

Your project must be described in docker-compose format. Pure Dockerfile is not supported (well, you can send a Pull Request).
