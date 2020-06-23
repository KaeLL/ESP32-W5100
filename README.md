Port of the WIZnet W5100 to the ESP-IDF

This is a test app that runs the basic esp-idf
mqtt and http_client examples over the W5100
Ethernet chip.

The code makes assumptions to fit my specific
design, such as GPIOs, but the most important one
is polling status instead of using interrupts.

Hot to get it:
Just run
```
git clone --recursive https://github.com/KaeLL/esp32_w5100.git
```