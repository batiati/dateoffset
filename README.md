# Lib dateoffset

### Modifies the system time for a single application or container through Linux syscall

## 1. What is it

Lib dateoffset provides a very simple way to run a single application or container with a different date, without changing the system clock.

It's useful for testing and debugging code that are time-dependant, like databases, certificates, and all sort of business logic that involves dates.

## 2. Motivation

If you are looking for a portable and full featured solution, you might opt for the excellent [libfaketime](https://github.com/wolfcw/libfaketime).

Unfortunately the way [libfaketime](https://github.com/wolfcw/libfaketime) works [clash with some applications](https://github.com/wolfcw/libfaketime/issues/130) using [jemalloc](http://jemalloc.net/) as memory allocator, most notably [Microsoft SQL Server for Linux](https://hub.docker.com/_/microsoft-mssql-server).

Lib dateoffset solves it by eliminating memory allocations caused by `dlsym` and directly calls Linux `clock_gettime` syscall.

I strong suggest you to try [libfaketime](https://github.com/wolfcw/libfaketime) first before using this one.

## 3. Usage

Just like [libfaketime](https://github.com/wolfcw/libfaketime), the basic way of running any command/program with Lib dateoffset is getting it loaded by system's linker using the environment variable `LD_PRELOAD`.

However, unlike [libfaketime](https://github.com/wolfcw/libfaketime), Lib dateoffset is much more limitted and has just one way to define the date, through the environment variable DATE_OFFSET with the start date in epoch format (seconds since 1970-01-01 00:00:00 UTC). You can use the command [date](https://man7.org/linux/man-pages/man1/date.1.html) to convert a date in format `yyyy-MM-dd` to seconds.

Examples:

a) Setting the environment variables

```bash
export LD_PRELOAD=/path/to/dateoffset.so
export DATE_OFFSET=$(date -d "2012/12/21 12:00:00" '+%s')
(now run any command you want)
```

b) Or it can be done by specifying it on the command line itself:

```bash
LD_PRELOAD=/path/to/dateoffset.so \
DATE_OFFSET=$(date -d "2019-10-30 00:00:00" '+%s') \
your_command_here
```

c) Changing just the date, but keeping the time unchanged

```bash
LD_PRELOAD=/path/to/dateoffset.so \
DATE_OFFSET=$(($(date -d "2016-10-12" '+%s') + ($(date '+%s') % (24*60*60)))) \
your_command_here
```

d) Dockerfile

```Dockerfile
ENV LD_PRELOAD=/path/to/dateoffset.so
ENV DATE_OFFSET=2090-07-04
ENTRYPOINT ["/my-app"]
```

## License

* This project is a free and unencumbered software released into the public domain. Plese visit [unlicense.org](https://unlicense.org/) for more details.

