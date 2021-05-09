# Arduino Remote Relay

An simple design that allows you to control relays via Ethernet.
The project was dedicated to testing physical equipment on the production line. 
Communication is done via HTTP, so it is easy to integrate it with test frameworks such as JUnit.

## Configuration

The configuration is located on the SD card (FAT32), see the *SD* directory in this repository.
Files on the SD card:
 * IP - contains static IP address

## Runtime

To list all relays:
```
GET http://192.168.0.200/modules
```
To show relay details:
```
GET http://192.168.0.200/modules/relay1
```
To enable relay:
```
GET http://192.168.0.200/modules/relay1?value=1
```
To disable relay:
```
GET http://192.168.0.200/modules/relay1?value=0
```

Note: *The last two requests would have to be made using the http PUT method, but due to limited Arduino resources GET was used...*
