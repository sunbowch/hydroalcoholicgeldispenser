# hydroalcoolicgeldispenser

Software for a contactless automatic dispenser, based on arduino, stepper driver, stepper motor and distance sensor

The idea behind this project is to provide one of the most needed product in the present situation: Contactless dispensers for disinfectant solution that can be built in mass by the community to cover the local needs as soon as possible.

In the present situation, in order to limit the contamination, hand cleaning is one of the most useful thing that can be done.

In order to make it accessible to a maximum of builders, it is based on most common hardware that any owner of a 3D printer should have handy... Or can order to local suppliers.

The design is based on NEMA-17 motors of any lenght, but to avoid using an external converter to power the arduino, a voltage <=12V is recommended. So low resistance motors (rated with high current) are prefered. Any standard 16 pins stepper driver can be used, a sparkfun distance sensor (https://www.sparkfun.com/products/15569) and 2x 685 miniature ball bearings for the pump's rotor. This motors are the most common available and it is likely that any 3D printer user has some spare.

If the pump is manufactured carefully and has the lowest possible operating torque without leakage, it is possible to power the system with a 9V battery. Depending on the pump quality, it is sufficient for about 5'000 doses, or about 5 liters of solution, or 100 hours of standby, if you also remove the power led resistor of the Arduino.

Here you will find also STL files to build the pump. It is designed for silicone tubing of 3 mm inner diameter and 0.5 mm thickness.

The schematics are available on : https://easyeda.com/sunbow/hydroalcoolicgeldispenser. It has been designed to be made with a solder strip grid board.

More parts will follow... And complete build informations.
