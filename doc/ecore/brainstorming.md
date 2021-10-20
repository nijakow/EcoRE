| Code                       | Equivalence                  |
| -------------------------- | ---------------------------- |
| `1 @ 2 @ nil`              | `(1 @ 2) @ nil`              |
| `1 + 2 + 3`                | `(1 + 2) + 3`                |
| `1 + 2 sqrt`               | `(1 + 2) sqrt`               |
| `stream << text <<newline` | `(stream << text) <<newline` |



# Streams

```
{
	Init => (
		| s <- StringStream new |
		s << 'Hello, world!';
		s <<newline;
		s << 42;       "The number 42"
		s char<< 42;   "The character '*'"
		s emit;        "-> 'Hello, world!\n42*'"
	);
}
```

