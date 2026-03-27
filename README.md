# http-chomik-server

**http-chomik-server** is a Unix-specific Chomik dialect that introduces a built-in HTTP server loop, allowing Chomik programs to serve dynamically generated HTML pages. It follows the same extension pattern as `sdl_chomik` — a C++ subclass of `libchomik`'s machine class — and inherits all core Chomik features.

The server is intended to replace traditional widget toolkits for GUI needs. A browser gives you forms, dropdowns, responsive layout, and JavaScript interactivity without any widget library code. The entire UI is generated from Chomik knowledge, using whatever HTML structure the handler program produces.

---

## How it works

A single statement starts the server:

```chomik
let the http chomik server file name = value string "handler.chomik";

<http chomik server loop at port 5001>;

# execution continues immediately — the server runs in a forked process
<print "server started on port 5001">;
```

Unlike `sdl_chomik`'s `<sdl loop>`, which blocks until the window is closed, `<http chomik server loop at port N>` forks a server process and returns immediately. The calling program continues executing. This allows starting multiple servers on different ports, or performing additional work after the server is running.

### Request handling

When a browser connects:

1. The central server process forks a *request handler process*.
2. The handler process parses the file named in `the http chomik server file name`.
3. It receives pipe file descriptors for communicating with the central machine, stored in predefined variables.
4. It executes `the http chomik server request handler` — a code variable the handler program assigns — which generates the HTTP response.

### The central machine

The central server process holds a persistent Chomik machine. Its variable space acts as a shared blackboard between all request handler processes. Handler processes can read from and write to it via pipes, making it possible to maintain state across requests without a separate database for simple use cases.

---

## Predefined variables

| Variable | Type | Description |
|---|---|---|
| `the http chomik server file name` | string | Path to the Chomik source file parsed for each request |
| `the http chomik server request handler` | code | Executed by the handler process to generate the HTTP response |
| `the http chomik server request url` | string | The URL requested by the browser |
| `the http chomik server request method` | string | The HTTP method (`GET`, `POST`, etc.) |
| `the http chomik server input pipe` | integer | File descriptor for reading from the central machine |
| `the http chomik server output pipe` | integer | File descriptor for writing to the central machine |

---

## Example handler

```chomik
#!/usr/local/bin/http_chomik_server

# optionally include a Chomik HTML knowledge library
let knowledge about chomik prefix = value integer 0;
include "knowledge_about_chomik.chomik"

let the http chomik server request handler = value code
{
    <print "HTTP/1.1 200 OK">;
    <print "Content-Type: text/html">;
    <print>;
    <print "<html><body>">;
    <print "<h1>Chomik</h1>">;
    <print <<knowledge about chomik prefix> knowledge about chomik what is chomik>>;
    <print "</body></html>">;
};
```

---

## HTML knowledge library

http-chomik-server is designed to work with an external Chomik library providing knowledge about HTML — tags, attributes, document structure — written entirely in Chomik. This keeps the dialect itself free of hardcoded HTML knowledge and allows the presentation layer to evolve independently of the server machinery.

See [knowledge-about-chomik](https://github.com/pawelbiernacki/knowledge-about-chomik) for the related knowledge base project.

---

## SDL hybrid

A planned future extension will allow an `sdl_chomik` program to be aware of a running http-chomik-server central machine and communicate with it via pipes. This enables a pattern where a graphical SDL application can present HTML forms in a browser for input — character creation, configuration, save/load screens — while the SDL loop handles real-time rendering. Both sides communicate through named variables in the central machine's shared variable space.

This hybrid will be Unix-specific, as it depends on `fork()` and pipes.

---

## Platform

**Unix/Linux only.** This dialect relies on `fork()` and POSIX pipes for its process model. There are no plans to support Windows.

---

## Building

Requires `libchomik` to be installed. See the [chomik repository](https://github.com/pawelbiernacki/chomik) for build instructions.

```bash
git clone https://github.com/pawelbiernacki/http-chomik-server.git
cd http-chomik-server
autoreconf -i
./configure
make
sudo make install
```

---

## Relationship to http_chomik and fancy_http_chomik

`http_chomik` and `fancy_http_chomik` (included in the main `chomik` repository) are sandbox environments — self-contained HTTP servers with a fixed structure. `http-chomik-server` is a dialect: a lower-level, more flexible tool that gives the Chomik programmer direct control over the server loop, the request handler, and the communication with the central machine. It is the foundation on which richer HTTP-based Chomik applications can be built.

---

## License

GPL 3.0. For commercial use, contact the author: pawel.f.biernacki@gmail.com

**Website:** [https://www.chomik.tech](https://www.chomik.tech)  
**Blog:** [https://chomik.hashnode.dev](https://chomik.hashnode.dev)
