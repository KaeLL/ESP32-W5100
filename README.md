Port of the WIZnet W5100 to the ESP-IDF

This is a test app that runs the basic esp-idf
mqtt/ssl and esp_http_client examples over the
W5100 Ethernet chip.

**Notice**: MQTT**S** is disabled due to the
certificate expiring. When espressif update the
mqtt example certificate, I'll update here as
well.

The code assumes my specific design, such as
GPIOs, but the most important one is polling
status instead of using interrupts.

Base IDF rev.:
espressif/esp-idf@1067b2870
