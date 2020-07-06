# 🍕 slice

A no-nonsense tool to copy a blob of data from one file to another.

Opens `input`, seeks to `start`, reads `length` bytes, and writes them to `output`.

## Options

```
-i,--input <input>
	Source file path to read from.
	[required]

-o,--output <output>
	Destination file path to write to.
	[required]

-s,--start <start>
	Start offset in bytes.
	[optional, defaults to 0 if omitted]

-n,--length <length>
	Length of data to copy, in bytes.
	[optional, reads to the end of input if omitted]
```

## Building

On any \*nix system it _should_ be as simple as:

```
git clone https://github.com/lunasorcery/slice.git
cd slice
make slice && make install
```

This depends on a C compiler being available. If you encounter issues, please let me know.
