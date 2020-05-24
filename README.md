# Make Jampa IoT

Sistema para monitoramento de consumo energético residencial e comercial que utiliza as plataformas de prototipagem de hardware Arduino e ESP8266.

A corrente é detectada pelo sensor de corrente não invasivo SCT013, lida pelo Arduino com auxílio da biblioteca `emonLib`, enviada por comunicação serial para o ESP8266 e enviada para um servidor através do protocolo `mqtt`.

## Referencias

[Sensores CT e Arduino](https://learn.openenergymonitor.org/electricity-monitoring/ct-sensors/interface-with-arduino)

[emonLib](https://github.com/openenergymonitor/EmonLib)

[mqtt e Arduino](https://techtutorialsx.com/2017/04/09/esp8266-connecting-to-mqtt-broker/)