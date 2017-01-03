#!/bin/sh

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
    echo "Time:\t\t$time"
    echo "Registration:\t$registration"
    echo "Model:\t\t$model"
    echo "Airline:\t$airline"
    echo "Flight:\t\t$callsign"
    echo "Route:\t\t$route"
    echo "Image:\t\t$image"
    echo "FR24:\t\thttps://www.flightradar24.com/$callsign"
    echo
done
