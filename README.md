# Eurus Recruitment 2026-2027 - Task 1 Base

**Applicant:** Moustafa Hesham  
**ID:** 202400624

This repository contains the Task 1 base implementation used in the submitted report.

## Contents

- `arduino/Task1_Base/Task1_Base.ino` - Arduino UNO / ATmega328P firmware source.
- `matlab/Eurus_Task1_GUI.mlapp` - MATLAB App Designer scripted demonstration.
- `simulation/Task1_EURUS_Control.pdsprj` - Proteus project.
- `simulation/Task1_Base.ino.hex` - firmware image used by the Proteus project.
- `evidence/Arduino_Compile_Success_FINAL.png` - successful Arduino compilation evidence.
- `evidence/Proteus_06_Manual_ON_OFF_Demo.mp4` - qualitative simulated motor/buzzer ON-OFF demonstration.

## Declared scope and limitations

This is a simulation/demo implementation. No physical Arduino UNO or MQ2 hardware was tested. Proteus uses an ATMEGA328P VSM model rather than a complete physical UNO board. The A1 input is a potentiometer/analog-output substitute for MQ2 behavior, not a gas or ppm model, and the gas threshold is a demonstration threshold rather than a calibrated safety limit. The MATLAB app is a scripted demonstration and is not connected live to physical hardware or Proteus.

## AI assistance disclosure

ChatGPT was used substantially for learning support, source guidance/checking, Arduino and App Designer development assistance, debugging guidance, evidence review, calculations, and documentation. The local Proteus simulation and MATLAB app demonstrations were run by the applicant and the associated evidence was supplied from those runs.
