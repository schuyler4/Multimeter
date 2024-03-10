<h2> Multimeter Test Method </h2>
<h3> Accuracy/Precision Test </h3>
<p>
Before running the test, the multimeter is calibrated/adjusted by measuring a Voltage/Resistance/Capacitance reference at
a single value, and adjusting the calibration parameters to make the error as low as possible. This should minimize 
systematic error.
</p>
<p>
Accuracy and precision is tested by taking measurements of a Voltage/Resistance/Capacitance reference at various values.
The firmware is put in logging mode, and 100 samples are collected. These 100 samples are then run through
the test script to find the accuracy and precision. Only accuracy is tested with capacitance, as only
one sample can be captured at a time.
</p>
<h3> References </h3>
<ul>
<li> Voltage Reference: AD584K based voltage reference (2.5V/5.0V (Calibration Value)/7.5V/10V) </li>
<li> Resistor Reference: (0.1%) THT Resistors (10Ω, 100Ω, 1kΩ (Calibration Value), 10kΩ, 40kΩ) </li>
<li> Capacitor Reference: (1%) THT Capacitors C0G (0.01uF, 0.1uF (Calibration Value), 1uF, 10uF) </li> 
</ul>
<h3> Preliminary Testing </h3>
<p>
Preliminary testing can be done with adjustable power supply and lower tolerance components. However, this obviously will
not be as accurate. 
</p>
