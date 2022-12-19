# Notes
___
## Content
- [Notes](#notes)
  - [Content](#content)
    - [Description](#description)
    - [Functional diagrams](#functional-diagrams)
    - [Folders](#folders)
    - [External dependencies](#external-dependencies)

### Description
- PPG or ECG meters
- The chip allows you to capture PPG or ECG data
- The component includes
  - managing abstract level for low-level functions
  - collecting PPG or ECG data and buffering them
  - settings management
- Files:
  - [AFE49I30_Types.h](AFE49I30_Types.h) - contains defining types
  - [AFE49I30_Config.h](AFE49I30_Config.h) - contains default settings, constants, and other configurable values
  - [AFE49I30_Adapter.c](AFE49I30_Adapter.c) - contains a low-level adapter for hardware management functions
  - [AFE49I30_Control.c](AFE49I30_Control.c) - contains management functions, status updates, processing

### Functional diagrams
- Initialization

  ![Initialization](Documents/functional_diagram-Initialization.jpg)

- Time synchronization

  ![Time synchronization](Documents/functional_diagram-TimeSynchronization.jpg)

- Handler

  ![Handler](Documents/functional_diagram-Handler.jpg)

### Folders
- [Documents](Documents) - contains a diagrams, description of the component modules, etc.

### External dependencies
- [Libs/AFE49I30](/Libs/AFE49I30)
- [Core/Inc/main.h](/Core/Inc/main.h)
- [Core/Inc/i2c.h](/Core/Inc/i2c.h)