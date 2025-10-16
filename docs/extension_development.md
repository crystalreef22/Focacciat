# Extension development

There are Three ways to develop extensions for Focacciat: Plugin, Python, and
IPC. Only one of them currently works. Actually it doesn't work fully yet.
Anyways!!!

# IPC

Focacciat uses named pipes on Windows and Unix Domain Sockets on Linux/Mac to
communicate with certain extensions. It is not recommended to access them
directly. Instead, you will find a program in the same directory Focacciat is
installed in, or in MacOS, 
`/Applications/Focacciat.app/Contents/MacOS/extensionhost`. This is intended to
allow Focacciat to communicate with the web browser, though any program may use
this. All messages to and from it must be in valid JSON and be preceded by a
32-bit unsigned integer representing the amount of bytes in the message.

## Format

Note that those comments are not supported

Also note that this feature is not yet implemented.

First we start with a handshake so that the programs know which version of
Focacciat's ipc we are using. Focacciat sends:

```json
{
    "type": "response",
    "action": "ping",
    "version": 0,
    "supportedVersions": [0]
}
```

Please reply:

```json
{
    "type": "request",
    "action": "connection",
    "version": 0
}
```

Focacciat responds:

```json
{
    "type": "response",
    "action": "connection",
}
```

or if error

```json
{
    "type": "response",
    "action": "connection",
    "error": {
        "code": "unsupported_version"
    }
}
```

or if error

```json
{
    "type": "response",
    "error": {
        "code": "invalid_message"
    }
}
```

Once a successful response is recieved from Focacciat, you may start doing stuff

## ping

```json
{
    "type": "request",
    "action": "ping"
}
```
-->
```json
{
    "type": "response",
    "action": "ping",
    "version": 0,
}
```

## Retrieve blocklist

```json
{
    "type": "request",
    "action": "blocklist.get"
}
```
-->
```json
{
    "type": "response",
    "action": "blocklist.get",
    "data": [
        "example.com",
        "example.org"
    ]
}
```

## Add to currently enabled blocklist

```json
{
    "type": "request",
    "action": "blocklist.append",
    "data": ["example.net"]
}
```
-->
```json
{
    "type": "response",
    "action": "blocklist.append",
}
