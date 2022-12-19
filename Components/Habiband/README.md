# Notes
___
## Content
- [Notes](#notes)
  - [Content](#content)
    - [Description](#description)
    - [Functional diagrams](#functional-diagrams)
    - [Description BLE](#description-ble)
    - [Folders](#folders)
    - [External dependencies](#external-dependencies)

### Description
- Smart bracelet that allow you to measure PG or ECG, have an accelerometer, gyroscope, temperature sensor, built-in backlight, BLE module
- The component includes:
  - abstract level for managing low-level functions
  - status monitoring and management
  - working with BLE
  - processing of the included components
  - settings management
- Files:
  - [Habiband_Types.h](Habiband_types.h) contains types
  - [Habiband_Config.h](Habiband_Config.h) - contains default settings, constants, and other configurable values
  - [Habiband_BLE.h](Habiband_BLE.h) contains data types for working with the BLE abstraction
  - [Habiband_BLEAdapter.с](Habiband_BLEAdapter.с) contains initialization of the BLE management interface
  - [Habiband_Control.с](Habiband_Control.с) contains common logic and handlers
  - [Habiband_Component.с](Habiband_Component.с) contains initialization and implementation of the necessary interfaces for the operation of the component

### Functional diagrams
- Initialization:
  
  ![Initialization](Documents/functional_diagram-Initialization.jpg)

- Inclusions:

  ![Inclusions](Documents/functional_diagram-Inclusions.jpg)

- Time synchronization:

  ![TimeSynchronization](Documents/functional_diagram-TimeSynchronization.jpg)

- Handler:

  ![Handler](Documents/functional_diagram-Handler.jpg)

### Description BLE
- Event handler:
  
  ![HabibandEventListener](Documents/functional_diagram_ble-HabibandEventListener.jpg)

- BLE Handler:
  
  ![HabibandEventListener](Documents/functional_diagram_ble-HabibandBLEHandler.jpg)
  
- PPG Packet:
  
  ![PPG Packet](Documents/functional_diagram_ble-PPG_Packet.jpg)
  
- ECG Packet:
  
  ![ECG Packet](Documents/functional_diagram_ble-ECG_Packet.jpg)
  
- Gyroscope Packet:
  
  ![Gyroscope_Packet](Documents/functional_diagram_ble-Gyroscope_Packet.jpg)
  
- Accelerometer Packet:
  
  ![Accelerometer_Packet](Documents/functional_diagram_ble-Accelerometer_Packet.jpg)

### Folders
- [Documents](Documents) - contains a diagrams, description of the component modules, etc.

### External dependencies
- [Components/ADP5360](/Components/ADP5360)
- [Components/AFE49I30](/Components/AFE49I30)
- [Components/LM2755](/Components/LM2755)
- [Components/LSM6DSOX](/Components/LSM6DSOX)
- [Components/MAX30208](/Components/MAX30208)
- [Bootloader/Components/Bootloader](/Bootloader/Components/Bootloader)
- [STM32_WPAN/App](/STM32_WPAN/App)
- [Core](/Core)
