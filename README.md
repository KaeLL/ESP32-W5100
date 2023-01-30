W5100 driver port test app for the ESP32/ESP-IDF

This is a test app containing ESP-IDF's mqtt/ssl
and esp_http_client examples, as well as a
lower-level layer implementation of the driver
interface.

The app is modeled after any other
ESP-IDF example, so the steps to run it are the
same.

The code assumes my specific design with regards
to GPIOs and polling the chip instead of using
interrupts. There's also a sdkconfig.defaults
file with my configurations. If you're having
issues running the example, deleting all
sdkconfig files in the root may help.

Last update based on: IDF v5.2.1
