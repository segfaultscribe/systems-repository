# Simple Persistent Key-Value Store (SPKVS)

A simple, persistent key-value store written in C for learning purposes.
Supports basic key-value operations through a CLI interface with data persisted in a file.

## Supported Commands

`PUT <key> <value>` :	    Add a new key-value pair to storage. Duplicate keys are rejected.<br>
`GET <key>` :	            Retrieve the value associated with a key.<br>
`DELETE <key>` :	        Remove a key-value pair from storage.<br>
`UPDATE <key> <value>` :	Update an existing key with a new value.<br>
`LIST` :	                List all stored key-value pairs.<br>
`HELP` :	                Show usage information.<br>
`exit` :	                Exit the CLI.<br>


Key Constraints:
<br>Duplicate keys are disallowed on PUT.<br>
Persistent storage file: store.txt

## Improvements

- Implement proper return codes and error handling for each function.
- Validate key and value lengths explicitly.
- Use a temporary file pattern to make handle_update safer from data loss.
- Add file locking to handle concurrent access.
- Optimize read/write performance by indexing keys in memory.
- Allow configurable storage file (via CLI args or config file).
- Add support for batch commands or script mode.
- Implement unit and integration tests.
- Improve CLI UX (e.g., command history, input validation).
- Support namespacing or key expiration.
- Add a compact binary format option for faster performance.

🛠️ Build & Run
```
gcc -o spkvs spkvs.c
./spkvs
```


Type `help` at the prompt for usage instructions.