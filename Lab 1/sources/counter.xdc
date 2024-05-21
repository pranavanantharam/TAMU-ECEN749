##Buttons
set_property PACKAGE_PIN Y16 [get_ports {BUTTONS[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {BUTTONS[0]}]

set_property PACKAGE_PIN K19 [get_ports {BUTTONS[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {BUTTONS[1]}]

##LEDs

set_property PACKAGE_PIN M14 [get_ports {LEDS[0]}]
set_property IOSTANDARD LVCMOS33 [get_ports {LEDS[0]}]

set_property PACKAGE_PIN M15 [get_ports {LEDS[1]}]
set_property IOSTANDARD LVCMOS33 [get_ports {LEDS[1]}]

set_property PACKAGE_PIN G14 [get_ports {LEDS[2]}]
set_property IOSTANDARD LVCMOS33 [get_ports {LEDS[2]}]

set_property PACKAGE_PIN D18 [get_ports {LEDS[3]}]
set_property IOSTANDARD LVCMOS33 [get_ports {LEDS[3]}]

##Reset
set_property PACKAGE_PIN P16 [get_ports {reset}]
set_property IOSTANDARD LVCMOS33 [get_ports {reset}]

##Clock
set_property PACKAGE_PIN K17 [get_ports {clock}]
set_property IOSTANDARD LVCMOS33 [get_ports {clock}]
