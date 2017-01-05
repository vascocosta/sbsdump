#!/bin/sh

ECHO="/bin/echo"

while read line
do
    hex_id=$(echo $line | awk '{print $3}')
    callsign=$(echo $line | awk '{print $5}')
    time=$(date +%T)
    registration=$(curl -s https://ae.roplan.es/api/hex-reg.php?hex=$hex_id)
    model=$(curl -s https://ae.roplan.es/api/hex-type.php?hex=$hex_id)
    airline=$(curl -s https://ae.roplan.es/api/hex-airline.php?hex=$hex_id)
    route=$(curl -s https://ae.roplan.es/api/callsign-route.php?callsign=$callsign)
    image=$(curl -s https://ae.roplan.es/api/hex-image.php?hex=$hex_id)
    $ECHO -e "Time:\t\t$time"
    $ECHO -e "Registration:\t$registration"
    $ECHO -e "Model:\t\t$model"
    $ECHO -e "Airline:\t$airline"
    $ECHO -e "Flight:\t\t$callsign"
    $ECHO -e "Route:\t\t$route"
    $ECHO -e "Image:\t\t$image"
    $ECHO -e "FR24:\t\thttps://www.flightradar24.com/$callsign"
    $ECHO
done
