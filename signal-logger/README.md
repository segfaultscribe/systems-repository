# SIGNAL Logger

A simple C funtion to log signals sent to it to the terminal. 

### Usage

```
gcc logger.c -o lg
./lg
```

Open up another terminal

```
pidof lg    # get the process id, let it be <pid>
kill -SIGINT <pid>  # this will get logged in the terminla u ran ./lg
```

```
Replace -SIGINT with other signals
```