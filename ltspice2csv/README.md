ltspice2csv
===========

An export file of an LTSpice plot (*.txt). Currently tested with ".tran" and ".ac" - time and frequency resonse plots. 

The export may include several "probes" to be converted.

How to use:
-----------
With DC (time, voltage):

```
  ltspice2csv -i INPUT.txt -o OUTPUT.csv
```

With AC (freq, (mag, phase)):

```
  ltspice2csv -i INPUT.txt -o OUTPUT.csv -a
```

INSTALL:
--------
Compile:

```
  make
```

Install:

```
  make install
```

Uninstall

```
  make uninstall
```

PLOT DATA IN MATLAB/OCTAVE
--------------------------

```octave
  %% Read and plot csv
  clc
  m = csvread('ac.csv', 1, 0);
  freq = m(:,1);
  mag = m(:,2);
  phase = m(:,3);
  semilogx (freq,mag,freq,phase)  
  grid on;
  title('AC Frequency Response');
  xlabel('Frequency (Hz)');
  ylabel('Response (dB)');
  legend('Magnitude', 'Phase');
  % Optional: Save plot as EPS
  print -depsc 'ac.eps';
```
