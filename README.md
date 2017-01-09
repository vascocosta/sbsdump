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

```
pi@neptunium:~/src/github.com/vascocosta/sbsdump $ ./sbsdump localhost
Callsign: VKG605
Altitude: 0 Ground speed: 0 Vertical rate: 0
Track: 0 Lat: 0.000000 Lon: 0.000000
Squawk: 0

Callsign: empty
Altitude: 0 Ground speed: 0 Vertical rate: 0
Track: 0 Lat: 0.000000 Lon: 0.000000
Squawk: 0

Callsign: empty
Altitude: 32000 Ground speed: 0 Vertical rate: 0
Track: 0 Lat: 37.961521 Lon: -8.618370
Squawk: 0
```

```
pi@neptunium:~/src/github.com/vascocosta/sbsdump $ ./sbsdump -r localhost
MSG,3,111,11111,4951CD,111111,2017/01/09,20:57:04.943,2017/01/09,20:57:04.943,,6050,,,38.80811,-9.23425,,,,,,0
MSG,4,111,11111,4951CD,111111,2017/01/09,20:57:05.113,2017/01/09,20:57:05.078,,,272,207,,,-704,,,,,0
MSG,3,111,11111,4951CD,111111,2017/01/09,20:57:05.442,2017/01/09,20:57:05.406,,6050,,,38.80753,-9.23456,,,,,,0
MSG,1,111,11111,4951CD,111111,2017/01/09,20:57:05.902,2017/01/09,20:57:05.865,TAP377  ,,,,,,,,,,,0
MSG,3,111,11111,4951CD,111111,2017/01/09,20:57:06.002,2017/01/09,20:57:05.993,,6050,,,38.80692,-9.23499,,,,,,0
```

```
pi@neptunium:~/src/github.com/vascocosta/sbsdump $ ./sbsdump -u localhost
Hex ID: 4951CB Callsign: TAP1042
Hex ID: 491147 Callsign: TAP1098
Hex ID: 45D963 Callsign: VKG605
```

```
pi@neptunium:~/src/github.com/vascocosta/sbsdump $ ./sbsdump -u localhost | ./lookup.sh
Time:           21:08:26
Registration:   CS-TNW
Model:          Airbus A-320-214
Airline:        TAP
Flight:         TAP1230
Route:          LPPT-UUDD
Image:          http://www.airport-data.com/images/aircraft/small/001/222/001222582.jpg
FR24:           https://www.flightradar24.com/TAP1230

Time:           21:08:29
Registration:   EI-EBS
Model:          Boeing 737-8AS
Airline:        Ryanair
Flight:         RYR2624
Route:          LPPD-LPPT
Image:          http://www.airport-data.com/images/aircraft/small/001/215/001215913.jpg
FR24:           https://www.flightradar24.com/RYR2624
```
