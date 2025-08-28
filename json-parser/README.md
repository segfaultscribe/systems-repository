# Lightweight JSON Parser in C

This project is a minimalist, low-level JSON parser written in C, designed to help understand how JSON parsing works under the hood using a clean `deterministic finite automaton (DFA)` style `state machine` approach.

It parses JSON directly from a file stream character-by-character and outputs key-value pairs in a flat, streamable format. <br><br>Currently it prints to the console, you can modify the output as you require.

- Parses flat and nested **JSON objects**
- Handles **JSON arrays**, emitting indexed keys (e.g., `tags[0]`)
- Supports all JSON value types:
  - Strings `"text"`
  - Numbers `123`, `3.14`, `-5.0`
  - Literals: `true`, `false`, `null`
- Constructs **fully qualified keys** using dot and bracket notation:
  - `user.name`, `tags[2]`, `profile.details.age`
- Uses a **deterministic state machine** no external dependencies or recursion

## 🧰 Example Output

**Input JSON:**
```json
{
  "user": {
    "name": "Alice",
    "age": 30
  },
  "tags": ["admin", "user", "editor"],
  "active": true
}
```

**Console Output**<br>

```
Key: user.name, Value: Alice
Key: user.age, Value: 30
Key: tags[0], Value: admin
Key: tags[1], Value: user
Key: tags[2], Value: editor
Key: active, Value: true
```

