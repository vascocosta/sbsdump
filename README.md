# sbsdump

The sbsdump tool connects to ADS-B devices on TCP port 30003 by default and dumps data in SBS format.

## Installation

`make`

## Usage

```
pi@neptunium:~/src/github.com/vascocosta/sbsdump $ ./sbsdump
Usage: sbsdump [OPTION]... hostname
Dump data in SBS format from a socket.
  -c show only aircrafts with a callsign in spotting mode
  -h display this help and exit
  -p set port (default 30003)
  -r show messages in raw format
  -s show only new aircrafts excluding dups (spotting mode)
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
pi@neptunium:~/src/github.com/vascocosta/sbsdump $ ./sbsdump -s localhost
Date:           2017/01/20
Time:           16:34:50.164
Hex:            020066
Registration:   CN-ROE
Model:          Boeing 737-8B6
Airline:        Royal Air Maroc
Image:          http://www.airport-data.com/images/aircraft/small/001/111/001111826.jpg
FR24:           https://www.flightradar24.com/data/aircraft/CN-ROE

Date:           2017/01/20
Time:           16:34:50.484
Hex:            4CA853
Registration:   EI-EMK
Model:          Boeing 737-8AS
Airline:        Ryanair
Image:          http://www.airport-data.com/images/aircraft/small/001/260/001260520.jpg
FR24:           https://www.flightradar24.com/data/aircraft/EI-EMK

Date:           2017/01/20
Time:           16:39:19.409
Hex:            495292
Registration:   CS-TTR
Model:          Airbus A319-112
Airline:        TAP Air Portugal
Image:          http://www.airport-data.com/images/aircraft/small/001/245/001245538.jpg
FR24:           https://www.flightradar24.com/data/aircraft/CS-TTR
```
