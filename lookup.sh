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
    echo -e "Time:\t\t$time"
    echo -e "Registration:\t$registration"
    echo -e "Model:\t\t$model"
    echo -e "Airline:\t$airline"
    echo -e "Flight:\t\t$callsign"
    echo -e "Route:\t\t$route"
    echo -e "Image:\t\t$image"
    echo -e "FR24:\t\thttps://www.flightradar24.com/$callsign"
    echo
done
