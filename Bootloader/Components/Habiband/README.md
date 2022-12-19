# Notes
___
## Content
- [Notes](#notes)
  - [Content](#content)
    - [Description](#description)
    - [Component diagrams](#component-diagrams)
    - [BLE logic diagrams](#ble-logic-diagrams)
    - [BLE Updates](#ble-updates)
    - [BLE Packets](#ble-packets)
    - [Folders](#folders)
    - [External dependencies](#external-dependencies)

### Description
- Implementation of Habiband firmware by BLE
- The component includes:
  - abstract level for managing low-level functions
  - status monitoring and management
  - working with BLE
  - processing of the included components
- Files:
  - [Habiband_BootloaderTypes.h](Habiband_BootloaderTypes.h) contains types
  - [Habiband_BootloaderConfig.h](Habiband_BootloaderConfig.h) - contains default settings, constants, and other configurable values
  - [Habiband_Bootloader.с](Habiband_Control.с) contains common logic and handlers
  - [Habiband_BootloaderRequestsReceiver.c](Habiband_BootloaderRequestsReceiver.c) contains a request handler for incoming BLE requests, a request parser

### Component diagrams
- Initialization:
  
  ![Initialization](Documents/functional_diagram-Initialization.jpg)

- Inclusions:

  ![TimeSynchronization](Documents/functional_diagram-TimeSynchronization.jpg)

- Handler:

  ![Handler](Documents/functional_diagram-Handler.jpg)

### BLE logic diagrams
- Event listener:
  
  ![Event listener](Documents/functional_diagram_ble-BLE_EventListener.jpg)

- Requests receiver:
  
  ![Requests receiver](Documents/functional_diagram_ble-RequestsReceiver.jpg)
  
- Downloading the firmware:
  
  ![Downloading the firmware](Documents/functional_diagram_ble-DownloadingTheFirmware.jpg)

### BLE Updates
- UpdateBootloaderStatus
  
  ![image](Documents/functional_diagram_ble_updates-UpdateBootloaderStatus.jpg)

- UpdateLoadingFirmware
  
  ![image](Documents/functional_diagram_ble_updates-UpdateLoadingFirmware.jpg)

### BLE Packets
___
- RequestGetSessionKey
  
  ![image](Documents/functional_diagram_ble_packet-RequestGetSessionKey.jpg)

- ResponseGetSessionKey
  
  ![image](Documents/functional_diagram_ble_packet-ResponseGetSessionKey.jpg)
___
- RequestConfirmSessionKey
  
  ![image](Documents/functional_diagram_ble_packet-RequestConfirmSessionKey.jpg)

- ResponseConfirmSessionKey
  
  ![image](Documents/functional_diagram_ble_packet-ResponseConfirmSessionKey.jpg)
___
- RequestStartBoot
  
  ![image](Documents/functional_diagram_ble_packet-RequestStartBoot.jpg)

- ResponseStartBoot
  
  ![image](Documents/functional_diagram_ble_packet-ResponseStartBoot.jpg)
___
- RequestStartBoot
  
  ![image](Documents/functional_diagram_ble_packet-RequestStartBoot.jpg)

- ResponseStartBoot
  
  ![image](Documents/functional_diagram_ble_packet-ResponseStartBoot.jpg)
___
- RequestInitLoadFirmware
  
  ![image](Documents/functional_diagram_ble_packet-RequestInitLoadFirmware.jpg)

- ResponseInitLoadFirmware
  
  ![image](Documents/functional_diagram_ble_packet-ResponseInitLoadFirmware.jpg)
___
- RequestLoad
  
  ![image](Documents/functional_diagram_ble_packet-RequestLoad.jpg)

- ResponseLoad
  
  ![image](Documents/functional_diagram_ble_packet-ResponseLoad.jpg)
___
- RequestConfirmationLoadFirmware
  
  ![image](Documents/functional_diagram_ble_packet-RequestConfirmationLoadFirmware.jpg)

- ResponseConfirmationLoadFirmware
  
  ![image](Documents/functional_diagram_ble_packet-ResponseConfirmationLoadFirmware.jpg)
___
- RequestStartApp
  
  ![image](Documents/functional_diagram_ble_packet-RequestStartApp.jpg)

- ResponseStartApp
  
  ![image](Documents/functional_diagram_ble_packet-ResponseStartApp.jpg)
___
- RequestCancelLoadFirmware
  
  ![image](Documents/functional_diagram_ble_packet-RequestCancelLoadFirmware.jpg)

- ResponseCancelLoadFirmware
  
  ![image](Documents/functional_diagram_ble_packet-ResponseCancelLoadFirmware.jpg)
___
- RequestReset
  
  ![image](Documents/functional_diagram_ble_packet-RequestReset.jpg)

- ResponseReset
  
  ![image](Documents/functional_diagram_ble_packet-ResponseReset.jpg)
___

### Folders
- [Documents](Documents) - contains a diagrams, description of the component modules, etc.

### External dependencies
- [Bootloader](/Bootloader/Components/Bootloader)
- [STM32_WPAN/App](/Bootloader/STM32_WPAN/App)
- [Core](/Bootloader/Core)
