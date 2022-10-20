# OPERATOR

***

## Raspberry Pi ZERO W with MQTT broker.

***

### MQTT BROKER:

Installing broker:

    sudo apt install mosquitto mosquitto-clients

Broker status:
    
    sudo systemctl status mosquitto
    
For test MQTT BROKER:

Terminal1:

    mosquitto_sub -h localhost -t "test"

Terminal2:

    mosquitto_pub -h localhost -t "test" -m "MQTT status: OK"

***