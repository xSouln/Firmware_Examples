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
- Gyroscope, accelerometer and temperature sensor
- The component includes:
  - abstract level for managing low-level functions
  - status monitoring and management
  - settings management
- Files:
  - [LSM6DSOX_Types.h](LSM6DSOX_Types.h) - contains defining types
  - [LSM6DSOX_Config.h](LSM6DSOX_Config.h) - contains default settings, constants, and other configurable values
  - [LSM6DSOX_Adapter.c](LSM6DSOX_Adapter.c) - contains a low-level adapter for hardware management functions
  - [LSM6DSOX_Control.c](LSM6DSOX_Control.c) - contains management functions, status updates, processing

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
- [Libs/LSM6DSOX](/Libs/LSM6DSOX)
- [Core/Inc/main.h](/Core/Inc/main.h)
- [Core/Inc/i2c.h](/Core/Inc/i2c.h)