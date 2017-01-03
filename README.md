# sbsdump

The sbsdump tool connects to ADS-B devices on TCP port 30003 by default and dumps data in SBS format.

## Installation

`make`

## Usage

```
Usage: sbsdump [OPTION]... hostname
Dump data in SBS format from a socket.
  -h display this help and exit
  -p set port (default 30003)
  -r show messages in raw format
  -u show only unique hex ids
```

## Examples

`./sbsdump localhost`

`./sbsdump -p 30001 localhost`

`./sbsdump -r localhost`

`./sbsdump -u localhost`

`./sbsdump -u localhost | ./lookup.sh`
