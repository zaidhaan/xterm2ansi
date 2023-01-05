# xterm2ansi

xterm2ansi is a command line utility for converting xterm escape codes to ANSI escape codes. It can convert xterm-256 escape codes to 4-bit ANSI escape codes.

## Usage
* `echo -e "text with xterm-256 escaped codes" | xterm2ansi`
* `xterm2ansi input.file`
* `[ansi_output_command] | xterm2ansi`

## Example
```sh
echo -e "\033[38;5;80mcyan\033[m" | xterm2ansi | cat -v
# ^[[36mcyan^[[0m
```

## Installation
```sh
git clone https://github.com/zaidhaan/xterm2ansi.git
cd xterm2ansi
make
make install
```

# TODO
Unlikely I'll work on this for long, but might be helpful to have
- [ ] input specified by `-f`
- [ ] parse first as input if no `-f` specified
- [ ] help and man page?
